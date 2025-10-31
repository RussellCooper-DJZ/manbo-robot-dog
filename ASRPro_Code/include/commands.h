/**
  ******************************************************************************
  * @file    commands.h
  * @author  Manbo Robot Dog Team
  * @version V1.0
  * @date    2025-10-31
  * @brief   ESP32与STM32通信命令定义
  *          定义了通过UART发送给STM32的单字符命令
  ******************************************************************************
  */

#ifndef __COMMANDS_H
#define __COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

/* 运动控制命令 ----------------------------------------------------------------*/
#define CMD_FORWARD         'f'    /**< 前进 */
#define CMD_BACKWARD        'b'    /**< 后退 */
#define CMD_TURN_LEFT       'l'    /**< 左转 */
#define CMD_TURN_RIGHT      'r'    /**< 右转 */
#define CMD_STOP            '5'    /**< 停止 */

/* 姿态控制命令 ----------------------------------------------------------------*/
#define CMD_STAND           '1'    /**< 站立 */
#define CMD_SIT             '3'    /**< 坐下 */
#define CMD_LIE             'p'    /**< 趴下 */
#define CMD_DANCE           'd'    /**< 跳舞 */

/* 动作命令 --------------------------------------------------------------------*/
#define CMD_SHAKE_HAND      's'    /**< 握手 */
#define CMD_HELLO           'o'    /**< 打招呼 */
#define CMD_STATUS          'Z'    /**< 状态查询 */

/* 避障模式命令 ----------------------------------------------------------------*/
#define CMD_ULTRASONIC_ON   'x'    /**< 打开超声波避障 */
#define CMD_ULTRASONIC_OFF  'c'    /**< 关闭超声波避障 */
#define CMD_INFRARED_ON     'H'    /**< 打开红外模式 */
#define CMD_INFRARED_OFF    'h'    /**< 关闭红外模式 */

/* 命令映射表结构 --------------------------------------------------------------*/
typedef struct {
    uint32_t snid;        /**< 语音识别ID */
    char command;         /**< 对应的命令字符 */
    const char* desc;     /**< 命令描述 */
} CommandMap_t;

/* 命令映射表声明 --------------------------------------------------------------*/
extern const CommandMap_t g_CommandMap[];
extern const uint8_t g_CommandMapSize;

/**
 * @brief  根据语音识别ID获取对应的命令字符
 * @param  snid: 语音识别ID
 * @param  cmd: 输出的命令字符指针
 * @retval true: 找到对应命令, false: 未找到
 */
bool GetCommandBySnid(uint32_t snid, char* cmd);

/**
 * @brief  获取命令描述
 * @param  snid: 语音识别ID
 * @retval 命令描述字符串，未找到返回NULL
 */
const char* GetCommandDescription(uint32_t snid);

#ifdef __cplusplus
}
#endif

#endif /* __COMMANDS_H */
