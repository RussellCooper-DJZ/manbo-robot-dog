/**
  ******************************************************************************
  * @file    custom_commands.cpp
  * @author  Manbo Robot Dog Team
  * @version V1.0
  * @date    2025-10-31
  * @brief   自定义命令处理示例
  *          演示如何添加自定义的语音命令处理逻辑
  ******************************************************************************
  */

#include <Arduino.h>
#include "asr.h"
#include "commands.h"

// 自定义命令ID（扩展）
#define ASR_ID_CUSTOM_1     100
#define ASR_ID_CUSTOM_2     101

/**
 * @brief  处理自定义命令
 * @param  snid: 语音识别ID
 * @retval true: 已处理, false: 未处理
 */
bool handleCustomCommand(uint32_t snid) {
    switch(snid) {
        case ASR_ID_CUSTOM_1:
            Serial.println("Executing Custom Command 1");
            // 在这里添加自定义逻辑
            ASR_SendCommand('C');  // 发送自定义命令字符
            return true;
            
        case ASR_ID_CUSTOM_2:
            Serial.println("Executing Custom Command 2");
            // 在这里添加自定义逻辑
            ASR_PlayAudio(10600);  // 播放自定义音频
            return true;
            
        default:
            return false;  // 未处理
    }
}

/**
 * @brief  Arduino初始化函数
 */
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Custom Commands Example");
    
    if (ASR_Init()) {
        Serial.println("ASR Module Initialized");
    } else {
        Serial.println("ASR Initialization Failed");
        while(1);
    }
}

/**
 * @brief  Arduino主循环函数
 */
void loop() {
    ASR_Process();
    delay(10);
}

/**
 * @brief  语音识别回调函数（带自定义处理）
 * @param  snid: 语音识别ID
 */
void onVoiceRecognized(uint32_t snid) {
    Serial.print("Voice Recognized: SNID = ");
    Serial.println(snid);
    
    // 先尝试处理自定义命令
    if (handleCustomCommand(snid)) {
        Serial.println("Custom command handled");
        return;
    }
    
    // 如果不是自定义命令，使用标准处理
    if (ASR_HandleCommand(snid)) {
        Serial.println("Standard command queued");
    } else {
        Serial.println("Unknown command");
    }
}
