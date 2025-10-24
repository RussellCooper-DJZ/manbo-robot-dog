#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "stm32f10x.h"   

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
void USART2_Init(void);
void USART2_SendString(u8 *DAT, u8 len);
uint8_t USART2_GetRxFlag(void);
uint8_t USART2_GetRxData(void);

#endif


