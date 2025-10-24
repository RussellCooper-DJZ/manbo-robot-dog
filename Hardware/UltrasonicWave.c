#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Movement.h"
//***超声避障***//
extern uint16_t T;  //记录距离
int cf=0;//记录重复次数 解决bug用

uint16_t Time; //记录时间
void UltrasonicWave_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
	/*GPIO初始化*/ 
	GPIO_InitTypeDef GPIO_InitStructure;
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA0 TRIG
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA1 ECHO

	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	
}
void UltrasonicWave_Start(void)
{
	//给触发信号
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	Delay_us(45);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);

	
	Timer_Init();  //计时 返回时间计算距离
}

uint16_t UltrasonicWave_Getvalue(void)
{
	UltrasonicWave_Start();
	Delay_ms(100);
	return ((Time * 0.0001) * 34000) / 2;
}
void Bizhang(void)
{
	T=UltrasonicWave_Getvalue();
	Delay_ms(10);
	if(T>=15)
	{
		{
		move_forward();
		//Delay_ms(10);
		cf++;
		}
	}
	else
	{
		//move_behind();
		T=UltrasonicWave_Getvalue();
	while(T<15)
	{	cf=0;  
		move_right();//一次约30°
		move_right();
		Delay_ms(10);
		T=UltrasonicWave_Getvalue();
		if(T>15)
		{
			break;
		}
	}	
}
}
