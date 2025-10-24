#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte1(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString1(char *String);
//void Serial_SendString2(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag1(void);
uint8_t Serial_GetRxData1(void);
uint8_t Serial_GetRxFlag2(void);
uint8_t Serial_GetRxData2(void);
uint8_t Serial_GetRxFlag3(void);
uint8_t Serial_GetRxData3(void);

#endif
