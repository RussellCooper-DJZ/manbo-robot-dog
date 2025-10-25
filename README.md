# 曼波机器狗 (Manbo Robot Dog)

基于STM32F103C8T6的智能四足机器狗项目,具备运动控制、自主避障、语音交互和表情显示等功能。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F103-green.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)

## 📋 项目简介

"小呆"是一款桌面级智能四足机器狗,集成了多种传感器和交互模块,能够实现:

- 🚶 **灵活运动**: 前进、后退、左转、右转等基本步态
- 🎭 **多种姿态**: 站立、坐下、趴下等静态姿势
- 🛡️ **自主避障**: 超声波+红外传感器融合的避障系统
- 🗣️ **语音交互**: 支持语音识别控制和语音合成反馈
- 📱 **蓝牙控制**: 通过手机APP进行无线遥控
- 😊 **表情显示**: OLED屏幕显示不同的眼部表情
- 💪 **状态管理**: 体力值和开心值系统,增加交互趣味性

## 🔧 硬件配置

### 核心控制器
- **主控**: STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **内存**: 64KB Flash + 20KB SRAM

### 执行机构
- **舵机**: 8个SG90微型舵机 (4条腿 × 2关节)
- **驱动**: PWM信号直接驱动

### 传感器系统
- **超声波**: HC-SR04 (障碍物检测, 2-400cm)
- **红外**: 4个红外避障传感器 (边缘检测, 防跌落)

### 通信模块
- **蓝牙**: HC-05/HC-06 蓝牙串口模块
- **语音识别**: SU-03T 离线语音识别模块
- **语音合成**: SYN6288 中文语音合成芯片

### 显示与反馈
- **显示屏**: 0.96寸OLED (128×64, I2C接口)
- **指示灯**: LED状态指示灯

### 电源系统
- **电池**: 2节18650锂电池串联 (7.4V)
- **稳压**: 5V (舵机) + 3.3V (主控及逻辑电路)

## 📁 项目结构

```
manbo-robot-dog/
├── Hardware/           # 硬件驱动层
│   ├── Hongwai.c/h    # 红外传感器驱动
│   ├── LED.c/h        # LED控制
│   ├── Mode.c/h       # 行为模式定义
│   ├── Movement.c/h   # 运动控制算法
│   ├── OLED.c/h       # OLED显示驱动
│   ├── OLED_Data.c/h  # OLED图像数据
│   ├── PWM.c/h        # PWM信号生成
│   ├── Serial.c/h     # 串口通信
│   ├── Servo.c/h      # 舵机控制
│   ├── syn6288.c/h    # 语音合成模块
│   ├── Timer.c/h      # 定时器配置
│   └── UltrasonicWave.c/h  # 超声波测距
├── System/            # 系统层
│   ├── Delay.c/h      # 延时函数
│   ├── usart1.c/h     # 串口1 (蓝牙)
│   ├── usart2.c/h     # 串口2 (语音合成)
│   └── usart3.c/h     # 串口3 (语音识别)
├── User/              # 应用层
│   ├── main.c         # 主程序
│   ├── Mode.c/h       # 模式管理
│   ├── stm32f10x_it.c/h  # 中断服务程序
│   └── stm32f10x_conf.h  # 配置文件
├── Library/           # STM32标准外设库
├── Start/             # 启动文件
├── Project.uvprojx    # Keil工程文件
├── README.md          # 项目说明
├── LICENSE            # 开源协议
└── docs/              # 文档目录
    ├── hardware.md    # 硬件设计文档
    ├── software.md    # 软件设计文档
    └── user_manual.md # 使用手册
```

## 🚀 快速开始

### 开发环境
- **IDE**: Keil MDK 5.x
- **编译器**: ARM Compiler
- **调试器**: ST-Link V2 或 J-Link
- **烧录工具**: ST-Link Utility / Keil MDK

### 编译步骤

1. **克隆仓库**
   ```bash
   git clone https://github.com/yourusername/manbo-robot-dog.git
   cd manbo-robot-dog
   ```

2. **打开工程**
   - 使用Keil MDK打开 `Project.uvprojx`

3. **配置目标**
   - 选择目标芯片: STM32F103C8
   - 配置调试器: ST-Link

4. **编译**
   - 点击 `Build` 或按 `F7` 编译工程

5. **烧录**
   - 连接ST-Link到目标板
   - 点击 `Download` 或按 `F8` 烧录程序

### 硬件连接

#### 舵机连接 (PWM)
| 舵机 | STM32引脚 | 功能 |
|------|-----------|------|
| 舵机1 | PA0 | 左前腿-髋关节 |
| 舵机2 | PA1 | 左前腿-膝关节 |
| 舵机3 | PA2 | 右前腿-髋关节 |
| 舵机4 | PA3 | 右前腿-膝关节 |
| 舵机5 | PA6 | 左后腿-髋关节 |
| 舵机6 | PA7 | 左后腿-膝关节 |
| 舵机7 | PB0 | 右后腿-髋关节 |
| 舵机8 | PB1 | 右后腿-膝关节 |

#### 传感器连接
| 模块 | STM32引脚 | 功能 |
|------|-----------|------|
| 超声波Trig | PB12 | 触发信号 |
| 超声波Echo | PB13 | 回波信号 |
| 红外1 | PB2 | 前方边缘检测 |
| 红外2 | PB3 | 后方边缘检测 |
| 红外3 | PB4 | 左侧边缘检测 |
| 红外4 | PB5 | 右侧边缘检测 |

#### 通信模块连接
| 模块 | STM32引脚 | 功能 |
|------|-----------|------|
| 蓝牙TX | PA9 (USART1_TX) | 发送 |
| 蓝牙RX | PA10 (USART1_RX) | 接收 |
| 语音合成TX | PA2 (USART2_TX) | 发送 |
| 语音合成RX | PA3 (USART2_RX) | 接收 |
| 语音识别TX | PB10 (USART3_TX) | 发送 |
| 语音识别RX | PB11 (USART3_RX) | 接收 |

#### 显示模块连接
| 模块 | STM32引脚 | 功能 |
|------|-----------|------|
| OLED_SCL | PB6 (I2C1_SCL) | I2C时钟 |
| OLED_SDA | PB7 (I2C1_SDA) | I2C数据 |
| LED | PC13 | 状态指示 |

## 🎮 使用说明

### 蓝牙控制指令

通过手机APP发送以下字符指令:

| 指令 | 功能 |
|------|------|
| `F` | 前进 |
| `B` | 后退 |
| `L` | 左转 |
| `R` | 右转 |
| `S` | 停止 |
| `1` | 站立 |
| `2` | 睡觉(卧) |
| `3` | 坐下 |
| `p` | 趴下 |
| `o` | 开启避障模式 |
| `c` | 关闭避障模式 |

### 语音控制指令

支持以下中文语音指令:

- "前进" - 机器狗向前行走
- "后退" - 机器狗向后行走
- "左转" - 机器狗向左转向
- "右转" - 机器狗向右转向
- "停止" - 机器狗停止运动
- "站立" - 机器狗站立姿态
- "坐下" - 机器狗坐下姿态
- "睡觉" - 机器狗趴下休息
- "旺旺" - 机器狗发出叫声

### 状态管理

#### 体力值系统
- **初始值**: 500
- **消耗**: 每次运动消耗体力
- **恢复**: 执行"睡觉"动作恢复体力
- **效果**: 体力耗尽时拒绝执行运动指令,只能休息

#### 开心值系统
- **初始值**: 200
- **预留**: 为未来功能扩展预留接口

### OLED表情

机器狗会根据当前状态显示不同表情:

- 😊 **立正**: 正常工作状态
- 😴 **睡觉**: 休息状态
- 😵 **难受**: 体力耗尽状态
- ❤️ **爱心**: 开心状态

## 🔬 核心技术

### 1. 舵机协调控制

采用**基于时间分割的线性插值算法**,实现多舵机的平滑协调运动:

```c
// 伪代码示例
void SmoothMove(Servo servo, int targetAngle, int duration) {
    int currentAngle = servo.angle;
    int steps = duration / 20;  // 20ms刷新周期
    int stepAngle = (targetAngle - currentAngle) / steps;
    
    for (int i = 0; i < steps; i++) {
        currentAngle += stepAngle;
        SetServoAngle(servo, currentAngle);
        Delay(20);
    }
}
```

### 2. 多传感器融合避障

采用**基于行为分级的决策融合策略**:

- **一级警报**: 超声波检测到前方障碍物 (< 20cm) → 减速+转向
- **二级警报**: 红外检测到边缘 → 立即停止+后退

### 3. 对角步态规划

采用**对角步态(Trot)**实现稳定行走:

1. 抬起左前腿+右后腿 → 前摆
2. 落下左前腿+右后腿
3. 抬起右前腿+左后腿 → 前摆
4. 落下右前腿+左后腿

### 4. 状态机设计

使用**有限状态机(FSM)**管理行为模式:

```
[正常] --体力耗尽--> [疲劳] --睡觉--> [恢复] --体力满--> [正常]
```

## 📊 性能指标

| 指标 | 数值 |
|------|------|
| 指令响应延迟 | ~250ms |
| 运动精度 | ±3% |
| 避障成功率 | 90% |
| 语音识别准确率 | 92% (安静环境) |
| 续航时间 | ~75分钟 (中等强度) |
| 工作电压 | 7.4V |
| 工作电流 | 0.5-2A (动态) |

## 🛠️ 常见问题

### Q1: 舵机抖动或无法转动?
**A**: 检查电源供电是否充足,舵机需要较大电流。建议使用独立的5V/3A电源为舵机供电。

### Q2: 蓝牙无法连接?
**A**: 
1. 检查HC-05/HC-06模块的波特率设置(默认9600)
2. 确认TX/RX引脚是否交叉连接
3. 检查手机蓝牙是否已配对

### Q3: 语音识别不灵敏?
**A**: 
1. 确保在安静环境下使用
2. 发音要清晰标准
3. 麦克风距离保持在20-50cm

### Q4: OLED屏幕不显示?
**A**: 
1. 检查I2C地址是否正确(通常为0x78或0x3C)
2. 确认SCL/SDA引脚连接正确
3. 检查电源是否正常

### Q5: 机器狗运动不稳定?
**A**: 
1. 校准舵机中位角度
2. 调整步态参数
3. 检查机械结构是否松动

## 📖 相关文档

- [硬件设计文档](docs/hardware.md)
- [软件设计文档](docs/software.md)
- [毕业设计论文](docs/README_THESIS.md) 🔐 (加密保护)

## 🤝 贡献

欢迎提交Issue和Pull Request!

### 贡献指南

1. Fork本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 提交Pull Request

## 📄 开源协议

本项目采用 [MIT License](LICENSE) 开源协议。

## 👥 作者

- **项目作者**: [russellCooper]

## 🙏 致谢

感谢以下开源项目和资源:

- [STM32 Standard Peripheral Library](https://www.st.com/en/embedded-software/stm32-standard-peripheral-libraries.html)
- [OLED驱动库](https://github.com/afiskon/stm32-ssd1306)
- 各位在开源社区分享知识的技术大佬

## 📮 联系方式

如有问题或建议,欢迎通过以下方式联系:

- **Email**: [dwjmrz@qq.com]
- **GitHub Issues**: [提交Issue](https://github.com/yourusername/manbo-robot-dog/issues)

## 🌟 Star History

如果这个项目对你有帮助,请给个Star⭐支持一下!

---

**注意**: 本项目仅供学习和研究使用,请勿用于商业用途。

