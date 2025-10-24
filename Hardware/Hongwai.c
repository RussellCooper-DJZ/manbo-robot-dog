#include "stm32f10x.h"                  // Device header
#include "Movement.h"
#include "Delay.h"
//***�������¼��***//
extern uint8_t t1 ; //�洢������ź� ǰ
extern uint8_t t2 ; //�洢������ź� ��
extern uint8_t t3 ; //�洢������ź� ��
extern uint8_t t4 ; //�洢������ź� ��
void hongwai_init(void) //�����ʼ�� ���������������� 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/*GPIO��ʼ��*/                                        
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//�ͱ�Ե������ IPD��ʾ��̬Ϊ0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
}

uint8_t hongwai_get_1(void) //ǰ
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
}
uint8_t hongwai_get_2(void)//��
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
}
uint8_t hongwai_get_3(void)//��
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
}
uint8_t hongwai_get_4(void)//��
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
}
void Edge_detect(void)
{
		t1=hongwai_get_1();
		t2=hongwai_get_2();
		t3=hongwai_get_3();
		t4=hongwai_get_4();
		if(t1==1)
		{
			move_behind();

			t1=0;
			
		}

		if(t2==1)
		{
			move_forward();


			t2=0;
		}


		if(t3==1)
		{
		{
			move_right_hw();
            move_forward();
			move_right_hw();
			move_forward();

			t3=0;
		}
		}
		if(t4==1)
		{
		{
			move_left_hw();
            move_forward();
			move_left_hw();
			move_forward();

			t4=0;
		}
		}
	}
