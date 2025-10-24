# 软件设计文档

## 1. 开发环境

### 1.1 工具链

**集成开发环境**:
- IDE: Keil MDK 5.x
- 编译器: ARM Compiler v5/v6
- 调试器: ST-Link / J-Link

**辅助工具**:
- 串口调试: Serial Port Utility / PuTTY
- 取模软件: Image2Lcd (OLED图像)
- 版本控制: Git

### 1.2 软件库

**STM32标准外设库 (SPL)**:
- 版本: STM32F10x_StdPeriph_Lib_V3.5.0
- 优点: API清晰,易于理解和使用
- 缺点: 不再更新,但足够稳定

**第三方库**:
- OLED驱动库: 自定义I2C驱动
- 语音合成库: SYN6288通信协议封装

## 2. 软件架构

### 2.1 分层设计

```
┌─────────────────────────────────────┐
│        应用层 (Application)          │
│  - 主程序逻辑                        │
│  - 状态机管理                        │
│  - 指令解析                          │
└─────────────────────────────────────┘
                 ↓
┌─────────────────────────────────────┐
│        中间层 (Middleware)           │
│  - 运动控制算法                      │
│  - 传感器数据处理                    │
│  - 通信协议                          │
│  - 避障逻辑                          │
└─────────────────────────────────────┘
                 ↓
┌─────────────────────────────────────┐
│        驱动层 (Driver)               │
│  - 舵机驱动                          │
│  - 传感器驱动                        │
│  - 串口驱动                          │
│  - OLED驱动                          │
└─────────────────────────────────────┘
                 ↓
┌─────────────────────────────────────┐
│        硬件抽象层 (HAL)              │
│  - GPIO / TIM / USART / I2C          │
│  - STM32标准外设库                   │
└─────────────────────────────────────┘
```

### 2.2 模块划分

**Hardware/** - 硬件驱动层
- `Servo.c/h`: 舵机控制
- `PWM.c/h`: PWM信号生成
- `UltrasonicWave.c/h`: 超声波测距
- `Hongwai.c/h`: 红外传感器
- `OLED.c/h`: OLED显示驱动
- `Serial.c/h`: 串口通信
- `syn6288.c/h`: 语音合成
- `LED.c/h`: LED控制
- `Timer.c/h`: 定时器配置

**System/** - 系统层
- `Delay.c/h`: 延时函数
- `usart1.c/h`: 蓝牙串口
- `usart2.c/h`: 语音合成串口
- `usart3.c/h`: 语音识别串口

**User/** - 应用层
- `main.c`: 主程序
- `Mode.c/h`: 行为模式
- `Movement.c/h`: 运动控制
- `stm32f10x_it.c/h`: 中断服务程序

## 3. 核心模块设计

### 3.1 舵机控制模块

#### 3.1.1 PWM配置

```c
// PWM初始化
void PWM_Init(void) {
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 2000 - 1;        // ARR: 20ms周期
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;      // PSC: 72MHz/720=100KHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 4. 配置PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 150;                // CCR: 1.5ms中位
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    // 5. 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}
```

#### 3.1.2 角度控制

```c
// 角度到CCR值的转换
// 0° → 0.5ms → CCR=50
// 90° → 1.5ms → CCR=150
// 180° → 2.5ms → CCR=250
void Servo_SetAngle(uint8_t channel, uint8_t angle) {
    uint16_t ccr = (uint16_t)(50 + angle * 200.0 / 180.0);
    
    switch(channel) {
        case 1: TIM_SetCompare1(TIM2, ccr); break;
        case 2: TIM_SetCompare2(TIM2, ccr); break;
        case 3: TIM_SetCompare3(TIM2, ccr); break;
        case 4: TIM_SetCompare4(TIM2, ccr); break;
        // ... 其他通道
    }
}
```

#### 3.1.3 平滑控制

```c
// 平滑转动函数
void Servo_SmoothMove(uint8_t channel, uint8_t targetAngle, uint16_t duration) {
    uint8_t currentAngle = Servo_GetAngle(channel);
    int16_t deltaAngle = targetAngle - currentAngle;
    uint16_t steps = duration / 20;  // 20ms刷新周期
    float stepAngle = (float)deltaAngle / steps;
    
    for(uint16_t i = 0; i < steps; i++) {
        currentAngle += stepAngle;
        Servo_SetAngle(channel, currentAngle);
        Delay_ms(20);
    }
    
    Servo_SetAngle(channel, targetAngle);  // 确保到达目标
}
```

### 3.2 运动控制模块

#### 3.2.1 步态定义

```c
// 步态结构体
typedef struct {
    uint8_t servo_id;      // 舵机ID
    uint8_t angle;         // 目标角度
    uint16_t duration;     // 运动时间(ms)
} GaitStep;

// 前进步态序列
GaitStep forward_gait[] = {
    // 阶段1: 抬起左前腿和右后腿
    {1, 60, 200},   // 左前髋
    {2, 45, 200},   // 左前膝
    {7, 120, 200},  // 右后髋
    {8, 135, 200},  // 右后膝
    
    // 阶段2: 身体前移
    {3, 100, 300},  // 右前髋
    {5, 80, 300},   // 左后髋
    
    // 阶段3: 落下左前腿和右后腿
    {1, 90, 200},
    {2, 90, 200},
    {7, 90, 200},
    {8, 90, 200},
    
    // 阶段4: 抬起右前腿和左后腿
    // ...
};
```

#### 3.2.2 对角步态实现

```c
// 对角步态 - 前进
void Movement_Forward(void) {
    // 第一组对角腿 (左前 + 右后)
    Servo_SmoothMove(1, 60, 200);   // 左前髋抬起
    Servo_SmoothMove(2, 45, 200);   // 左前膝抬起
    Servo_SmoothMove(7, 120, 200);  // 右后髋抬起
    Servo_SmoothMove(8, 135, 200);  // 右后膝抬起
    
    Delay_ms(200);
    
    // 身体前移 (支撑腿蹬地)
    Servo_SmoothMove(3, 100, 300);  // 右前髋后摆
    Servo_SmoothMove(5, 80, 300);   // 左后髋后摆
    
    Delay_ms(300);
    
    // 第一组对角腿落下
    Servo_SmoothMove(1, 90, 200);
    Servo_SmoothMove(2, 90, 200);
    Servo_SmoothMove(7, 90, 200);
    Servo_SmoothMove(8, 90, 200);
    
    Delay_ms(200);
    
    // 第二组对角腿 (右前 + 左后)
    // ... 类似操作
}
```

#### 3.2.3 姿态控制

```c
// 站立姿态
void Posture_Stand(void) {
    uint8_t stand_angles[8] = {90, 90, 90, 90, 90, 90, 90, 90};
    
    for(uint8_t i = 0; i < 8; i++) {
        Servo_SmoothMove(i+1, stand_angles[i], 1000);
    }
}

// 坐下姿态
void Posture_Sit(void) {
    // 前腿保持站立
    Servo_SmoothMove(1, 90, 800);
    Servo_SmoothMove(2, 90, 800);
    Servo_SmoothMove(3, 90, 800);
    Servo_SmoothMove(4, 90, 800);
    
    // 后腿弯曲
    Servo_SmoothMove(5, 120, 800);
    Servo_SmoothMove(6, 60, 800);
    Servo_SmoothMove(7, 60, 800);
    Servo_SmoothMove(8, 120, 800);
}

// 趴下姿态
void Posture_Lie(void) {
    uint8_t lie_angles[8] = {45, 135, 135, 45, 45, 135, 135, 45};
    
    for(uint8_t i = 0; i < 8; i++) {
        Servo_SmoothMove(i+1, lie_angles[i], 1000);
    }
}
```

### 3.3 传感器处理模块

#### 3.3.1 超声波测距

```c
// 超声波测距函数
float UltrasonicWave_GetDistance(void) {
    uint32_t time = 0;
    
    // 1. 发送触发信号
    GPIO_SetBits(GPIOB, GPIO_Pin_12);   // Trig = 1
    Delay_us(15);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); // Trig = 0
    
    // 2. 等待Echo变高
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0);
    
    // 3. 测量高电平时间
    TIM_SetCounter(TIM4, 0);
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 1) {
        time = TIM_GetCounter(TIM4);
        if(time > 30000) break;  // 超时保护
    }
    
    // 4. 计算距离 (cm)
    // 距离 = (时间 × 声速) / 2
    // 声速 = 340m/s = 0.034cm/us
    float distance = time * 0.034 / 2;
    
    return distance;
}

// 滤波函数
float UltrasonicWave_GetDistance_Filtered(void) {
    float distances[5];
    float sum = 0;
    
    // 连续测量5次
    for(uint8_t i = 0; i < 5; i++) {
        distances[i] = UltrasonicWave_GetDistance();
        Delay_ms(10);
    }
    
    // 去掉最大值和最小值
    float max = distances[0], min = distances[0];
    for(uint8_t i = 1; i < 5; i++) {
        if(distances[i] > max) max = distances[i];
        if(distances[i] < min) min = distances[i];
    }
    
    // 计算平均值
    for(uint8_t i = 0; i < 5; i++) {
        sum += distances[i];
    }
    sum = sum - max - min;
    
    return sum / 3.0;
}
```

#### 3.3.2 红外传感器

```c
// 红外传感器读取
typedef struct {
    uint8_t front;   // 前方
    uint8_t back;    // 后方
    uint8_t left;    // 左侧
    uint8_t right;   // 右侧
} InfraredData;

InfraredData Infrared_Read(void) {
    InfraredData data;
    
    data.front = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
    data.back = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    data.left = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
    data.right = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
    
    return data;
}

// 边缘检测
uint8_t Infrared_DetectEdge(void) {
    InfraredData data = Infrared_Read();
    
    // 任一传感器检测到边缘(高电平)则返回1
    if(data.front == 1 || data.back == 1 || 
       data.left == 1 || data.right == 1) {
        return 1;
    }
    
    return 0;
}
```

### 3.4 避障算法

#### 3.4.1 多传感器融合

```c
// 避障状态枚举
typedef enum {
    AVOID_NONE = 0,      // 无障碍
    AVOID_WARNING,       // 警告 (超声波)
    AVOID_DANGER         // 危险 (红外)
} AvoidState;

// 避障决策
AvoidState ObstacleAvoidance_Decide(void) {
    // 一级警报: 超声波
    float distance = UltrasonicWave_GetDistance_Filtered();
    if(distance < 20.0 && distance > 2.0) {
        return AVOID_WARNING;
    }
    
    // 二级警报: 红外
    if(Infrared_DetectEdge()) {
        return AVOID_DANGER;
    }
    
    return AVOID_NONE;
}

// 避障行为
void ObstacleAvoidance_Action(AvoidState state) {
    switch(state) {
        case AVOID_WARNING:
            // 减速 + 转向
            Movement_Stop();
            Delay_ms(500);
            Movement_TurnLeft();
            Delay_ms(1000);
            break;
            
        case AVOID_DANGER:
            // 立即停止 + 后退
            Movement_Stop();
            Delay_ms(200);
            Movement_Backward();
            Delay_ms(800);
            Movement_Stop();
            break;
            
        default:
            break;
    }
}
```

### 3.5 通信协议

#### 3.5.1 蓝牙通信

```c
// 蓝牙接收中断
void USART1_IRQHandler(void) {
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1);
        
        // 指令解析
        switch(data) {
            case 'F': move_mode = MODE_FORWARD; break;
            case 'B': move_mode = MODE_BACKWARD; break;
            case 'L': move_mode = MODE_TURN_LEFT; break;
            case 'R': move_mode = MODE_TURN_RIGHT; break;
            case 'S': move_mode = MODE_STOP; break;
            case '1': move_mode = MODE_STAND; break;
            case '2': move_mode = MODE_SLEEP; break;
            case '3': move_mode = MODE_SIT; break;
            case 'o': avoid_enable = 1; break;
            case 'c': avoid_enable = 0; break;
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
```

#### 3.5.2 语音识别

```c
// 语音识别中断
void USART3_IRQHandler(void) {
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART3);
        
        // 只有在蓝牙无指令时才响应语音
        if(move_mode == MODE_IDLE) {
            switch(data) {
                case 0x01: move_mode = MODE_FORWARD; break;
                case 0x02: move_mode = MODE_BACKWARD; break;
                case 0x03: move_mode = MODE_TURN_LEFT; break;
                case 0x04: move_mode = MODE_TURN_RIGHT; break;
                case 0x05: move_mode = MODE_STOP; break;
                case 0x06: move_mode = MODE_STAND; break;
                case 0x07: move_mode = MODE_SIT; break;
                case 0x08: move_mode = MODE_SLEEP; break;
            }
        }
        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}
```

#### 3.5.3 语音合成

```c
// SYN6288数据帧格式
// [0xFD][长度高][长度低][0x01][编码][文本][校验和]

void SYN6288_Speak(char *text) {
    uint16_t length = strlen(text) + 2;  // +2: 命令字节和编码字节
    uint8_t checksum = 0;
    
    // 发送帧头
    USART_SendData(USART2, 0xFD);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    // 发送长度
    USART_SendData(USART2, (length >> 8) & 0xFF);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, length & 0xFF);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    // 发送命令字节
    USART_SendData(USART2, 0x01);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    checksum ^= 0x01;
    
    // 发送编码格式 (GB2312)
    USART_SendData(USART2, 0x00);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    // 发送文本
    for(uint16_t i = 0; i < strlen(text); i++) {
        USART_SendData(USART2, text[i]);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        checksum ^= text[i];
    }
    
    // 发送校验和
    USART_SendData(USART2, checksum);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
```

### 3.6 OLED显示

#### 3.6.1 I2C驱动

```c
// OLED初始化
void OLED_Init(void) {
    // I2C初始化
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;  // 400KHz
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    
    // OLED初始化命令序列
    OLED_WriteCommand(0xAE);  // 关闭显示
    OLED_WriteCommand(0xD5);  // 设置时钟分频
    OLED_WriteCommand(0x80);
    // ... 更多初始化命令
    OLED_WriteCommand(0xAF);  // 开启显示
}
```

#### 3.6.2 图像显示

```c
// 显示图像
void OLED_ShowImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bmp) {
    uint8_t page_start = y / 8;
    uint8_t page_end = (y + height - 1) / 8;
    
    for(uint8_t page = page_start; page <= page_end; page++) {
        OLED_SetPosition(x, page);
        for(uint8_t col = 0; col < width; col++) {
            uint16_t index = (page - page_start) * width + col;
            OLED_WriteData(bmp[index]);
        }
    }
}

// 表情数据 (取模生成)
const uint8_t BMP_Happy[128] = {
    // 笑脸表情的像素数据
    0x00, 0x00, 0xE0, 0xF8, 0x1C, 0x0E, 0x06, 0x03,
    // ...
};

const uint8_t BMP_Sleep[128] = {
    // 睡觉表情的像素数据
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
    // ...
};
```

### 3.7 状态管理

#### 3.7.1 状态定义

```c
// 状态结构体
typedef struct {
    uint16_t stamina;     // 体力值 (0-500)
    uint16_t happiness;   // 开心值 (0-200)
    uint8_t mode;         // 当前模式
    uint8_t avoid_enable; // 避障使能
} RobotState;

RobotState robot_state = {
    .stamina = 500,
    .happiness = 200,
    .mode = MODE_IDLE,
    .avoid_enable = 0
};
```

#### 3.7.2 体力值管理

```c
// 消耗体力
void State_ConsumeStamina(uint16_t amount) {
    if(robot_state.stamina > amount) {
        robot_state.stamina -= amount;
    } else {
        robot_state.stamina = 0;
    }
}

// 恢复体力
void State_RecoverStamina(uint16_t amount) {
    if(robot_state.stamina + amount < 500) {
        robot_state.stamina += amount;
    } else {
        robot_state.stamina = 500;
    }
}

// 检查体力
uint8_t State_CheckStamina(void) {
    return (robot_state.stamina > 0) ? 1 : 0;
}
```

## 4. 主程序流程

### 4.1 初始化流程

```c
int main(void) {
    // 1. 系统初始化
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // 2. 外设初始化
    Delay_Init();
    LED_Init();
    PWM_Init();
    Servo_Init();
    UltrasonicWave_Init();
    Infrared_Init();
    USART1_Init(9600);   // 蓝牙
    USART2_Init(9600);   // 语音合成
    USART3_Init(9600);   // 语音识别
    OLED_Init();
    
    // 3. 初始姿态
    Posture_Stand();
    
    // 4. 欢迎语音
    SYN6288_Speak("你好,我是小呆");
    
    // 5. 显示初始表情
    OLED_ShowImage(0, 0, 128, 64, BMP_Happy);
    
    // 6. 进入主循环
    while(1) {
        MainLoop();
    }
}
```

### 4.2 主循环

```c
void MainLoop(void) {
    // 1. 检查体力值
    if(!State_CheckStamina()) {
        // 体力耗尽,进入疲劳模式
        Posture_Lie();
        OLED_ShowImage(0, 0, 128, 64, BMP_Tired);
        SYN6288_Speak("我好累啊,让我休息一下");
        
        // 只响应睡觉指令
        while(!State_CheckStamina()) {
            if(move_mode == MODE_SLEEP) {
                State_RecoverStamina(10);
                Delay_ms(1000);
            }
        }
        
        return;
    }
    
    // 2. 避障检测
    if(robot_state.avoid_enable) {
        AvoidState avoid_state = ObstacleAvoidance_Decide();
        if(avoid_state != AVOID_NONE) {
            ObstacleAvoidance_Action(avoid_state);
            return;
        }
    }
    
    // 3. 指令执行
    switch(move_mode) {
        case MODE_FORWARD:
            Movement_Forward();
            State_ConsumeStamina(1);
            OLED_ShowImage(0, 0, 128, 64, BMP_Happy);
            break;
            
        case MODE_BACKWARD:
            Movement_Backward();
            State_ConsumeStamina(1);
            break;
            
        case MODE_TURN_LEFT:
            Movement_TurnLeft();
            State_ConsumeStamina(1);
            break;
            
        case MODE_TURN_RIGHT:
            Movement_TurnRight();
            State_ConsumeStamina(1);
            break;
            
        case MODE_STOP:
            Movement_Stop();
            break;
            
        case MODE_STAND:
            Posture_Stand();
            OLED_ShowImage(0, 0, 128, 64, BMP_Stand);
            break;
            
        case MODE_SIT:
            Posture_Sit();
            OLED_ShowImage(0, 0, 128, 64, BMP_Sit);
            break;
            
        case MODE_SLEEP:
            Posture_Lie();
            OLED_ShowImage(0, 0, 128, 64, BMP_Sleep);
            State_RecoverStamina(5);
            break;
            
        default:
            break;
    }
    
    // 4. LED闪烁 (心跳)
    LED_Toggle();
    
    // 5. 延时
    Delay_ms(50);
}
```

## 5. 调试与优化

### 5.1 调试方法

**串口调试**:
```c
// 打印调试信息
void Debug_Printf(char *format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    USART_SendString(USART1, buffer);
}

// 使用示例
Debug_Printf("Distance: %.2f cm\r\n", distance);
Debug_Printf("Stamina: %d\r\n", robot_state.stamina);
```

**LED指示**:
```c
// 错误指示
void Error_Handler(uint8_t error_code) {
    while(1) {
        for(uint8_t i = 0; i < error_code; i++) {
            LED_On();
            Delay_ms(200);
            LED_Off();
            Delay_ms(200);
        }
        Delay_ms(1000);
    }
}
```

### 5.2 性能优化

**1. 减少延时**:
- 使用定时器中断代替Delay
- 采用非阻塞式编程

**2. 优化算法**:
- 减少浮点运算
- 使用查找表代替复杂计算

**3. 内存优化**:
- 合理使用全局变量和局部变量
- 避免大数组占用SRAM

**4. 功耗优化**:
- 空闲时进入低功耗模式
- 关闭不用的外设时钟

## 6. 常见问题

### 6.1 编译错误

**问题**: 找不到头文件
**解决**: 检查Include Paths设置

**问题**: 链接错误
**解决**: 检查库文件是否添加

### 6.2 运行问题

**问题**: 程序不运行
**解决**: 检查启动文件和中断向量表

**问题**: 串口乱码
**解决**: 检查波特率设置

**问题**: 舵机不动
**解决**: 检查PWM配置和电源

## 7. 扩展功能

### 7.1 添加新动作

1. 在Movement.c中定义新的步态序列
2. 在Mode.h中添加新的模式枚举
3. 在主循环switch中添加case分支

### 7.2 添加新传感器

1. 在Hardware目录创建驱动文件
2. 实现初始化和读取函数
3. 在主程序中调用

### 7.3 添加新指令

1. 在通信协议中定义新指令码
2. 在中断服务程序中添加解析
3. 在主循环中添加执行逻辑

---

**文档版本**: v1.0  
**更新日期**: 2025-10-24  
**作者**: Manbo Robot Dog Team

