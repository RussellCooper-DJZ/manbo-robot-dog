/**
  ******************************************************************************
  * @file    asr.cpp
  * @author  Manbo Robot Dog Team
  * @version V2.0
  * @date    2025-10-31
  * @brief   ASR Pro语音识别模块实现
  ******************************************************************************
  */

#include "asr.h"
#include "commands.h"

/* Arduino/ESP32库 */
#include <Arduino.h>
#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/* 外部依赖 */
extern "C" {
    void* __dso_handle = 0;
}

/* ASR Pro库函数声明 (假设这些函数由ASR Pro SDK提供) */
extern "C" {
    void set_state_enter_wakeup(uint32_t timeout);
    void vol_set(uint8_t volume);
    void play_audio(uint16_t audio_id);
    void setPinFun(uint8_t pin, uint8_t function);
}

/* 常量定义 */
#define FORTH_FUNCTION  4  // GPIO功能定义

/* 全局变量 ------------------------------------------------------------------*/
static ASR_State_t g_ASR_State = ASR_STATE_IDLE;
static QueueHandle_t g_MessageQueue = NULL;
static uint32_t g_CurrentSnid = 0;
static uint8_t g_CurrentVolume = ASR_DEFAULT_VOLUME;

/* 串口对象 */
HardwareSerial Serial2(2);  // 使用UART2

/* 私有函数声明 --------------------------------------------------------------*/
static void ASR_ProcessCommand(uint32_t snid);
static bool ASR_SendCommandWithDelay(char cmd, uint32_t delay_ms);
static void ASR_ErrorHandler(const char* error_msg);

/* 公共函数实现 --------------------------------------------------------------*/

/**
 * @brief  初始化ASR Pro模块
 * @param  None
 * @retval true: 初始化成功, false: 初始化失败
 */
bool ASR_Init(void)
{
    // 创建消息队列
    g_MessageQueue = xQueueCreate(ASR_QUEUE_SIZE, sizeof(uint32_t));
    if (g_MessageQueue == NULL) {
        ASR_ErrorHandler("Failed to create message queue");
        return false;
    }
    
    // 设置默认音量
    vol_set(g_CurrentVolume);
    
    // 配置GPIO引脚功能
    setPinFun(ASR_SERIAL_TX_PIN, FORTH_FUNCTION);
    setPinFun(ASR_SERIAL_RX_PIN, FORTH_FUNCTION);
    
    // 初始化串口
    Serial2.begin(ASR_SERIAL_BAUDRATE);
    Serial2.setTimeout(ASR_SERIAL_TIMEOUT);
    
    // 设置初始状态
    g_ASR_State = ASR_STATE_IDLE;
    
    // 设置唤醒超时
    set_state_enter_wakeup(ASR_WAKEUP_TIMEOUT);
    
    return true;
}

/**
 * @brief  ASR主处理函数，需在主循环中调用
 * @param  None
 * @retval None
 */
void ASR_Process(void)
{
    uint32_t snid;
    
    // 检查消息队列
    if (xQueueReceive(g_MessageQueue, &snid, 0) == pdTRUE) {
        g_CurrentSnid = snid;
        g_ASR_State = ASR_STATE_PROCESSING;
        
        // 处理命令
        ASR_ProcessCommand(snid);
        
        g_ASR_State = ASR_STATE_IDLE;
    }
}

/**
 * @brief  处理语音识别命令
 * @param  snid: 语音识别ID
 * @retval true: 处理成功, false: 处理失败
 */
bool ASR_HandleCommand(uint32_t snid)
{
    // 将命令放入队列
    if (xQueueSend(g_MessageQueue, &snid, 0) != pdTRUE) {
        ASR_ErrorHandler("Message queue full");
        return false;
    }
    
    return true;
}

/**
 * @brief  设置音量
 * @param  volume: 音量等级 (0-8)
 * @retval true: 设置成功, false: 设置失败
 */
bool ASR_SetVolume(uint8_t volume)
{
    if (volume > 8) {
        return false;
    }
    
    g_CurrentVolume = volume;
    vol_set(volume);
    
    return true;
}

/**
 * @brief  获取当前状态
 * @param  None
 * @retval ASR模块当前状态
 */
ASR_State_t ASR_GetState(void)
{
    return g_ASR_State;
}

/**
 * @brief  播放音频
 * @param  audio_id: 音频ID
 * @retval true: 播放成功, false: 播放失败
 */
bool ASR_PlayAudio(uint16_t audio_id)
{
    play_audio(audio_id);
    return true;
}

/**
 * @brief  发送命令到STM32
 * @param  cmd: 命令字符
 * @retval true: 发送成功, false: 发送失败
 */
bool ASR_SendCommand(char cmd)
{
    if (Serial2.availableForWrite() > 0) {
        Serial2.print(cmd);
        return true;
    }
    
    return false;
}

/**
 * @brief  设置唤醒超时时间
 * @param  timeout_ms: 超时时间(毫秒)
 * @retval None
 */
void ASR_SetWakeupTimeout(uint32_t timeout_ms)
{
    set_state_enter_wakeup(timeout_ms);
}

/* 私有函数实现 --------------------------------------------------------------*/

/**
 * @brief  处理语音识别命令
 * @param  snid: 语音识别ID
 * @retval None
 */
static void ASR_ProcessCommand(uint32_t snid)
{
    char cmd;
    
    // 设置唤醒超时
    set_state_enter_wakeup(ASR_WAKEUP_TIMEOUT);
    
    // 处理音乐播放命令
    if (snid >= ASR_ID_MUSIC_1 && snid <= ASR_ID_MUSIC_3) {
        uint16_t audio_id;
        
        switch (snid) {
            case ASR_ID_MUSIC_1:
                audio_id = AUDIO_ID_HELLO;
                break;
            case ASR_ID_MUSIC_2:
                audio_id = AUDIO_ID_HAKIXIA;
                break;
            case ASR_ID_MUSIC_3:
                audio_id = AUDIO_ID_HAKIMI;
                break;
            default:
                return;
        }
        
        ASR_PlayAudio(audio_id);
        return;
    }
    
    // 处理音量控制命令
    if (snid >= ASR_VOL_MIN && snid <= ASR_VOL_MAX) {
        uint8_t volume;
        
        switch (snid) {
            case ASR_VOL_MAX:
                volume = 8;
                break;
            case ASR_VOL_MID:
                volume = 4;
                break;
            case ASR_VOL_MIN:
                volume = 1;
                break;
            default:
                return;
        }
        
        ASR_SetVolume(volume);
        return;
    }
    
    // 处理运动控制命令
    if (GetCommandBySnid(snid, &cmd)) {
        g_ASR_State = ASR_STATE_SENDING;
        
        // 对于运动命令，需要延迟后发送停止命令
        if (snid >= ASR_ID_FORWARD && snid <= ASR_ID_DANCE) {
            ASR_SendCommandWithDelay(cmd, ASR_ACTION_DELAY);
        } else {
            ASR_SendCommand(cmd);
        }
    }
}

/**
 * @brief  发送命令并在延迟后发送停止命令
 * @param  cmd: 命令字符
 * @param  delay_ms: 延迟时间(毫秒)
 * @retval true: 发送成功, false: 发送失败
 */
static bool ASR_SendCommandWithDelay(char cmd, uint32_t delay_ms)
{
    // 发送动作命令
    if (!ASR_SendCommand(cmd)) {
        return false;
    }
    
    // 延迟
    delay(delay_ms);
    
    // 发送停止命令
    return ASR_SendCommand(CMD_STOP);
}

/**
 * @brief  错误处理函数
 * @param  error_msg: 错误信息
 * @retval None
 */
static void ASR_ErrorHandler(const char* error_msg)
{
    g_ASR_State = ASR_STATE_ERROR;
    
    // 输出错误信息到串口(用于调试)
    Serial.print("[ASR ERROR] ");
    Serial.println(error_msg);
    
    // 可以在这里添加LED指示或其他错误处理逻辑
}

/* Arduino Setup和Loop函数 ---------------------------------------------------*/

/**
 * @brief  Arduino setup函数
 * @note   在main.cpp中调用
 */
void setup()
{
    // 初始化调试串口
    Serial.begin(115200);
    Serial.println("ASR Pro Module Starting...");
    
    // 初始化ASR模块
    if (ASR_Init()) {
        Serial.println("ASR Module Initialized Successfully");
    } else {
        Serial.println("ASR Module Initialization Failed");
    }
    
    Serial.println("System Ready");
}

/**
 * @brief  Arduino loop函数
 * @note   在main.cpp中调用
 */
void loop()
{
    // 处理ASR事件
    ASR_Process();
    
    // 可以在这里添加其他周期性任务
    
    // 短暂延迟，避免CPU占用过高
    delay(10);
}
