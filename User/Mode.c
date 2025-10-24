#include "stm32f10x.h"                  // Device header
#include "stdlib.h"
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
#include "stdio.h"
//***����״ִ̬�к���***//
//���ö���ִ��һ�κ�ֹͣ������ĩβ���ϡ�	move_mode = '0';������������������ִ�У�

extern uint8_t move_mode1 ;//���ڽ��������������ݵı���
extern uint8_t move_mode3 ;//���ڽ�������ʶ�𴮿����ݵı���
extern uint8_t move_mode ;//����״̬����
extern uint8_t previous_mode ;//���ڱ�����һ�ν��մ������ݵı���
extern uint16_t bz_flag;//���ϱ�־λ ON/OFF Ĭ��off
extern uint16_t hw_flag;//�����־λ ON/OFF Ĭ��off
extern int happiness; //����ָ��
extern int stamina;  //����ֵ
char read[8]={0};  //�ʶ�����ֵ������ֵ���ַ�������
extern uint16_t ff; //����ǰ������
extern uint16_t bb; //�������˼���
extern uint16_t ll; //������ת����
extern uint16_t rr; //������ת����

/**************�����ϳ�оƬ��������*********************/
//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
//m[0~16]:0��������Ϊ������16���������������
//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
//t[0~5]:0�ʶ�����������5�ʶ��������

void mode_forward(void)//ǰ��
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //ǰ����	
			move_forward();
			LED13_Turn();//��ת��ƽ״̬����1��Ϊ0�����������
			if(previous_mode == 'f')
			{
				ff++;
			}
			else
			{
				ff=0;
			}
			if(ff>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;
	
}
void mode_behind(void)//����
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //ǰ����
			move_behind();
			LED13_Turn();
			if(previous_mode == 'b')
			{
				bb++;
			}
			else
			{
				bb=0;
			}
			if(bb>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;
	
}
void mode_left(void)//��ת
{
			OLED_ShowImage(0, 0, 128, 64, BMP3); //��ת��
			move_left();
			LED13_Turn();
			if(previous_mode == 'l')
			{
				ll++;
			}
			else
			{
				ll=0;
			}
			if(ll>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;
	
	
}
void mode_right(void)//��ת
{
			OLED_ShowImage(0, 0, 128, 64, BMP4); //��ת��
			move_right();
			LED13_Turn();
			if(previous_mode == 'r')
			{
				rr++;
			}
			else
			{
				rr=0;
			}
			if(rr>=5)
			{
				happiness -=3;
			}
			previous_mode = move_mode;
			stamina -=5;
}

void mode_swing_qianhou(void)//ǰ��ҡ��
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //�Ժ���
			move_shake_qianhou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}
void mode_swing_zuoyou(void)//����ҡ��
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //�Ժ���
			move_shake_zuoyou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}

void mode_dance(void)//����
{
			OLED_ShowImage(0, 0, 128, 64, BMP5); //������
			move_dance();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
	
}
void mode_stand(void)//����
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_stand();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]����");
			Delay_s(1);
			stamina -=1;
	
	
}
void mode_slowstand(void)//����
{
			if (move_mode3 == 'q'){//ֻ����ͨ�������������Żᴥ�������ϳ�
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]���Ҹ���");Delay_s(2);
			}
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_slow_stand(previous_mode);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=1;
}

void mode_strech(void)//���²���
{
			if (move_mode3 == 's'){//ֻ����ͨ�������������Żᴥ�������ϳ�
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]����");Delay_ms(500);
			}
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP2);//ǰ����
			move_stretch();
			OLED_ShowImage(0, 0, 128, 64, BMP12);//èè��
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=2;
}
void mode_hello(void)//���к�
{
			if (previous_mode != '5' && previous_mode != 'D') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //������
				move_slow_stand(previous_mode);
			}
	OLED_ShowImage(0, 0, 128, 64, BMP12);//èè��
			//move_hello();
		int i;
	for(i=0;i<20;i++)//
	{
	Servo_SetAngle1(90-i);
	Servo_SetAngle2(90+i);
	//Servo_SetAngle3(90+i);
	Servo_SetAngle4(90-i);
		Delay_ms(7);
	}
	for(i=0;i<40;i++)//
	{
	Servo_SetAngle2(110+i);
	Servo_SetAngle4(70-i);
		Delay_ms(7);
	}
	for(i=0;i<60;i++)//��ǰ�㻺��̧��
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	Delay_ms(50);
	//������ҡ������ǰ��
	SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5] ���ѽ");
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(180);
	Delay_ms(400);
	Servo_SetAngle1(130);
	Delay_ms(400);
	Servo_SetAngle1(70);
	Delay_ms(500);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=2;
			happiness +=2;
}
void mode_twohands(void)//����̧��
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_stand();
			move_two_hands();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;
			happiness +=2;
	
}
void mode_lanyao(void)//����
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP9);//������
			lan_yao();
			OLED_ShowImage(0, 0, 128, 64, BMP1);//������
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=1;
	
}
void mode_headup(void)//̧ͷ
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //������
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP10);//��Ƥ��
			move_head_up();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;
	
}
void mode_sleeppa(void)//ſ��˯��
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //������
				move_slow_stand(previous_mode);
			}
			if (rand()%2) {//����������ֱ����е�һ��
				OLED_ShowImage(0, 0, 128, 64, BMP6); //��ͨ˯����
			}
			else{
				OLED_ShowImage(0, 0, 128, 64, BMP8); //��˯��
			}
			move_sleep_p();
			SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��");Delay_s(1);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_sleepwo(void)//����˯��
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //������
				move_slow_stand(previous_mode);
				Delay_s(1);
			}
			if (rand()%2) {//����������ֱ����е�һ��
				OLED_ShowImage(0, 0, 128, 64, BMP6); //��ͨ˯����
			}
			else{
				OLED_ShowImage(0, 0, 128, 64 , BMP8); //��˯��
			}
			move_sleep_w();
			SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��");Delay_s(1);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_nanshou(void)//����
{
	OLED_ShowImage(0, 0, 128, 64, BMP2); //ǰ����	
	move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]����");Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_biaobai(void)//���
{
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love������	
	Servo_SetAngle1(135);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]���Ǻ����������������������������ε�����");Delay_s(8);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //�����Ժ���	
	Servo_SetAngle3(45);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]ӵ���뻳�����Ǵ������");Delay_s(5);
	Servo_SetAngle1(90);//
	Servo_SetAngle3(90);//�ջ���ֻǰ��
	OLED_ShowImage(0, 0, 128, 64, BMP9); //������	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]�����������������ﻨ��");Delay_s(5);
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love������
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]����ģ�С��");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(5);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=5;
}
void mode_yuansu(void)//Ԫ�����ڱ�
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //�ٺ���	
	Servo_SetAngle1(135);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]�⺤�����");Delay_s(2);Delay_ms(500);
	OLED_ShowImage(0, 0, 128, 64, BMP10); //��Ƥ��	
	Servo_SetAngle3(45);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]̼��������");Delay_s(2);Delay_ms(500);
	Servo_SetAngle2(45);//�Һ�����̧
	Servo_SetAngle4(135);//�������̧
	OLED_ShowImage(0, 0, 128, 64, BMP12); //èè��	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��þ������");Delay_s(2);Delay_ms(500);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //�����Ժ���
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5] С����ɰ�");
	move_shake_qianhou();
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=2;
}
void mode_xiaoxun(void)//Уѵ
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //�ٺ���	
	Servo_SetAngle1(135);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]������ѧ");Delay_s(2);//�����ָպ�1s
	OLED_ShowImage(0, 0, 128, 64, BMP10); //��Ƥ��	
	Servo_SetAngle3(45);//̧����ǰ��
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]������־");Delay_s(2);
	Servo_SetAngle2(45);//�Һ�����̧
	Servo_SetAngle4(135);//�������̧
	OLED_ShowImage(0, 0, 128, 64, BMP12); //èè��	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��������");Delay_s(2);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //�����Ժ���
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��ˡ����");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
}
void mode_world(void)//����֮��
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //�ٺ���	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]Ϊ����֮��");
	Servo_SetAngle2(135);//�Һ����ǰ̧
	Servo_SetAngle4(45);//������ǰ̧
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_xiaodai(void)//��������С��
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //������
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]����");
	Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_happiness(void)//OLEDչʾ����ֵ
{
	OLED_ShowChinese(0,0,"����ֵ��");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_happiness(void)//չʾ����ֵ
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //������
	OLED_happiness();
	sprintf(read,"[v12][m0][t5]��ǰ�Ŀ���ֵΪ%d",happiness);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(happiness>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //������
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��������һ���濪��");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(4);
	}
	else if(happiness<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��С����һ���");
		if (rand()%2) //����������ֶ����е�һ��
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(3);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_stamina(void)//OLEDչʾ����ֵ
{
	OLED_ShowChinese(0,0,"����ֵ��");
	if(stamina==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_stamina(void)//չʾ����ֵ
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //������
	OLED_stamina();
	sprintf(read,"[v12][m0][t5]��ǰ������ֵΪ%d",stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(stamina>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //������
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]С����������������һ�����");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(6);
	}
	else if(stamina<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]��С����Ϣһ�°�");
		if (rand()%2) //����������ֶ����е�һ��
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(4);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_index(void)//չʾ����ֵ������ֵ
{
	OLED_Clear();
	OLED_ShowChinese(0,0,"����ֵ��");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_ShowChinese(0,16,"����ֵ��");
	if(stamina==100)
	{OLED_ShowNum(64,16,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,16,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,32);
	sprintf(read,"[v12][m0][t5]��ǰ�Ŀ���ֵΪ%d������ֵΪ%d",happiness,stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(10);
	previous_mode = move_mode;
	move_mode = '0';
}

