/**
  ******************************************************************************
  * @file    commands.cpp
  * @author  Manbo Robot Dog Team
  * @version V1.0
  * @date    2025-10-31
  * @brief   命令映射表实现
  ******************************************************************************
  */

#include "commands.h"
#include "asr.h"
#include <stddef.h>

/* 命令映射表 ------------------------------------------------------------------*/
const CommandMap_t g_CommandMap[] = {
    /* 基本运动命令 */
    {ASR_ID_FORWARD,        CMD_FORWARD,        "前进"},
    {ASR_ID_BACKWARD,       CMD_BACKWARD,       "后退"},
    {ASR_ID_TURN_LEFT,      CMD_TURN_LEFT,      "左转"},
    {ASR_ID_TURN_RIGHT,     CMD_TURN_RIGHT,     "右转"},
    {ASR_ID_DANCE,          CMD_DANCE,          "跳舞"},
    {ASR_ID_STAND,          CMD_STOP,           "立正/停止"},
    {ASR_ID_LIE,            CMD_LIE,            "趴下"},
    
    /* 交互命令 */
    {ASR_ID_SHAKE_HAND,     CMD_SHAKE_HAND,     "握手"},
    {ASR_ID_STATUS,         CMD_STATUS,         "状态查询"},
    {ASR_ID_HELLO,          CMD_HELLO,          "打招呼"},
    
    /* 避障模式控制 */
    {ASR_ID_ULTRASONIC_ON,  CMD_ULTRASONIC_ON,  "打开超声波避障"},
    {ASR_ID_INFRARED_ON,    CMD_INFRARED_ON,    "打开红外模式"},
    {ASR_ID_ULTRASONIC_OFF, CMD_ULTRASONIC_OFF, "关闭超声波避障"},
    {ASR_ID_INFRARED_OFF,   CMD_INFRARED_OFF,   "关闭红外模式"},
};

const uint8_t g_CommandMapSize = sizeof(g_CommandMap) / sizeof(CommandMap_t);

/**
 * @brief  根据语音识别ID获取对应的命令字符
 * @param  snid: 语音识别ID
 * @param  cmd: 输出的命令字符指针
 * @retval true: 找到对应命令, false: 未找到
 */
bool GetCommandBySnid(uint32_t snid, char* cmd)
{
    if (cmd == NULL) {
        return false;
    }
    
    for (uint8_t i = 0; i < g_CommandMapSize; i++) {
        if (g_CommandMap[i].snid == snid) {
            *cmd = g_CommandMap[i].command;
            return true;
        }
    }
    
    return false;
}

/**
 * @brief  获取命令描述
 * @param  snid: 语音识别ID
 * @retval 命令描述字符串，未找到返回NULL
 */
const char* GetCommandDescription(uint32_t snid)
{
    for (uint8_t i = 0; i < g_CommandMapSize; i++) {
        if (g_CommandMap[i].snid == snid) {
            return g_CommandMap[i].desc;
        }
    }
    
    return NULL;
}
