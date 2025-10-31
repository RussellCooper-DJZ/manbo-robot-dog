/**
  ******************************************************************************
  * @file    basic_usage.cpp
  * @author  Manbo Robot Dog Team
  * @version V1.0
  * @date    2025-10-31
  * @brief   ASR Pro模块基本使用示例
  ******************************************************************************
  */

#include <Arduino.h>
#include "asr.h"

/**
 * @brief  Arduino初始化函数
 */
void setup() {
    // 初始化调试串口
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=================================");
    Serial.println("  ASR Pro Basic Usage Example");
    Serial.println("=================================");
    
    // 初始化ASR模块
    if (ASR_Init()) {
        Serial.println("[OK] ASR Module Initialized");
    } else {
        Serial.println("[ERROR] ASR Initialization Failed");
        while(1); // 停止运行
    }
    
    // 设置音量为中等
    ASR_SetVolume(4);
    Serial.println("[OK] Volume set to 4");
    
    Serial.println("\nSystem Ready. Waiting for voice commands...\n");
}

/**
 * @brief  Arduino主循环函数
 */
void loop() {
    // 处理ASR事件
    ASR_Process();
    
    // 打印当前状态（每5秒一次）
    static uint32_t last_print = 0;
    if (millis() - last_print > 5000) {
        last_print = millis();
        
        ASR_State_t state = ASR_GetState();
        Serial.print("Current State: ");
        
        switch(state) {
            case ASR_STATE_IDLE:
                Serial.println("IDLE");
                break;
            case ASR_STATE_LISTENING:
                Serial.println("LISTENING");
                break;
            case ASR_STATE_PROCESSING:
                Serial.println("PROCESSING");
                break;
            case ASR_STATE_SENDING:
                Serial.println("SENDING");
                break;
            case ASR_STATE_ERROR:
                Serial.println("ERROR");
                break;
        }
    }
    
    delay(10);
}

/**
 * @brief  语音识别回调函数示例
 * @note   这个函数需要在ASR Pro SDK的事件回调中调用
 * @param  snid: 语音识别ID
 */
void onVoiceRecognized(uint32_t snid) {
    Serial.print("Voice Recognized: SNID = ");
    Serial.println(snid);
    
    // 处理语音命令
    if (ASR_HandleCommand(snid)) {
        Serial.println("[OK] Command queued");
    } else {
        Serial.println("[ERROR] Failed to queue command");
    }
}
