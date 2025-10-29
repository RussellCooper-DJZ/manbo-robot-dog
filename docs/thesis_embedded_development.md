# 基于STM32的四足机器狗嵌入式系统设计与开发

**摘要**: 本文从嵌入式系统开发的角度,详细阐述了四足机器狗控制系统的设计与实现过程。系统以STM32F103C8T6微控制器为核心,采用模块化分层架构,实现了PWM舵机驱动、多串口通信、I2C显示控制、定时器中断管理等功能。本文重点介绍了STM32外设配置方法、驱动程序开发技术、实时操作系统设计、调试优化策略等内容,并通过实际项目验证了系统的可行性和可靠性。研究成果可为嵌入式系统开发者提供完整的工程实践参考。

**关键词**: STM32; 嵌入式系统; 驱动开发; 实时控制; 四足机器人

---

## 第一章 绪论

### 1.1 嵌入式系统概述

#### 1.1.1 嵌入式系统定义

嵌入式系统(Embedded System)是指嵌入在其他设备中,专门用于完成特定功能的计算机系统。与通用计算机系统不同,嵌入式系统具有以下特点:

**专用性**。嵌入式系统通常只完成特定的功能,不需要像通用计算机那样支持各种应用程序。这使得嵌入式系统可以针对特定应用进行优化,提高性能和效率。

**实时性**。许多嵌入式系统需要在规定的时间内对外部事件做出响应,具有严格的实时性要求。例如,汽车的防抱死制动系统(ABS)必须在毫秒级时间内响应。

**资源受限**。嵌入式系统通常在处理器性能、存储容量、功耗等方面受到限制,需要在有限资源下实现功能。

**可靠性高**。嵌入式系统往往应用于关键场合,对可靠性和稳定性要求很高,需要能够长时间无故障运行。

**成本敏感**。嵌入式系统通常需要大批量生产,对成本控制要求严格,需要在性能和成本之间找到平衡。

#### 1.1.2 嵌入式系统组成

典型的嵌入式系统由以下部分组成:

**硬件系统**:
- 处理器: 如ARM、MIPS、RISC-V等
- 存储器: Flash、SRAM、DRAM等
- 外设: GPIO、定时器、串口、SPI、I2C、ADC、DAC等
- 电源管理: 稳压器、电池管理等

**软件系统**:
- 引导程序(Bootloader): 负责系统启动和程序加载
- 操作系统: 裸机程序、RTOS或嵌入式Linux
- 驱动程序: 硬件抽象层和设备驱动
- 应用程序: 实现具体功能的业务逻辑

**开发工具**:
- 集成开发环境(IDE): Keil、IAR、Eclipse等
- 编译器: GCC、ARMCC等
- 调试器: JTAG、SWD等
- 仿真器: Proteus、Keil Simulator等

#### 1.1.3 STM32微控制器简介

STM32是意法半导体(STMicroelectronics)公司推出的基于ARM Cortex-M内核的32位微控制器系列。STM32系列产品线丰富,覆盖了从低功耗到高性能的各种应用场景。

**STM32系列分类**:
- STM32F0: Cortex-M0内核,入门级
- STM32F1: Cortex-M3内核,主流级
- STM32F2: Cortex-M3内核,高性能
- STM32F3: Cortex-M4内核,混合信号
- STM32F4: Cortex-M4内核,高性能
- STM32F7: Cortex-M7内核,超高性能
- STM32L系列: 低功耗系列
- STM32H系列: 高性能系列

本项目选用的STM32F103C8T6属于STM32F1系列,主要特性如下:
- 内核: ARM Cortex-M3, 72MHz
- Flash: 64KB
- SRAM: 20KB
- GPIO: 37个
- 定时器: 3个通用定时器,1个高级定时器
- 串口: 3个USART
- SPI: 2个
- I2C: 2个
- ADC: 2个12位ADC
- 封装: LQFP48
- 价格: 约10元

STM32F103C8T6性价比高,资源丰富,适合作为学习和开发平台。

### 1.2 项目背景与需求

#### 1.2.1 项目背景

四足机器人作为一种重要的移动机器人平台,在教育、科研、娱乐等领域有广泛应用。然而,现有的四足机器人系统多采用高性能处理器(如树莓派、Jetson Nano)或复杂的控制器(如Arduino Mega),成本较高,不利于推广。

本项目旨在开发一款基于STM32的低成本四足机器狗系统,为嵌入式系统学习者提供一个完整的工程实践案例。通过本项目,可以学习到:

- STM32外设配置和使用方法
- 驱动程序开发技术
- 实时控制系统设计
- 多任务调度策略
- 通信协议设计与实现
- 调试和优化技巧

#### 1.2.2 功能需求

系统需要实现以下功能:

**运动控制**:
- 控制4个舵机实现前进、后退、左转、右转等运动
- 支持多种运动模式(站立、坐下、趴下、打招呼等)
- 运动过程平滑,无明显抖动

**传感器采集**:
- 超声波测距,检测前方障碍物
- 红外传感器检测地面边缘
- 实时采集传感器数据,更新频率不低于10Hz

**通信功能**:
- 蓝牙通信,接收遥控指令
- 语音识别,支持语音控制
- 语音合成,提供语音反馈

**显示功能**:
- OLED显示屏显示状态信息
- 显示当前模式、电量、距离等信息
- 显示表情图案

**避障功能**:
- 自主检测障碍物和边缘
- 自动执行避障动作
- 可通过指令开启/关闭避障功能

#### 1.2.3 性能需求

系统需要满足以下性能要求:

**实时性**:
- PWM信号频率: 50Hz,精度±1%
- 传感器采集周期: ≤100ms
- 指令响应时间: ≤300ms

**可靠性**:
- 连续运行时间: ≥5小时
- 故障率: ≤1%
- 抗干扰能力: 能在一般电磁环境下正常工作

**功耗**:
- 待机功耗: ≤100mA
- 运动功耗: ≤2A
- 续航时间: ≥60分钟

**成本**:
- 硬件成本: ≤200元
- 开发成本: 可接受的开发周期和人力投入

### 1.3 开发环境与工具

#### 1.3.1 硬件开发环境

**开发板**: STM32F103C8T6最小系统板
- 板载8MHz外部晶振
- 板载LED指示灯
- 引出所有GPIO引脚
- 支持SWD调试接口

**调试器**: ST-Link V2
- 支持SWD和JTAG接口
- 支持在线调试和程序下载
- 支持虚拟串口功能

**测试设备**:
- 数字示波器: 用于测量PWM波形
- 万用表: 用于测量电压、电流
- 逻辑分析仪: 用于分析数字信号

#### 1.3.2 软件开发环境

**集成开发环境**: Keil MDK 5.x
- 版本: 5.36或更高
- 支持STM32F1系列
- 集成编译器、调试器、仿真器

**编译器**: ARM Compiler v5/v6
- 优化等级: -O2
- 支持C99标准
- 生成高效的ARM代码

**固件库**: STM32F10x标准外设库 v3.5.0
- 提供完整的外设驱动
- 包含丰富的示例代码
- 文档齐全,易于使用

**调试工具**:
- Keil Debugger: 在线调试
- Serial Wire Viewer: 实时数据查看
- Logic Analyzer: 逻辑分析

**版本控制**: Git
- 代码托管: GitHub
- 分支管理: Git Flow
- 协作开发: Pull Request

#### 1.3.3 开发流程

本项目采用以下开发流程:

**需求分析** → **系统设计** → **模块开发** → **单元测试** → **集成测试** → **系统测试** → **优化调试** → **文档整理**

具体步骤:

1. **需求分析**: 明确功能需求和性能需求
2. **系统设计**: 设计硬件架构和软件架构
3. **模块开发**: 按模块逐个开发驱动和功能
4. **单元测试**: 对每个模块进行独立测试
5. **集成测试**: 将各模块集成,测试接口
6. **系统测试**: 测试整体功能和性能
7. **优化调试**: 优化代码,提高性能
8. **文档整理**: 编写技术文档和用户手册

### 1.4 论文组织结构

本文共分为七章:

**第一章**为绪论,介绍嵌入式系统概述、项目背景与需求、开发环境与工具。

**第二章**为STM32硬件系统设计,介绍STM32最小系统、外设接口电路、电源管理等内容。

**第三章**为STM32软件架构设计,介绍软件分层架构、模块划分、开发规范等内容。

**第四章**为外设驱动程序开发,详细阐述GPIO、定时器、串口、I2C等外设的配置和驱动开发。

**第五章**为应用程序开发,介绍主程序设计、任务调度、状态机设计、通信协议等内容。

**第六章**为系统调试与优化,介绍调试方法、性能优化、功耗优化等技术。

**第七章**为总结与展望,总结开发经验,展望未来改进方向。

---

## 第二章 STM32硬件系统设计

### 2.1 STM32最小系统

#### 2.1.1 电源电路

STM32F103C8T6的电源引脚包括:
- VDD/VSS: 数字电源,3.3V
- VDDA/VSSA: 模拟电源,3.3V
- VBAT: 备份电源,用于RTC和备份寄存器

**电源设计要点**:

1. **去耦电容**: 在每个VDD引脚附近放置100nF陶瓷电容,在电源输入端放置10μF电解电容,用于滤除电源噪声。

2. **模拟电源滤波**: VDDA引脚通过10μF电容和磁珠与VDD连接,减少数字电路对模拟电路的干扰。

3. **备份电源**: VBAT引脚可以连接纽扣电池(CR2032),在主电源断电时保持RTC运行。

4. **电源指示**: 在VDD引脚连接LED和限流电阻,指示电源状态。

电路原理图:

```
3.3V ──┬── 10μF ──┬── 100nF ── VDD1
       │          ├── 100nF ── VDD2
       │          ├── 100nF ── VDD3
       │          └── 100nF ── VDD4
       │
       ├── 磁珠 ── 10μF ── 100nF ── VDDA
       │
       └── LED + 1kΩ ── GND
```

#### 2.1.2 时钟电路

STM32F103C8T6支持多种时钟源:
- 内部高速时钟(HSI): 8MHz,精度较低
- 外部高速时钟(HSE): 4-16MHz,精度高
- 内部低速时钟(LSI): 40kHz,用于看门狗
- 外部低速时钟(LSE): 32.768kHz,用于RTC

**时钟设计要点**:

1. **外部晶振**: 选用8MHz无源晶振,精度±20ppm。

2. **负载电容**: 根据晶振参数选择负载电容,通常为15pF~30pF。计算公式:
   ```
   C1 = C2 = 2 × (CL - Cstray)
   ```
   其中,CL为晶振负载电容,Cstray为PCB寄生电容(约3pF~5pF)。

3. **布局布线**: 晶振应尽量靠近STM32,走线尽量短,避免干扰。

4. **RTC晶振**: 如需使用RTC,可选配32.768kHz晶振。

电路原理图:

```
OSC_IN ── 8MHz晶振 ── OSC_OUT
   │                    │
  20pF                 20pF
   │                    │
  GND                  GND
```

#### 2.1.3 复位电路

STM32的复位引脚NRST为低电平有效。复位电路设计:

**上电复位**: 通过RC电路实现上电复位。

**手动复位**: 通过按键实现手动复位。

**看门狗复位**: 通过内部看门狗实现自动复位。

电路原理图:

```
3.3V ──┬── 10kΩ ── NRST
       │
       └── 按键 ── GND
       
NRST ── 100nF ── GND
```

上电复位时间:
```
T = R × C = 10kΩ × 100nF = 1ms
```

#### 2.1.4 调试接口

STM32支持SWD和JTAG两种调试接口。SWD接口只需要2根信号线(SWDIO和SWCLK),节省引脚资源。

**SWD接口引脚**:
- SWDIO: PA13
- SWCLK: PA14
- NRST: 复位引脚(可选)
- GND: 地线
- 3.3V: 电源(可选)

**接口设计要点**:

1. **引脚保护**: 在SWDIO和SWCLK引脚串联100Ω电阻,防止静电损坏。

2. **下拉电阻**: 在SWDIO引脚接10kΩ下拉电阻,防止悬空。

3. **接口类型**: 使用2.54mm排针,方便连接调试器。

电路原理图:

```
PA13(SWDIO) ── 100Ω ── SWDIO ── 10kΩ ── GND
PA14(SWCLK) ── 100Ω ── SWCLK
NRST ─────────────── NRST
```

#### 2.1.5 启动模式配置

STM32通过BOOT0和BOOT1引脚配置启动模式:

| BOOT0 | BOOT1 | 启动模式 |
|-------|-------|---------|
| 0 | X | 从Flash启动(正常模式) |
| 1 | 0 | 从系统存储器启动(ISP模式) |
| 1 | 1 | 从SRAM启动(调试模式) |

**配置方法**:

1. **正常模式**: BOOT0接GND,BOOT1任意。

2. **ISP模式**: BOOT0通过跳线帽或拨码开关选择GND或3.3V。

电路原理图:

```
BOOT0 ── 10kΩ ── GND  (或通过跳线帽选择3.3V)
BOOT1 ── 10kΩ ── GND
```

### 2.2 外设接口电路设计

#### 2.2.1 PWM输出接口

PWM信号用于控制舵机。STM32的定时器可以输出PWM信号。

**引脚分配**:
- TIM3_CH1: PA6 → 舵机1
- TIM3_CH2: PA7 → 舵机2
- TIM3_CH3: PB0 → 舵机3
- TIM3_CH4: PB1 → 舵机4

**接口电路**:

由于STM32的GPIO输出电流有限(最大25mA),而舵机控制信号电流很小(约1mA),可以直接连接。

```
PA6 ── 舵机1信号线
PA7 ── 舵机2信号线
PB0 ── 舵机3信号线
PB1 ── 舵机4信号线
```

**注意事项**:

1. **电源隔离**: 舵机电源(5V)与STM32电源(3.3V)应分开供电,共地。

2. **信号电平**: 舵机可以识别3.3V的PWM信号,无需电平转换。

3. **EMI抑制**: 在舵机电源线上串联磁珠,并联电容,抑制电磁干扰。

#### 2.2.2 串口通信接口

系统使用3个串口:
- USART1: 蓝牙通信
- USART2: 语音合成
- USART3: 语音识别

**USART1接口(蓝牙)**:
- TX: PA9
- RX: PA10

连接HC-05蓝牙模块:

```
PA9(TX) ── RX(HC-05)
PA10(RX) ── TX(HC-05)
GND ── GND
5V ── VCC
```

**USART2接口(语音合成)**:
- TX: PA2
- RX: PA3

连接SYN6288语音合成模块:

```
PA2(TX) ── RX(SYN6288)
PA3(RX) ── TX(SYN6288)
GND ── GND
5V ── VCC
```

**USART3接口(语音识别)**:
- TX: PB10
- RX: PB11

连接LD3320语音识别模块:

```
PB10(TX) ── RX(LD3320)
PB11(RX) ── TX(LD3320)
GND ── GND
3.3V ── VCC
```

**注意事项**:

1. **波特率**: 蓝牙模块默认波特率9600,语音模块默认波特率9600。

2. **电平匹配**: HC-05和SYN6288为5V模块,但可以识别3.3V信号。LD3320为3.3V模块,不能接5V信号。

3. **交叉连接**: STM32的TX连接模块的RX,STM32的RX连接模块的TX。

#### 2.2.3 I2C接口

I2C用于连接OLED显示屏。

**引脚分配**:
- SCL: PB6
- SDA: PB7

连接0.96寸OLED(SSD1306):

```
PB6(SCL) ── SCL(OLED)
PB7(SDA) ── SDA(OLED)
GND ── GND
3.3V ── VCC
```

**上拉电阻**:

I2C总线需要上拉电阻。OLED模块通常已经集成上拉电阻(4.7kΩ),无需外接。如果模块没有上拉电阻,需要在SCL和SDA引脚接4.7kΩ上拉电阻到3.3V。

```
SCL ── 4.7kΩ ── 3.3V
SDA ── 4.7kΩ ── 3.3V
```

#### 2.2.4 超声波接口

超声波传感器HC-SR04使用GPIO控制。

**引脚分配**:
- Trig: PB12 (输出)
- Echo: PB13 (输入)

连接HC-SR04:

```
PB12(Trig) ── Trig(HC-SR04)
PB13(Echo) ── Echo(HC-SR04)
GND ── GND
5V ── VCC
```

**电平转换**:

HC-SR04的Echo输出为5V,超过STM32的输入电压(3.6V)。需要进行电平转换。

方法1: 电阻分压

```
Echo(5V) ── 2kΩ ── PB13 ── 3kΩ ── GND
```

输出电压:
```
V_out = 5V × 3kΩ / (2kΩ + 3kΩ) = 3V
```

方法2: 使用5V容忍引脚

STM32的某些引脚(如PA15、PB3、PB4)为5V容忍引脚,可以直接接5V信号。但PB13不是5V容忍引脚,建议使用电阻分压。

#### 2.2.5 红外传感器接口

红外传感器使用GPIO输入。

**引脚分配**:
- 前方: PB2
- 后方: PB3
- 左侧: PB4
- 右侧: PB5

连接红外避障模块:

```
PB2 ── OUT(前方红外)
PB3 ── OUT(后方红外)
PB4 ── OUT(左侧红外)
PB5 ── OUT(右侧红外)
GND ── GND
3.3V ── VCC
```

**输入模式**:

红外传感器输出为数字信号(0或1),配置GPIO为输入模式,使能内部上拉电阻。

### 2.3 电源管理系统

#### 2.3.1 电源架构

系统电源架构:

```
7.4V锂电池
   │
   ├─→ LM2596 → 5V → 舵机×4
   │              └→ HC-05蓝牙
   │              └→ SYN6288语音合成
   │              └→ HC-SR04超声波
   │
   └─→ AMS1117-3.3 → 3.3V → STM32
                           └→ OLED
                           └→ LD3320语音识别
                           └→ 红外传感器×4
```

#### 2.3.2 5V稳压电路

使用LM2596 DC-DC降压模块,将7.4V转换为5V。

**模块参数**:
- 输入电压: 4.5V~40V
- 输出电压: 1.25V~37V(可调)
- 输出电流: 3A(最大)
- 效率: 92%
- 纹波: <50mV

**连接方式**:

```
电池+ ── IN+(LM2596)
电池- ── IN-(LM2596)
OUT+ ── 5V输出
OUT- ── GND
```

**输出电压调节**:

通过调节电位器,使输出电压为5.0V±0.1V。

#### 2.3.3 3.3V稳压电路

使用AMS1117-3.3 LDO稳压器,将5V转换为3.3V。

**芯片参数**:
- 输入电压: 4.5V~15V
- 输出电压: 3.3V
- 输出电流: 800mA(最大)
- 压差: 1.2V(典型值)
- 封装: SOT-223

**电路设计**:

```
5V ── 10μF ── AMS1117-3.3 ── 10μF ── 3.3V
              (IN)  (OUT)
               │
              GND
```

**散热设计**:

AMS1117的功耗:
```
P = (V_in - V_out) × I_out
  = (5V - 3.3V) × 0.2A
  = 0.34W
```

需要在PCB上预留足够的铜箔面积用于散热,或增加散热片。

#### 2.3.4 电源保护

**过流保护**: 在电池输出端串联自恢复保险丝(3A),防止短路损坏电池。

**反接保护**: 在电池输入端串联肖特基二极管(SS34),防止电池反接损坏电路。

**欠压保护**: 使用电池保护板,防止电池过放。

电路原理图:

```
电池+ ── 保险丝 ── 二极管 ── 电源输入
电池- ────────────────── GND
```

#### 2.3.5 电源监测

通过ADC监测电池电压,实现电量显示和低电量报警。

**分压电路**:

电池电压(7.4V)超过STM32的ADC输入范围(0~3.3V),需要分压。

```
电池+ ── 10kΩ ── PA0(ADC1_IN0) ── 3.3kΩ ── GND
```

分压比:
```
V_ADC = V_BAT × 3.3kΩ / (10kΩ + 3.3kΩ) = V_BAT × 0.248
```

当V_BAT = 7.4V时,V_ADC = 1.84V,在ADC输入范围内。

**电压计算**:

```c
uint16_t adc_value = ADC_Read();
float v_adc = adc_value * 3.3 / 4096;
float v_bat = v_adc / 0.248;
```

---

## 第三章 STM32软件架构设计

### 3.1 软件分层架构

#### 3.1.1 分层设计原则

软件系统采用分层架构,遵循以下原则:

**单一职责原则**: 每一层只负责特定的功能,不承担其他层的职责。

**依赖倒置原则**: 高层模块不依赖低层模块,两者都依赖抽象接口。

**开闭原则**: 对扩展开放,对修改关闭。添加新功能时,尽量不修改现有代码。

**接口隔离原则**: 接口应该小而专,不要设计臃肿的接口。

#### 3.1.2 四层架构

系统软件分为四层:

**第一层: 硬件抽象层(HAL)**

封装STM32标准外设库,提供统一的硬件访问接口。包括:
- 时钟配置
- GPIO操作
- 定时器操作
- 串口操作
- I2C操作
- ADC操作
- 中断管理

**第二层: 驱动层(Driver)**

实现各硬件模块的驱动程序。包括:
- PWM驱动
- 舵机驱动
- 超声波驱动
- 红外传感器驱动
- OLED驱动
- 蓝牙驱动
- 语音模块驱动

**第三层: 中间层(Middleware)**

实现通用的功能模块。包括:
- 延时函数
- 数据滤波
- 数学运算
- 字符串处理
- 队列管理
- 状态机框架

**第四层: 应用层(Application)**

实现业务逻辑。包括:
- 主程序
- 运动控制
- 避障逻辑
- 模式管理
- 通信协议
- 用户界面

#### 3.1.3 模块间通信

模块间通信遵循以下规则:

**上层调用下层**: 应用层调用中间层,中间层调用驱动层,驱动层调用硬件抽象层。

**禁止跨层调用**: 应用层不能直接调用硬件抽象层,必须通过驱动层。

**回调机制**: 下层通过回调函数通知上层事件。例如,串口接收中断通过回调函数通知应用层。

**全局变量**: 尽量避免使用全局变量。如果必须使用,应该封装在模块内部,通过接口函数访问。

### 3.2 文件组织结构

#### 3.2.1 目录结构

```
Project/
├── Start/                  # 启动文件
│   ├── startup_stm32f10x_md.s
│   └── system_stm32f10x.c
├── Library/                # STM32标准外设库
│   ├── inc/
│   │   ├── stm32f10x.h
│   │   ├── stm32f10x_gpio.h
│   │   ├── stm32f10x_rcc.h
│   │   └── ...
│   └── src/
│       ├── stm32f10x_gpio.c
│       ├── stm32f10x_rcc.c
│       └── ...
├── System/                 # 系统层
│   ├── Delay.c/h          # 延时函数
│   ├── usart1.c/h         # 串口1驱动
│   ├── usart2.c/h         # 串口2驱动
│   └── usart3.c/h         # 串口3驱动
├── Hardware/               # 硬件驱动层
│   ├── PWM.c/h            # PWM驱动
│   ├── Servo.c/h          # 舵机驱动
│   ├── Movement.c/h       # 运动控制
│   ├── UltrasonicWave.c/h # 超声波驱动
│   ├── Hongwai.c/h        # 红外驱动
│   ├── OLED.c/h           # OLED驱动
│   ├── Serial.c/h         # 蓝牙通信
│   └── syn6288.c/h        # 语音合成驱动
├── User/                   # 应用层
│   ├── main.c             # 主程序
│   ├── Mode.c/h           # 模式管理
│   ├── stm32f10x_it.c/h   # 中断服务
│   └── stm32f10x_conf.h   # 配置文件
└── Project.uvprojx         # Keil工程文件
```

#### 3.2.2 文件命名规范

**源文件**: 使用小写字母或大驼峰命名法,扩展名为.c。
- 示例: `delay.c`, `Servo.c`, `UltrasonicWave.c`

**头文件**: 与源文件同名,扩展名为.h。
- 示例: `delay.h`, `Servo.h`, `UltrasonicWave.h`

**头文件保护**: 使用宏定义防止重复包含。
```c
#ifndef __SERVO_H
#define __SERVO_H

// 头文件内容

#endif
```

#### 3.2.3 代码注释规范

**文件头注释**:

```c
/**
  ******************************************************************************
  * @file    Servo.c
  * @author  [Your Name]
  * @version V1.0
  * @date    2025-10-25
  * @brief   舵机驱动程序
  ******************************************************************************
  * @attention
  *
  * 本文件实现了舵机的PWM控制功能,包括角度设置、平滑运动等。
  *
  ******************************************************************************
  */
```

**函数注释**:

```c
/**
  * @brief  设置舵机角度
  * @param  channel: 舵机通道(1~4)
  * @param  angle: 目标角度(0~180)
  * @retval None
  */
void Servo_SetAngle(uint8_t channel, uint8_t angle)
{
    // 函数实现
}
```

**行内注释**:

```c
uint16_t ccr = angle * 2000 / 180 + 500;  // 角度转换为CCR值
```

### 3.3 数据结构设计

#### 3.3.1 舵机状态结构体

```c
typedef struct {
    uint8_t current_angle;   // 当前角度
    uint8_t target_angle;    // 目标角度
    uint8_t is_moving;       // 是否正在运动
    uint16_t speed;          // 运动速度(度/秒)
} ServoState_t;

ServoState_t servo_state[4];  // 4个舵机的状态
```

#### 3.3.2 传感器数据结构体

```c
typedef struct {
    float distance;          // 超声波测距值(cm)
    uint8_t front_ir;        // 前方红外
    uint8_t back_ir;         // 后方红外
    uint8_t left_ir;         // 左侧红外
    uint8_t right_ir;        // 右侧红外
    uint32_t timestamp;      // 时间戳(ms)
} SensorData_t;

SensorData_t sensor_data;
```

#### 3.3.3 系统状态结构体

```c
typedef enum {
    MODE_STAND = '0',        // 站立
    MODE_FORWARD = '1',      // 前进
    MODE_BACKWARD = '2',     // 后退
    MODE_TURN_LEFT = '3',    // 左转
    MODE_TURN_RIGHT = '4',   // 右转
    MODE_SIT = '5',          // 坐下
    MODE_LIE = '6',          // 趴下
    MODE_HELLO = '7',        // 打招呼
    MODE_STRETCH = '8',      // 伸懒腰
    MODE_DANCE = '9',        // 跳舞
} OperationMode_t;

typedef struct {
    OperationMode_t current_mode;   // 当前模式
    OperationMode_t previous_mode;  // 上一个模式
    uint8_t avoid_enable;           // 避障使能
    uint8_t stamina;                // 体力值(0~100)
    float battery_voltage;          // 电池电压(V)
} SystemState_t;

SystemState_t system_state;
```

#### 3.3.4 通信数据结构体

```c
typedef struct {
    uint8_t buffer[256];     // 接收缓冲区
    uint16_t head;           // 队列头指针
    uint16_t tail;           // 队列尾指针
    uint16_t count;          // 数据个数
} RxQueue_t;

RxQueue_t usart1_rx_queue;   // 串口1接收队列
```

### 3.4 状态机设计

#### 3.4.1 主状态机

主状态机管理系统的运行模式。

**状态定义**:

```c
typedef enum {
    STATE_INIT,              // 初始化状态
    STATE_IDLE,              // 空闲状态
    STATE_RUNNING,           // 运行状态
    STATE_AVOIDING,          // 避障状态
    STATE_ERROR,             // 错误状态
} MainState_t;

MainState_t main_state = STATE_INIT;
```

**状态转换**:

```
STATE_INIT → STATE_IDLE → STATE_RUNNING ⇄ STATE_AVOIDING
                                ↓
                           STATE_ERROR
```

**状态机实现**:

```c
void MainStateMachine(void)
{
    switch (main_state) {
        case STATE_INIT:
            // 系统初始化
            System_Init();
            main_state = STATE_IDLE;
            break;
            
        case STATE_IDLE:
            // 等待指令
            if (Command_Received()) {
                main_state = STATE_RUNNING;
            }
            break;
            
        case STATE_RUNNING:
            // 执行运动
            if (Obstacle_Detected() && system_state.avoid_enable) {
                main_state = STATE_AVOIDING;
            }
            Execute_Movement();
            break;
            
        case STATE_AVOIDING:
            // 执行避障
            Execute_Avoidance();
            if (Avoidance_Finished()) {
                main_state = STATE_RUNNING;
            }
            break;
            
        case STATE_ERROR:
            // 错误处理
            Error_Handler();
            break;
    }
}
```

#### 3.4.2 运动状态机

运动状态机管理舵机的运动过程。

**状态定义**:

```c
typedef enum {
    MOVE_STATE_IDLE,         // 空闲
    MOVE_STATE_PREPARING,    // 准备
    MOVE_STATE_MOVING,       // 运动中
    MOVE_STATE_FINISHING,    // 结束
} MoveState_t;

MoveState_t move_state = MOVE_STATE_IDLE;
```

**状态机实现**:

```c
void MoveStateMachine(void)
{
    switch (move_state) {
        case MOVE_STATE_IDLE:
            // 等待运动指令
            if (Move_Command_Received()) {
                move_state = MOVE_STATE_PREPARING;
            }
            break;
            
        case MOVE_STATE_PREPARING:
            // 准备运动(读取当前角度,计算目标角度)
            Prepare_Movement();
            move_state = MOVE_STATE_MOVING;
            break;
            
        case MOVE_STATE_MOVING:
            // 执行运动(平滑运动)
            if (Movement_Finished()) {
                move_state = MOVE_STATE_FINISHING;
            } else {
                Update_Servo_Angles();
            }
            break;
            
        case MOVE_STATE_FINISHING:
            // 结束运动(清理状态)
            Finish_Movement();
            move_state = MOVE_STATE_IDLE;
            break;
    }
}
```

---

## 第四章 外设驱动程序开发

### 4.1 GPIO驱动

#### 4.1.1 GPIO配置

GPIO(General Purpose Input/Output)是STM32最基本的外设。GPIO配置步骤:

**步骤1: 使能时钟**

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
```

**步骤2: 配置GPIO模式**

```c
GPIO_InitTypeDef GPIO_InitStructure;

// 配置输出引脚(LED)
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     // 推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOC, &GPIO_InitStructure);

// 配置输入引脚(按键)
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        // 上拉输入
GPIO_Init(GPIOA, &GPIO_InitStructure);
```

**GPIO模式**:
- `GPIO_Mode_AIN`: 模拟输入
- `GPIO_Mode_IN_FLOATING`: 浮空输入
- `GPIO_Mode_IPD`: 下拉输入
- `GPIO_Mode_IPU`: 上拉输入
- `GPIO_Mode_Out_OD`: 开漏输出
- `GPIO_Mode_Out_PP`: 推挽输出
- `GPIO_Mode_AF_OD`: 复用开漏输出
- `GPIO_Mode_AF_PP`: 复用推挽输出

#### 4.1.2 GPIO操作

**输出操作**:

```c
// 设置引脚为高电平
GPIO_SetBits(GPIOC, GPIO_Pin_13);

// 设置引脚为低电平
GPIO_ResetBits(GPIOC, GPIO_Pin_13);

// 翻转引脚电平
GPIO_WriteBit(GPIOC, GPIO_Pin_13, 
    (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));
```

**输入操作**:

```c
// 读取引脚电平
uint8_t pin_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

if (pin_state == 0) {
    // 引脚为低电平
} else {
    // 引脚为高电平
}
```

#### 4.1.3 GPIO中断

GPIO可以配置为外部中断,用于检测引脚电平变化。

**配置步骤**:

**步骤1: 配置GPIO为输入模式**

```c
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(GPIOA, &GPIO_InitStructure);
```

**步骤2: 配置EXTI(外部中断)**

```c
// 使能AFIO时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

// 连接EXTI线到GPIO引脚
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

// 配置EXTI
EXTI_InitTypeDef EXTI_InitStructure;
EXTI_InitStructure.EXTI_Line = EXTI_Line0;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);
```

**步骤3: 配置NVIC(中断控制器)**

```c
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

**步骤4: 编写中断服务函数**

```c
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // 处理中断
        // ...
        
        // 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
```

### 4.2 定时器驱动

#### 4.2.1 定时器基本配置

STM32F103C8T6有4个定时器:
- TIM1: 高级定时器
- TIM2, TIM3, TIM4: 通用定时器

本项目使用TIM3生成PWM信号,使用TIM4测量超声波回波时间。

**TIM3配置(PWM输出)**:

```c
void TIM3_PWM_Init(void)
{
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // 2. 配置GPIO为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 3. 配置定时器时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;        // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;        // PSC
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 4. 配置PWM模式
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    
    // 5. 使能定时器
    TIM_Cmd(TIM3, ENABLE);
}
```

**TIM4配置(定时计数)**:

```c
void TIM4_Init(void)
{
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 2. 配置定时器
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;           // ARR(最大值)
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;       // PSC(1MHz计数频率)
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    // 3. 使能定时器
    TIM_Cmd(TIM4, ENABLE);
}
```

#### 4.2.2 PWM输出控制

**设置PWM占空比**:

```c
void PWM_SetCompare1(uint16_t compare)
{
    TIM_SetCompare1(TIM3, compare);
}

void PWM_SetCompare2(uint16_t compare)
{
    TIM_SetCompare2(TIM3, compare);
}

void PWM_SetCompare3(uint16_t compare)
{
    TIM_SetCompare3(TIM3, compare);
}

void PWM_SetCompare4(uint16_t compare)
{
    TIM_SetCompare4(TIM3, compare);
}
```

**读取PWM占空比**:

```c
uint16_t PWM_GetCompare1(void)
{
    return TIM_GetCapture1(TIM3);
}
```

#### 4.2.3 定时器中断

定时器可以配置为定时中断,用于周期性执行任务。

**配置步骤**:

```c
void TIM2_Init(void)
{
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 2. 配置定时器(1ms中断一次)
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;        // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;       // PSC
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    // 3. 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    // 4. 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 5. 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}
```

**中断服务函数**:

```c
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        // 每1ms执行一次
        // ...
        
        // 清除中断标志
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
```

### 4.3 串口驱动

#### 4.3.1 串口配置

STM32F103C8T6有3个USART。本项目使用USART1连接蓝牙,USART2连接语音合成,USART3连接语音识别。

**USART1配置**:

```c
void USART1_Init(uint32_t baudrate)
{
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // TX引脚: 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // RX引脚: 浮空输入或上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    
    // 4. 使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
    // 5. 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 6. 使能USART
    USART_Cmd(USART1, ENABLE);
}
```

#### 4.3.2 串口发送

**发送单个字节**:

```c
void USART1_SendByte(uint8_t byte)
{
    // 等待发送缓冲区空
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    // 发送数据
    USART_SendData(USART1, byte);
    
    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
```

**发送字符串**:

```c
void USART1_SendString(char *str)
{
    while (*str != '\0') {
        USART1_SendByte(*str);
        str++;
    }
}
```

**发送数组**:

```c
void USART1_SendArray(uint8_t *array, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        USART1_SendByte(array[i]);
    }
}
```

#### 4.3.3 串口接收

串口接收通过中断实现。

**中断服务函数**:

```c
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // 读取接收到的数据
        uint8_t data = USART_ReceiveData(USART1);
        
        // 将数据存入接收队列
        RxQueue_Push(&usart1_rx_queue, data);
        
        // 清除中断标志(读取数据后自动清除)
    }
}
```

**接收队列操作**:

```c
void RxQueue_Init(RxQueue_t *queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
}

void RxQueue_Push(RxQueue_t *queue, uint8_t data)
{
    if (queue->count < 256) {
        queue->buffer[queue->tail] = data;
        queue->tail = (queue->tail + 1) % 256;
        queue->count++;
    }
}

uint8_t RxQueue_Pop(RxQueue_t *queue)
{
    uint8_t data = 0;
    
    if (queue->count > 0) {
        data = queue->buffer[queue->head];
        queue->head = (queue->head + 1) % 256;
        queue->count--;
    }
    
    return data;
}

uint8_t RxQueue_IsEmpty(RxQueue_t *queue)
{
    return (queue->count == 0);
}
```

### 4.4 I2C驱动

#### 4.4.1 I2C配置

I2C用于连接OLED显示屏。STM32的I2C外设配置较为复杂,也可以使用GPIO模拟I2C。

**硬件I2C配置**:

```c
void I2C1_Init(void)
{
    // 1. 使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 2. 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // 复用开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 3. 配置I2C
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;  // 400kHz
    I2C_Init(I2C1, &I2C_InitStructure);
    
    // 4. 使能I2C
    I2C_Cmd(I2C1, ENABLE);
}
```

**软件I2C(GPIO模拟)**:

由于硬件I2C配置复杂,且容易出现死锁问题,本项目采用GPIO模拟I2C。

```c
#define I2C_SCL_PIN GPIO_Pin_6
#define I2C_SDA_PIN GPIO_Pin_7
#define I2C_GPIO GPIOB

void I2C_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
    
    GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN | I2C_SDA_PIN);
}

void I2C_Start(void)
{
    GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN);
    GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN);
    Delay_us(5);
    GPIO_ResetBits(I2C_GPIO, I2C_SDA_PIN);
    Delay_us(5);
    GPIO_ResetBits(I2C_GPIO, I2C_SCL_PIN);
}

void I2C_Stop(void)
{
    GPIO_ResetBits(I2C_GPIO, I2C_SDA_PIN);
    GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN);
    Delay_us(5);
    GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN);
    Delay_us(5);
}

void I2C_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & 0x80) {
            GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN);
        } else {
            GPIO_ResetBits(I2C_GPIO, I2C_SDA_PIN);
        }
        byte <<= 1;
        Delay_us(2);
        GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN);
        Delay_us(5);
        GPIO_ResetBits(I2C_GPIO, I2C_SCL_PIN);
        Delay_us(2);
    }
}

uint8_t I2C_ReceiveAck(void)
{
    uint8_t ack;
    GPIO_SetBits(I2C_GPIO, I2C_SDA_PIN);
    Delay_us(2);
    GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN);
    Delay_us(2);
    ack = GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA_PIN);
    Delay_us(2);
    GPIO_ResetBits(I2C_GPIO, I2C_SCL_PIN);
    Delay_us(2);
    return ack;
}
```

#### 4.4.2 I2C数据传输

**写数据**:

```c
void I2C_WriteData(uint8_t addr, uint8_t reg, uint8_t data)
{
    I2C_Start();
    I2C_SendByte(addr << 1 | 0);  // 发送设备地址+写命令
    I2C_ReceiveAck();
    I2C_SendByte(reg);             // 发送寄存器地址
    I2C_ReceiveAck();
    I2C_SendByte(data);            // 发送数据
    I2C_ReceiveAck();
    I2C_Stop();
}
```

**读数据**:

```c
uint8_t I2C_ReadData(uint8_t addr, uint8_t reg)
{
    uint8_t data;
    
    I2C_Start();
    I2C_SendByte(addr << 1 | 0);  // 发送设备地址+写命令
    I2C_ReceiveAck();
    I2C_SendByte(reg);             // 发送寄存器地址
    I2C_ReceiveAck();
    
    I2C_Start();                   // 重新启动
    I2C_SendByte(addr << 1 | 1);  // 发送设备地址+读命令
    I2C_ReceiveAck();
    data = I2C_ReceiveByte();      // 接收数据
    I2C_SendAck(1);                // 发送NACK
    I2C_Stop();
    
    return data;
}
```

---

## 第五章 应用程序开发

### 5.1 主程序设计

#### 5.1.1 主程序流程

主程序采用"初始化+主循环"结构:

```c
int main(void)
{
    // 系统初始化
    System_Init();
    
    // 主循环
    while (1) {
        // 读取传感器数据
        Sensor_Update();
        
        // 处理通信数据
        Communication_Process();
        
        // 执行运动控制
        Movement_Control();
        
        // 更新显示
        Display_Update();
        
        // 延时
        Delay_ms(50);
    }
}
```

#### 5.1.2 系统初始化

```c
void System_Init(void)
{
    // 1. 配置系统时钟(72MHz)
    SystemInit();
    
    // 2. 配置NVIC优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // 3. 初始化延时函数
    Delay_Init();
    
    // 4. 初始化GPIO
    GPIO_Init_All();
    
    // 5. 初始化定时器
    TIM3_PWM_Init();
    TIM4_Init();
    
    // 6. 初始化串口
    USART1_Init(9600);   // 蓝牙
    USART2_Init(9600);   // 语音合成
    USART3_Init(9600);   // 语音识别
    
    // 7. 初始化I2C
    I2C_GPIO_Init();
    
    // 8. 初始化OLED
    OLED_Init();
    OLED_ShowString(1, 1, "System Init...");
    
    // 9. 初始化舵机(回中位)
    Servo_Init();
    
    // 10. 初始化系统状态
    system_state.current_mode = MODE_STAND;
    system_state.previous_mode = MODE_STAND;
    system_state.avoid_enable = 1;
    system_state.stamina = 100;
    
    // 11. 延时等待系统稳定
    Delay_ms(1000);
    
    // 12. 显示就绪信息
    OLED_Clear();
    OLED_ShowString(1, 1, "Ready!");
}
```

#### 5.1.3 传感器更新

```c
void Sensor_Update(void)
{
    // 更新超声波测距
    sensor_data.distance = UltrasonicWave_GetDistance_Filtered();
    
    // 更新红外传感器
    sensor_data.front_ir = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
    sensor_data.back_ir = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    sensor_data.left_ir = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
    sensor_data.right_ir = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
    
    // 更新时间戳
    sensor_data.timestamp = GetSystemTick();
}
```

#### 5.1.4 通信处理

```c
void Communication_Process(void)
{
    // 处理蓝牙指令
    if (!RxQueue_IsEmpty(&usart1_rx_queue)) {
        uint8_t cmd = RxQueue_Pop(&usart1_rx_queue);
        Command_Parse(cmd);
    }
    
    // 处理语音识别指令
    if (!RxQueue_IsEmpty(&usart3_rx_queue)) {
        uint8_t cmd = RxQueue_Pop(&usart3_rx_queue);
        Voice_Command_Parse(cmd);
    }
}
```

#### 5.1.5 运动控制

```c
void Movement_Control(void)
{
    // 检查是否需要避障
    if (system_state.avoid_enable && Obstacle_Detected()) {
        Avoidance_Execute();
        return;
    }
    
    // 根据当前模式执行运动
    switch (system_state.current_mode) {
        case MODE_STAND:
            move_stand();
            break;
        case MODE_FORWARD:
            move_forward();
            break;
        case MODE_BACKWARD:
            move_behind();
            break;
        case MODE_TURN_LEFT:
            move_left();
            break;
        case MODE_TURN_RIGHT:
            move_right();
            break;
        case MODE_SIT:
            move_sit();
            break;
        case MODE_LIE:
            move_lie();
            break;
        case MODE_HELLO:
            move_hello();
            break;
        case MODE_STRETCH:
            move_stretch();
            break;
        case MODE_DANCE:
            move_dance();
            break;
        default:
            move_stand();
            break;
    }
}
```

### 5.2 通信协议设计

#### 5.2.1 蓝牙通信协议

蓝牙通信采用单字符指令:

| 指令 | 功能 |
|------|------|
| '0' | 站立 |
| '1' | 前进 |
| '2' | 后退 |
| '3' | 左转 |
| '4' | 右转 |
| '5' | 坐下 |
| '6' | 趴下 |
| '7' | 打招呼 |
| '8' | 伸懒腰 |
| '9' | 跳舞 |
| 'a' | 开启避障 |
| 'b' | 关闭避障 |

**指令解析**:

```c
void Command_Parse(uint8_t cmd)
{
    // 保存上一个模式
    system_state.previous_mode = system_state.current_mode;
    
    // 解析指令
    switch (cmd) {
        case '0':
            system_state.current_mode = MODE_STAND;
            break;
        case '1':
            system_state.current_mode = MODE_FORWARD;
            break;
        case '2':
            system_state.current_mode = MODE_BACKWARD;
            break;
        case '3':
            system_state.current_mode = MODE_TURN_LEFT;
            break;
        case '4':
            system_state.current_mode = MODE_TURN_RIGHT;
            break;
        case '5':
            system_state.current_mode = MODE_SIT;
            break;
        case '6':
            system_state.current_mode = MODE_LIE;
            break;
        case '7':
            system_state.current_mode = MODE_HELLO;
            break;
        case '8':
            system_state.current_mode = MODE_STRETCH;
            break;
        case '9':
            system_state.current_mode = MODE_DANCE;
            break;
        case 'a':
            system_state.avoid_enable = 1;
            OLED_ShowString(4, 1, "Avoid: ON ");
            break;
        case 'b':
            system_state.avoid_enable = 0;
            OLED_ShowString(4, 1, "Avoid: OFF");
            break;
        default:
            break;
    }
}
```

#### 5.2.2 语音合成协议

SYN6288语音合成模块使用自定义协议:

**数据帧格式**:

```
[帧头][长度][命令][参数][数据][校验]
 0xFD  1字节  1字节  1字节  N字节  1字节
```

**发送语音合成指令**:

```c
void SYN6288_Speak(char *text)
{
    uint16_t length = strlen(text) + 2;
    uint8_t buffer[256];
    uint8_t checksum = 0;
    
    buffer[0] = 0xFD;                    // 帧头
    buffer[1] = (length >> 8) & 0xFF;    // 长度高字节
    buffer[2] = length & 0xFF;           // 长度低字节
    buffer[3] = 0x01;                    // 命令(合成播放)
    buffer[4] = 0x00;                    // 参数(背景音乐)
    
    // 复制文本数据
    for (uint16_t i = 0; i < strlen(text); i++) {
        buffer[5 + i] = text[i];
    }
    
    // 计算校验和
    for (uint16_t i = 0; i < length + 3; i++) {
        checksum ^= buffer[i];
    }
    buffer[length + 3] = checksum;
    
    // 发送数据
    USART2_SendArray(buffer, length + 4);
}
```

#### 5.2.3 语音识别协议

LD3320语音识别模块输出识别结果的ASCII码。

**识别结果映射**:

```c
void Voice_Command_Parse(uint8_t result)
{
    switch (result) {
        case 0x01:  // "前进"
            system_state.current_mode = MODE_FORWARD;
            break;
        case 0x02:  // "后退"
            system_state.current_mode = MODE_BACKWARD;
            break;
        case 0x03:  // "左转"
            system_state.current_mode = MODE_TURN_LEFT;
            break;
        case 0x04:  // "右转"
            system_state.current_mode = MODE_TURN_RIGHT;
            break;
        case 0x05:  // "停止"
            system_state.current_mode = MODE_STAND;
            break;
        default:
            break;
    }
}
```

### 5.3 任务调度设计

#### 5.3.1 时间片轮询调度

由于系统任务较少,采用简单的时间片轮询调度:

```c
uint32_t system_tick = 0;

void SysTick_Handler(void)
{
    system_tick++;
}

uint32_t GetSystemTick(void)
{
    return system_tick;
}

void Task_Scheduler(void)
{
    static uint32_t last_sensor_update = 0;
    static uint32_t last_display_update = 0;
    static uint32_t last_battery_check = 0;
    
    uint32_t current_tick = GetSystemTick();
    
    // 传感器更新任务(100ms)
    if (current_tick - last_sensor_update >= 100) {
        Sensor_Update();
        last_sensor_update = current_tick;
    }
    
    // 显示更新任务(500ms)
    if (current_tick - last_display_update >= 500) {
        Display_Update();
        last_display_update = current_tick;
    }
    
    // 电池检查任务(5000ms)
    if (current_tick - last_battery_check >= 5000) {
        Battery_Check();
        last_battery_check = current_tick;
    }
}
```

#### 5.3.2 优先级调度

对于有实时性要求的任务,可以使用优先级调度:

```c
typedef enum {
    TASK_PRIORITY_HIGH = 0,
    TASK_PRIORITY_MEDIUM,
    TASK_PRIORITY_LOW
} TaskPriority_t;

typedef struct {
    void (*function)(void);
    TaskPriority_t priority;
    uint32_t period;
    uint32_t last_run;
} Task_t;

Task_t task_list[] = {
    {Sensor_Update, TASK_PRIORITY_HIGH, 100, 0},
    {Movement_Control, TASK_PRIORITY_HIGH, 50, 0},
    {Display_Update, TASK_PRIORITY_MEDIUM, 500, 0},
    {Battery_Check, TASK_PRIORITY_LOW, 5000, 0}
};

void Task_Scheduler_Priority(void)
{
    uint32_t current_tick = GetSystemTick();
    
    // 按优先级执行任务
    for (uint8_t priority = TASK_PRIORITY_HIGH; priority <= TASK_PRIORITY_LOW; priority++) {
        for (uint8_t i = 0; i < sizeof(task_list) / sizeof(Task_t); i++) {
            if (task_list[i].priority == priority) {
                if (current_tick - task_list[i].last_run >= task_list[i].period) {
                    task_list[i].function();
                    task_list[i].last_run = current_tick;
                }
            }
        }
    }
}
```

---

## 第六章 系统调试与优化

### 6.1 调试方法

#### 6.1.1 串口调试

串口是最常用的调试方法。通过串口输出调试信息,可以实时查看程序运行状态。

**重定向printf到串口**:

```c
#include <stdio.h>

// 重定向fputc函数
int fputc(int ch, FILE *f)
{
    USART1_SendByte((uint8_t)ch);
    return ch;
}

// 使用printf输出调试信息
printf("Distance: %.2f cm\r\n", distance);
printf("Mode: %d\r\n", system_state.current_mode);
```

**调试宏定义**:

```c
#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
    #define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...)
#endif

// 使用
DEBUG_PRINT("Servo1 angle: %d\r\n", angle);
```

#### 6.1.2 LED指示调试

使用LED指示灯显示程序运行状态:

```c
#define LED_PIN GPIO_Pin_13
#define LED_GPIO GPIOC

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
}

void LED_On(void)
{
    GPIO_ResetBits(LED_GPIO, LED_PIN);
}

void LED_Off(void)
{
    GPIO_SetBits(LED_GPIO, LED_PIN);
}

void LED_Toggle(void)
{
    GPIO_WriteBit(LED_GPIO, LED_PIN, 
        (BitAction)(1 - GPIO_ReadOutputDataBit(LED_GPIO, LED_PIN)));
}

// 使用LED指示程序运行
void main_loop(void)
{
    while (1) {
        LED_Toggle();  // LED闪烁表示程序正常运行
        Delay_ms(500);
    }
}
```

#### 6.1.3 在线调试

使用Keil的在线调试功能,可以设置断点、单步执行、查看变量等。

**设置断点**:
- 在代码行号处单击,设置断点
- 按F9切换断点

**单步执行**:
- F10: 单步跳过(Step Over)
- F11: 单步进入(Step Into)
- Shift+F11: 单步跳出(Step Out)

**查看变量**:
- 在Watch窗口添加变量
- 鼠标悬停在变量上查看值

**查看寄存器**:
- View → Peripheral → 选择外设
- 查看外设寄存器的值

#### 6.1.4 逻辑分析仪调试

使用逻辑分析仪可以查看数字信号的波形,分析时序问题。

**测量PWM波形**:
- 将逻辑分析仪探头连接到PWM输出引脚
- 设置采样率(如10MHz)
- 触发条件设置为上升沿
- 观察PWM波形的频率和占空比

**分析I2C通信**:
- 连接SCL和SDA引脚
- 使用I2C协议解码功能
- 查看地址、数据、ACK等信息

### 6.2 性能优化

#### 6.2.1 代码优化

**编译器优化等级**:

Keil MDK支持多个优化等级:
- -O0: 不优化
- -O1: 基本优化
- -O2: 较高优化
- -O3: 最高优化

在Options for Target → C/C++ → Optimization中设置。

建议使用-O2,在性能和代码大小之间取得平衡。

**循环优化**:

```c
// 优化前
for (int i = 0; i < 100; i++) {
    array[i] = i * 2;
}

// 优化后(循环展开)
for (int i = 0; i < 100; i += 4) {
    array[i] = i * 2;
    array[i+1] = (i+1) * 2;
    array[i+2] = (i+2) * 2;
    array[i+3] = (i+3) * 2;
}
```

**查表法**:

```c
// 优化前(计算三角函数)
float angle_rad = angle * 3.14159 / 180.0;
float sin_value = sin(angle_rad);

// 优化后(查表)
const float sin_table[91] = {0.0, 0.017, 0.035, ...};
float sin_value = sin_table[angle];
```

#### 6.2.2 内存优化

**减少全局变量**:

全局变量占用SRAM,应尽量使用局部变量。

```c
// 不推荐
uint8_t temp_buffer[1024];

void function1(void)
{
    // 使用temp_buffer
}

// 推荐
void function1(void)
{
    uint8_t temp_buffer[1024];
    // 使用temp_buffer
}
```

**使用const修饰常量**:

const修饰的常量存储在Flash中,不占用SRAM。

```c
// 不推荐
uint8_t lookup_table[256] = {0, 1, 2, ...};

// 推荐
const uint8_t lookup_table[256] = {0, 1, 2, ...};
```

**减少栈使用**:

避免在函数中定义大数组,防止栈溢出。

```c
// 不推荐
void function(void)
{
    uint8_t large_array[2048];
    // ...
}

// 推荐
static uint8_t large_array[2048];

void function(void)
{
    // 使用large_array
}
```

#### 6.2.3 功耗优化

**降低系统时钟**:

如果不需要高性能,可以降低系统时钟频率,减少功耗。

```c
// 从72MHz降低到36MHz
RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  // 8MHz * 9 / 2 = 36MHz
```

**使用睡眠模式**:

在空闲时进入睡眠模式,降低功耗。

```c
void Enter_Sleep_Mode(void)
{
    __WFI();  // Wait For Interrupt
}

void main_loop(void)
{
    while (1) {
        // 执行任务
        Task_Execute();
        
        // 进入睡眠,等待中断唤醒
        Enter_Sleep_Mode();
    }
}
```

**关闭未使用的外设**:

```c
// 关闭未使用的外设时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
```

### 6.3 常见问题与解决

#### 6.3.1 舵机抖动

**问题**: 舵机在静止时抖动。

**原因**:
1. PWM信号不稳定
2. 电源电压波动
3. 舵机质量问题

**解决方法**:
1. 检查PWM频率和占空比是否正确
2. 增大电源滤波电容
3. 更换质量更好的舵机

#### 6.3.2 串口乱码

**问题**: 串口接收到的数据是乱码。

**原因**:
1. 波特率不匹配
2. 数据位、停止位、校验位设置错误
3. 接线错误(TX和RX接反)

**解决方法**:
1. 检查波特率设置
2. 检查串口参数(8N1)
3. 检查接线(TX接RX,RX接TX)

#### 6.3.3 I2C通信失败

**问题**: OLED显示屏无显示。

**原因**:
1. I2C地址错误
2. 上拉电阻缺失或阻值不合适
3. 时序不正确

**解决方法**:
1. 使用I2C扫描程序确认设备地址
2. 检查SCL和SDA的上拉电阻(4.7kΩ)
3. 使用逻辑分析仪检查I2C时序

#### 6.3.4 程序跑飞

**问题**: 程序运行一段时间后死机或重启。

**原因**:
1. 栈溢出
2. 数组越界
3. 指针错误
4. 看门狗复位

**解决方法**:
1. 增大栈空间
2. 检查数组访问是否越界
3. 检查指针是否为空
4. 关闭看门狗或定期喂狗

---

## 第七章 总结与展望

### 7.1 工作总结

本文从嵌入式系统开发的角度,详细介绍了基于STM32的四足机器狗控制系统的设计与实现过程。主要完成了以下工作:

**硬件系统设计**。设计了基于STM32F103C8T6的硬件平台,包括最小系统、外设接口电路、电源管理系统等。硬件设计遵循工程规范,考虑了电源滤波、信号完整性、电磁兼容等问题。

**软件架构设计**。采用分层架构,将软件系统分为硬件抽象层、驱动层、中间层和应用层。这种架构提高了代码的模块化程度和可维护性。

**外设驱动开发**。详细介绍了GPIO、定时器、串口、I2C等外设的配置方法和驱动开发技术。通过实际代码示例,展示了STM32外设的使用方法。

**应用程序开发**。实现了主程序、通信协议、任务调度等功能。通过状态机设计,实现了清晰的程序逻辑。

**系统调试与优化**。介绍了串口调试、LED指示调试、在线调试、逻辑分析仪调试等方法。提出了代码优化、内存优化、功耗优化等策略。

通过本项目的开发,积累了丰富的嵌入式系统开发经验,掌握了STM32的使用方法,为今后的嵌入式系统开发打下了坚实基础。

### 7.2 开发经验

在项目开发过程中,总结了以下经验:

**硬件设计要充分考虑可靠性**。电源滤波、信号完整性、电磁兼容等问题都会影响系统稳定性。在设计阶段就要充分考虑这些问题,避免后期调试困难。

**软件架构要模块化**。模块化的软件架构便于开发、测试和维护。每个模块应该有清晰的接口,模块之间的耦合度要低。

**调试要分步进行**。不要一次性集成所有功能,应该分模块逐步调试。先调试基本功能,再调试复杂功能。

**文档要及时整理**。在开发过程中要及时整理文档,记录设计思路、遇到的问题和解决方法。这些文档对后期维护和二次开发非常有帮助。

**代码要规范**。遵循编码规范,使用有意义的变量名和函数名,添加必要的注释。规范的代码易于阅读和维护。

### 7.3 未来改进方向

本项目还有以下改进方向:

**引入RTOS**。当前系统使用裸机程序,任务调度较为简单。引入RTOS(如FreeRTOS)可以实现更灵活的任务管理和资源分配。

**增加传感器**。增加IMU(惯性测量单元)、电流传感器等,实现姿态反馈控制和电流监测。

**优化通信协议**。当前通信协议较为简单,可以设计更完善的协议,支持参数配置、状态查询等功能。

**增加数据记录功能**。通过SD卡或Flash存储运行数据,用于后期分析和优化。

**移植到其他平台**。将代码移植到STM32F4、STM32H7等更高性能的平台,或移植到其他MCU(如ESP32)。

**开发上位机软件**。开发PC端或手机端的上位机软件,实现图形化控制和参数配置。

### 7.4 结语

嵌入式系统开发是一个理论与实践紧密结合的过程。通过本项目的开发,不仅学习了STM32的使用方法,更重要的是掌握了嵌入式系统开发的思路和方法。希望本文能为嵌入式系统学习者提供有价值的参考。

---

## 致谢

本项目的完成离不开多方面的支持和帮助。感谢指导老师的悉心指导,感谢实验室同学的帮助,感谢开源社区提供的技术资料,感谢家人的理解和支持。

---

## 参考文献

[1] 刘火良, 杨森. STM32库开发实战指南[M]. 北京: 机械工业出版社, 2016.

[2] 野火. STM32库开发实战指南[M]. 北京: 机械工业出版社, 2018.

[3] ARM. Cortex-M3 Technical Reference Manual[R]. 2010.

[4] STMicroelectronics. STM32F103xx Reference Manual[R]. 2015.

[5] STMicroelectronics. STM32F10xxx Cortex-M3 Programming Manual[R]. 2010.

[6] 周立功. 深入浅出ARM7-LPC213x/214x[M]. 北京: 北京航空航天大学出版社, 2005.

[7] Joseph Yiu. The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors[M]. Newnes, 2013.

---



