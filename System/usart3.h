#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "stm32f10x.h"   

#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
void USART3_Init(void);

uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);

#endif


