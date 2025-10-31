# ASR Pro 语音识别模块

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](../LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework](https://img.shields.io/badge/framework-Arduino-00979D.svg)](https://www.arduino.cc/)

本模块是曼波机器狗的语音识别控制系统，基于ESP32和ASR Pro离线语音识别SDK开发。

## 📋 功能特性

- ✅ **离线语音识别**: 无需联网即可识别预设命令词
- ✅ **串口通信**: 通过UART与STM32主控板通信
- ✅ **音频播放**: 支持播放提示音和背景音乐
- ✅ **音量控制**: 支持语音调节音量大小
- ✅ **模块化设计**: 代码结构清晰，易于维护和扩展
- ✅ **FreeRTOS集成**: 使用消息队列实现异步事件处理
- ✅ **错误处理**: 完善的错误检测和状态管理机制

## 🚀 快速开始

### 环境准备

1. 安装 [VS Code](https://code.visualstudio.com/)
2. 安装 [PlatformIO IDE](https://platformio.org/platformio-ide) 扩展
3. 准备ESP32开发板和ASR Pro语音识别模块

### 编译和上传

```bash
# 克隆仓库
git clone https://github.com/RussellCooper-DJZ/manbo-robot-dog.git
cd manbo-robot-dog/ASRPro_Code

# 使用PlatformIO编译
pio run

# 上传到ESP32
pio run --target upload

# 查看串口输出
pio device monitor
```

## 📁 项目结构

```
ASRPro_Code/
├── src/                  # 源代码
│   ├── asr.cpp           # 核心功能实现
│   ├── commands.cpp      # 命令映射表
│   └── main.cpp          # 主程序入口
├── include/              # 头文件
│   ├── asr.h             # 核心API
│   └── commands.h        # 命令定义
├── examples/             # 示例代码
│   ├── basic_usage.cpp   # 基本用法
│   └── custom_commands.cpp # 自定义命令
├── docs/                 # 文档
│   ├── README.md         # 详细说明
│   ├── protocol.md       # 通信协议
│   └── wiring.md         # 接线图
└── platformio.ini        # 项目配置
```

## 🔌 硬件连接

| ESP32 | STM32 | 功能 |
|:---:|:---:|:---|
| GPIO5 (TX2) | PA11 (RX3) | 发送命令 |
| GPIO6 (RX2) | PA10 (TX3) | 接收响应 |
| GND | GND | 共地 |

详细接线说明请参考 [docs/wiring.md](docs/wiring.md)

## 📡 通信协议

本模块与STM32采用单字符串口通信协议：

| 命令 | 功能 | 命令 | 功能 |
|:---:|:---|:---:|:---|
| `f` | 前进 | `b` | 后退 |
| `l` | 左转 | `r` | 右转 |
| `5` | 停止 | `d` | 跳舞 |
| `s` | 握手 | `o` | 打招呼 |
| `x` | 开启超声波避障 | `c` | 关闭超声波避障 |

完整协议说明请参考 [docs/protocol.md](docs/protocol.md)

## 💻 API 参考

### 初始化

```cpp
bool ASR_Init(void);
```

### 主循环处理

```cpp
void ASR_Process(void);
```

### 命令处理

```cpp
bool ASR_HandleCommand(uint32_t snid);
```

### 音量控制

```cpp
bool ASR_SetVolume(uint8_t volume);  // 0-8
```

### 音频播放

```cpp
bool ASR_PlayAudio(uint16_t audio_id);
```

## 📖 使用示例

```cpp
#include <Arduino.h>
#include "asr.h"

void setup() {
    Serial.begin(115200);
    
    // 初始化ASR模块
    if (ASR_Init()) {
        Serial.println("ASR Module Ready");
    }
    
    // 设置音量
    ASR_SetVolume(5);
}

void loop() {
    // 处理ASR事件
    ASR_Process();
    delay(10);
}

// 语音识别回调
void onVoiceRecognized(uint32_t snid) {
    ASR_HandleCommand(snid);
}
```

## 🛠️ 自定义开发

### 添加新命令

1. 在 `include/commands.h` 中定义新的命令字符
2. 在 `src/commands.cpp` 的命令映射表中添加映射关系
3. 在STM32端添加对应的命令处理逻辑

示例请参考 [examples/custom_commands.cpp](examples/custom_commands.cpp)

## 📊 代码统计

- **总代码行数**: 859 行
- **源文件数**: 3 个 (.cpp)
- **头文件数**: 2 个 (.h)
- **示例代码**: 2 个
- **文档文件**: 3 个

## 🤝 贡献

欢迎提交Issue和Pull Request！

## 📄 许可证

本项目采用 [MIT License](../LICENSE) 开源协议。

## 📮 联系方式

- **作者**: RussellCooper
- **邮箱**: dwjmrz@qq.com
- **GitHub**: [RussellCooper-DJZ/manbo-robot-dog](https://github.com/RussellCooper-DJZ/manbo-robot-dog)

---

**注意**: 本模块需要配合ASR Pro SDK使用，请根据实际使用的模块型号配置相应的依赖库。
