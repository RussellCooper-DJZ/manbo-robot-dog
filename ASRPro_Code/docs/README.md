# ASR Pro 语音识别模块 (ESP32)

本项目是为曼波机器狗设计的独立语音识别模块，基于ESP32开发板和ASR Pro离线语音识别SDK。它通过串口与STM32主控板通信，实现对机器狗的语音控制。

## 模块功能

- **离线语音识别**: 无需联网即可识别特定命令词。
- **串口通信**: 将识别结果转换为单字符命令，发送给STM32主控。
- **音频播放**: 支持播放预设的提示音和歌曲。
- **音量控制**: 支持通过语音指令调节音量大小。
- **模块化设计**: 代码结构清晰，易于扩展和维护。

## 硬件配置

- **核心控制器**: ESP32-WROOM-32 或其他兼容开发板
- **语音识别SDK**: ASR Pro (具体型号依赖用户实际使用的模块)
- **通信接口**: UART (串口)

## 项目结构

```
ASRPro_Code/
├── src/                  # 源代码目录
│   ├── asr.cpp           # 核心功能实现
│   ├── commands.cpp      # 命令映射表
│   └── main.cpp          # 主程序入口 (Arduino)
├── include/              # 头文件目录
│   ├── asr.h             # 核心功能头文件
│   └── commands.h        # 命令定义
├── examples/             # 示例代码
│   ├── basic_usage.cpp   # 基本用法示例
│   └── custom_commands.cpp # 自定义命令示例
├── docs/                 # 文档目录
│   ├── README.md         # 本文档
│   ├── protocol.md       # 通信协议说明
│   └── wiring.md         # 硬件接线图
└── platformio.ini        # PlatformIO项目配置文件
```

## 开发环境

- **IDE**: [PlatformIO for VS Code](https://platformio.org/platformio-ide)
- **框架**: Arduino
- **平台**: Espressif 32

## 快速开始

### 1. 安装PlatformIO

请确保您已在VS Code中安装了PlatformIO IDE扩展。

### 2. 克隆项目

将整个`manbo-robot-dog`仓库克隆到本地。

### 3. 打开项目

在VS Code中，通过 `PlatformIO Home` > `Open` > `Open Project`，选择`manbo-robot-dog/ASRPro_Code`目录。

### 4. 配置依赖库

本项目依赖于ASR Pro官方提供的SDK。请根据您使用的具体模块型号，在`platformio.ini`文件的`lib_deps`部分添加对应的库依赖。例如：

```ini
lib_deps = 
    https://github.com/espressif/arduino-esp32.git
    # 假设的ASR Pro SDK库
    # https://github.com/YourASRProvider/asr-pro-sdk.git
```

**注意**: 由于无法获知您使用的具体ASR Pro SDK，您需要手动添加此依赖项才能成功编译。

### 5. 编译和上传

- **编译**: 点击PlatformIO工具栏上的 `Build` 按钮 (或按 `Ctrl+Alt+B`)。
- **上传**: 连接ESP32开发板，点击 `Upload` 按钮 (或按 `Ctrl+Alt+U`)。

## 硬件接线

请参考 [硬件接线文档 (wiring.md)](wiring.md) 完成ESP32与STM32主控板的连接。

## 通信协议

本模块与STM32主控之间采用简单的单字符串口通信协议。详细信息请参考 [通信协议文档 (protocol.md)](protocol.md)。

## 如何贡献

欢迎通过提交Issue或Pull Request来改进此项目。
