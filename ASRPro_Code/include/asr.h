/**
  ******************************************************************************
  * @file    asr.h
  * @author  Manbo Robot Dog Team
  * @version V2.0
  * @date    2025-10-31
  * @brief   ASR Pro语音识别模块头文件
  *          本模块基于ESP32，通过UART与STM32主控通信
  ******************************************************************************
  * @attention
  *
  * 硬件连接:
  *   - ESP32 GPIO5 (TX) -> STM32 USART3 RX
  *   - ESP32 GPIO6 (RX) -> STM32 USART3 TX
  *   - 波特率: 9600
  *
  * 使用说明:
  *   1. 调用 ASR_Init() 初始化模块
  *   2. 在主循环中调用 ASR_Process() 处理语音识别事件
  *   3. 通过 ASR_SetVolume() 调整音量
  *
  ******************************************************************************
  */

#ifndef __ASR_H
#define __ASR_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件 ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* 语音识别ID定义 --------------------------------------------------------------*/

/* 基本运动命令 (1-10) */
#define ASR_ID_FORWARD          1      /**< 往前走俩步 */
#define ASR_ID_BACKWARD         2      /**< 后退 */
#define ASR_ID_TURN_LEFT        3      /**< 左转 */
#define ASR_ID_TURN_RIGHT       4      /**< 右转 */
#define ASR_ID_DANCE            5      /**< 跳个舞 */
#define ASR_ID_STAND            6      /**< 立正 */
#define ASR_ID_LIE              7      /**< 趴下 */
#define ASR_ID_BACKFLIP         8      /**< 来个后空翻 */
#define ASR_ID_SHAKE_HAND       9      /**< 握手 */
#define ASR_ID_STATUS           10     /**< 你还好吗 */

/* 交互命令 (11-20) */
#define ASR_ID_HELLO            11     /**< 打招呼 */
#define ASR_ID_ULTRASONIC_ON    12     /**< 打开超声波避障 */
#define ASR_ID_INFRARED_ON      13     /**< 打开红外模式 */
#define ASR_ID_ULTRASONIC_OFF   14     /**< 关闭超声波避障 */
#define ASR_ID_INFRARED_OFF     15     /**< 关闭红外模式 */

/* 唤醒词ID (50-110) */
#define ASR_WAKEUP_LAODA        51     /**< 唤醒词: 牢大 */
#define ASR_WAKEUP_MANBO        110    /**< 唤醒词: 曼波 */

/* 音乐播放命令 (801-810) */
#define ASR_ID_MUSIC_1          801    /**< 唱首歌 - 蓝莲哈 */
#define ASR_ID_MUSIC_2          802    /**< 换首歌 - 哈基侠 */
#define ASR_ID_MUSIC_3          803    /**< 再换首歌 - 哈基米山歌 */

/* 音量控制命令 (10250-10252) */
#define ASR_VOL_MAX             10250  /**< 最大音量 */
#define ASR_VOL_MID             10251  /**< 中等音量 */
#define ASR_VOL_MIN             10252  /**< 最小音量 */

/* 音频播放ID定义 --------------------------------------------------------------*/
#define AUDIO_ID_HELLO          10500  /**< 蓝莲哈 */
#define AUDIO_ID_HAKIXIA        10501  /**< 哈基侠 */
#define AUDIO_ID_HAKIMI         10502  /**< 哈基米山歌 */

/* 配置参数 --------------------------------------------------------------------*/
#define ASR_SERIAL_BAUDRATE     9600   /**< 串口波特率 */
#define ASR_SERIAL_TIMEOUT      10     /**< 串口超时时间(ms) */
#define ASR_WAKEUP_TIMEOUT      40000  /**< 唤醒超时时间(ms) */
#define ASR_ACTION_DELAY        6000   /**< 动作执行延迟(ms) */
#define ASR_QUEUE_SIZE          5      /**< 消息队列大小 */
#define ASR_DEFAULT_VOLUME      3      /**< 默认音量等级 */

/* GPIO引脚定义 ----------------------------------------------------------------*/
#define ASR_SERIAL_TX_PIN       5      /**< UART TX引脚 */
#define ASR_SERIAL_RX_PIN       6      /**< UART RX引脚 */

/* 类型定义 --------------------------------------------------------------------*/

/**
 * @brief ASR模块状态枚举
 */
typedef enum {
    ASR_STATE_IDLE = 0,        /**< 空闲状态 */
    ASR_STATE_LISTENING,       /**< 监听状态 */
    ASR_STATE_PROCESSING,      /**< 处理状态 */
    ASR_STATE_SENDING,         /**< 发送状态 */
    ASR_STATE_ERROR            /**< 错误状态 */
} ASR_State_t;

/**
 * @brief ASR事件结构体
 */
typedef struct {
    uint32_t snid;             /**< 语音识别ID */
    uint32_t timestamp;        /**< 时间戳 */
} ASR_Event_t;

/* 函数声明 --------------------------------------------------------------------*/

/**
 * @brief  初始化ASR Pro模块
 * @param  None
 * @retval true: 初始化成功, false: 初始化失败
 */
bool ASR_Init(void);

/**
 * @brief  ASR主处理函数，需在主循环中调用
 * @param  None
 * @retval None
 */
void ASR_Process(void);

/**
 * @brief  处理语音识别命令
 * @param  snid: 语音识别ID
 * @retval true: 处理成功, false: 处理失败
 */
bool ASR_HandleCommand(uint32_t snid);

/**
 * @brief  设置音量
 * @param  volume: 音量等级 (0-8)
 * @retval true: 设置成功, false: 设置失败
 */
bool ASR_SetVolume(uint8_t volume);

/**
 * @brief  获取当前状态
 * @param  None
 * @retval ASR模块当前状态
 */
ASR_State_t ASR_GetState(void);

/**
 * @brief  播放音频
 * @param  audio_id: 音频ID
 * @retval true: 播放成功, false: 播放失败
 */
bool ASR_PlayAudio(uint16_t audio_id);

/**
 * @brief  发送命令到STM32
 * @param  cmd: 命令字符
 * @retval true: 发送成功, false: 发送失败
 */
bool ASR_SendCommand(char cmd);

/**
 * @brief  设置唤醒超时时间
 * @param  timeout_ms: 超时时间(毫秒)
 * @retval None
 */
void ASR_SetWakeupTimeout(uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* __ASR_H */
