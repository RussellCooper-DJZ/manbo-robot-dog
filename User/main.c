#include "stm32f10x.h"                  // Device header
#include "stdlib.h"
#include <stdio.h>
#include "LED.h"
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "PWM.h"
#include "Movement.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "syn6288.h"
#include "Hongwai.h"
#include "UltrasonicWave.h"
#include "Mode.h"

/****************************************************
*
*�������ƣ�����������С����
*��ǰ�汾��V1.0-2024/6/9
*
*****************************************************
*/

uint8_t RxData;			//���ڽ��մ������ݵı���
uint8_t move_mode1 = '0';//���ڽ��������������ݵı���
uint8_t move_mode3 = '0';//���ڽ�������ʶ�𴮿����ݵı���
uint8_t move_mode = '0';//����״̬����
uint8_t previous_mode = '0';//���ڱ�����һ�ν��մ������ݵı���
 uint16_t T=100; // ��¼���� ����ֵ����10cm(����������) 
uint16_t bz_flag;//���ϱ�־λ ON/OFF Ĭ��off
uint16_t hw_flag;//�����־λ ON/OFF Ĭ��off
int happiness; //����ָ��
int stamina;  //����ֵ
uint8_t t1=0 ; //�洢������ź� ǰ
uint8_t t2=0 ; //�洢������ź� ��
uint8_t t3=0 ; //�洢������ź� ��
uint8_t t4=0 ; //�洢������ź� ��
uint16_t ff=0; //����ǰ������
uint16_t bb=0; //�������˼���
uint16_t ll=0; //������ת����
uint16_t rr=0; //������ת����

int main(void)
{
	/*ģ���ʼ��*/
	LED_Init();			//LED��ʼ��
	OLED_Init();		//OLED��ʼ��
	USART1_Init();		//�������ڳ�ʼ��
	USART2_Init();		//�����ϳɴ��ڳ�ʼ��
	USART3_Init();		//����ʶ�𴮿ڳ�ʼ��
	Servo_Init();		//�����ʼ��
	hongwai_init();     //�����ʼ��
	UltrasonicWave_Init(); //��������ʼ��
	happiness=200;//��ʼ������ָ��
	stamina=500;//��ʼ������ֵ
	bz_flag=0;//���ϱ�־λ ON/OFF Ĭ��off
	hw_flag=0;//�����־λ ON/OFF Ĭ��off
	/*����Ĭ��״̬*/
	OLED_Clear();
	OLED_ShowImage(0, 0, 128, 64, BMP1); //����
 
/**************�����ϳ�оƬ��������*********************/
//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
//m[0~16]:0��������Ϊ������16���������������
//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
//t[0~5]:0�ʶ�����������5�ʶ��������

	SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]����");Delay_s(1);//���Ѻ󹷽�һ��,����12�ʺ���ʾ��10�ʺϵ���
	
	while (1)
	{  
		//***����ֵ�뿪��ֵ��Ӧ����***//
		if(stamina>0)//����ֵ������Ż�����
	{
		move_mode1 = USART1_GetRxData();		//��ȡ�������ڽ��յ�����
		move_mode3 = USART3_GetRxData();		//��ȡ����ʶ�𴮿ڽ��յ�����
		if (USART1_GetRxFlag()) {//���������������ȼ���������ʶ��
			move_mode = move_mode1;
		}
		else if (USART3_GetRxFlag()){
			move_mode = move_mode3;
		}
	}
		if(stamina<0)//����ֵС��0��ִ�С����ܡ�״̬
	{
		mode_nanshou();
		stamina=0;
	}
		if(stamina==0)//����ֵΪ0��ֻ����˯��ָ��
	{
		if(USART1_GetRxData()=='2'|USART3_GetRxData()=='2')
		{mode_sleepwo();}
		else if(USART1_GetRxData()=='p'|USART3_GetRxData()=='p')
		{mode_sleeppa();}
	}
		if(stamina>500)//����ֵ������1000
		{stamina=500;}
		
		if(happiness<0){//����ֵС��0��ִ�С����ܡ�״̬���������ӿ���ֵ��ָ����������״̬
		happiness=0;
		mode_nanshou();
		}
		if(happiness>200)//����ֵ������1000
		{happiness=200;}
		
		//***�������¼��ģ������***//
		if(move_mode == 'H') //���⿪
		{hw_flag=1;}
		if(move_mode == 'h') //�����
		{hw_flag=0;
			//���¸��ĸ��������������
			t1=0;t2=0;t3=0;t4=0;
		}
		if(hw_flag){
			Edge_detect();//�򿪺������¼��ģʽ	
		}
		
	   //***��������ģ������***//
		if(move_mode == 'x') //������
		{
			bz_flag=1;
		}
		if(move_mode == 'c') //������
		{
			bz_flag=0;
			T=100;
		}
		if(bz_flag){
		Bizhang();//�򿪳�������ģʽ
		}
		
		
		//***��������***//
		if (move_mode == 'f') { //ǰ��
			mode_forward();
		} else if (move_mode == 'b') { //����
			mode_behind();
		} else if (move_mode == 'l') { //��ת
			mode_left();
		} else if (move_mode == 'r') { //��ת
			mode_right();
		} else if (move_mode == 'w') { //ǰ��ҡ��
			mode_swing_qianhou();
		} else if (move_mode == 'z') { //����ҡ��
			mode_swing_zuoyou();
		} else if (move_mode == 'd') { //����
			mode_dance();
		} else if (move_mode == '5') { //����
			mode_stand();
		} else if (move_mode == 'q' && previous_mode != '0') { //����
			mode_slowstand();
		} else if (move_mode == 's' && previous_mode != 's') { //����
			mode_strech();
		} else if (move_mode == 'j') { //����̧��
			mode_twohands();
		} else if (move_mode == 'y') { //������
			mode_lanyao();
		} else if (move_mode == '1') { //̧ͷ
			mode_headup();
		} else if (move_mode == 'p' && previous_mode != 'p') { //ſ��˯��
			mode_sleeppa();
		} else if (move_mode == '2' && previous_mode != '2') { //����˯��
			mode_sleepwo();
		} else if (move_mode == 'n') { //����
			mode_nanshou();
		} else if (move_mode == 'B') { //���
			mode_biaobai();
		} else if (move_mode == 'Y') { //��Ԫ�����ڱ�
			mode_yuansu();
		} else if (move_mode == 'X') { //��Уѵ
			mode_xiaoxun();
		} else if (move_mode == 'W') { //Ϊ����֮��
			mode_world();
		} else if (move_mode == 'o') { //���к�
			mode_hello();
		} else if (move_mode == 'U') { //���С�С����
			mode_xiaodai();
		} else if (move_mode == 'K') { //չʾ����ֵ
			mode_happiness();
		} else if (move_mode == 'T') { //չʾ����ֵ
			mode_stamina();
		} else if (move_mode == 'Z') { //չʾ����ֵ������ֵ
			mode_index();
		} 


	}
}
