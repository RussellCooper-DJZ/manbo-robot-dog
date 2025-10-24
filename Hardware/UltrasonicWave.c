#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Movement.h"
//***��������***//
extern uint16_t T;  //��¼����
int cf=0;//��¼�ظ����� ���bug��

uint16_t Time; //��¼ʱ��
void UltrasonicWave_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	/*GPIO��ʼ��*/ 
	GPIO_InitTypeDef GPIO_InitStructure;
	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA0 TRIG
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA1 ECHO

	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	
}
void UltrasonicWave_Start(void)
{
	//�������ź�
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	Delay_us(45);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);

	
	Timer_Init();  //��ʱ ����ʱ��������
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
		move_right();//һ��Լ30��
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
