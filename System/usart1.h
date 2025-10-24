#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "stm32f10x.h"   

#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
void USART1_Init(void);
void USART1_SendByte(uint8_t Byte);
void USART1_SendArray(uint8_t *Array, uint16_t Length);
void USART1_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void USART1_SendNumber(uint32_t Number, uint8_t Length);
int USART1_fputc(int ch, FILE *f);
void USART1_Printf(char *format, ...);

uint8_t USART1_GetRxFlag(void);
uint8_t USART1_GetRxData(void);

#endif


