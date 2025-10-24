#include "stm32f10x.h"                  // Device header
#include "Servo.h"
#include "PWM.h"
#include "Delay.h"
#include "stdlib.h"
//PWM、Servo、Movement三个文件共同为驱动舵机服务
//Movement用于存储具体的动作设计
int i,j;
int movedelay=150;//改变这里来改变动作间隔
uint8_t angle_1, angle_2, angle_3, angle_4;
	
void move_stand(void){//站立
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(500);
}

void move_forward(void){ //前进

	Servo_SetAngle1(135);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(45);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(135);
	Servo_SetAngle3(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(45);
	Servo_SetAngle4(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
}

void move_behind(void){//后退
	
	Servo_SetAngle1(45);
	Servo_SetAngle4(135);
	Delay_ms(movedelay);
		
	Servo_SetAngle2(135);
	Servo_SetAngle3(45);
	Delay_ms(movedelay);
		
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);

	Servo_SetAngle2(45);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
		
	Servo_SetAngle1(135);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
}

void move_right(void){ //右转
	Servo_SetAngle1(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(135);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
}
void move_right_hw(void){ //右转
	Servo_SetAngle1(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(135);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
//	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
}

void move_left(void) //左转
{
	Servo_SetAngle2(135);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
}
void move_left_hw(void) //左转
{
	Servo_SetAngle2(135);
	Servo_SetAngle3(135);
	Delay_ms(movedelay);
	
	Servo_SetAngle1(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
//	Delay_ms(movedelay);
	
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
}
void move_hello(void)//打招呼
{
	for(i=0;i<20;i++)//
	{
	Servo_SetAngle1(90-i);
	Servo_SetAngle2(90+i);
	//Servo_SetAngle3(90+i);
	Servo_SetAngle4(90-i);
		Delay_ms(4);
	}
	for(i=0;i<60;i++)//
	{
	Servo_SetAngle2(110+i);
	Servo_SetAngle4(70-i);
		Delay_ms(4);
	}
	for(i=0;i<60;i++)//右前足缓慢抬起
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	Delay_ms(50);
	//下面是摇两次右前足
	Servo_SetAngle1(180);
	Delay_ms(500);
	Servo_SetAngle1(130);
	Delay_ms(500);
	Servo_SetAngle1(180);
	Delay_ms(500);
	Servo_SetAngle1(130);
	Delay_ms(500);
	Servo_SetAngle1(70);
	Delay_ms(500);
}

void move_shake_qianhou(void)//前后摇摆
{  
	Servo_SetAngle2(135);
	Servo_SetAngle1(135);
	Servo_SetAngle3(45);
	Servo_SetAngle4(45);
	Delay_ms(150);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(150);
	Servo_SetAngle2(45);
	Servo_SetAngle1(45);
	Servo_SetAngle3(135);
	Servo_SetAngle4(135);
	Delay_ms(150);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(150);
	
	
}
void move_shake_zuoyou(void)//左右摇摆
{
	Servo_SetAngle1(135);
	Servo_SetAngle2(135);
	Delay_ms(movedelay);
	Servo_SetAngle3(135);
	Servo_SetAngle4(135);
	Delay_ms(movedelay);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Delay_ms(movedelay);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	Servo_SetAngle3(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	Servo_SetAngle1(45);
	Servo_SetAngle2(45);
	Delay_ms(movedelay);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Delay_ms(movedelay);
}
void move_dance(void)//跳舞
{
	Servo_SetAngle1(135);
	Servo_SetAngle2(135);
	Delay_ms(movedelay);
	Servo_SetAngle3(135);
	Servo_SetAngle4(135);
	Delay_ms(movedelay);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Delay_ms(movedelay);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(135);
	Servo_SetAngle1(135);
	Servo_SetAngle3(45);
	Servo_SetAngle4(45);
	Delay_ms(150);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(150);
	
	
	Servo_SetAngle3(45);
	Servo_SetAngle4(45);
	Delay_ms(movedelay);
	Servo_SetAngle1(45);
	Servo_SetAngle2(45);
	Delay_ms(movedelay);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(movedelay);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Delay_ms(movedelay);
	
	Servo_SetAngle2(45);
	Servo_SetAngle1(45);
	Servo_SetAngle3(135);
	Servo_SetAngle4(135);
	Delay_ms(150);
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	Servo_SetAngle3(90);
	Servo_SetAngle4(90);
	Delay_ms(150);
	
}
void move_head_up(void)//抬头
{
	for(i=0;i<20;i++)
	{
		Servo_SetAngle1(90-i);
		Servo_SetAngle3(90+i);
		Delay_ms(10);
	}
	for(i=0;i<65;i++)
	{
		Servo_SetAngle2(90+i);
		Servo_SetAngle4(90-i);
		Delay_ms(10);
	}
}
void move(uint16_t set1 , uint16_t set2 , uint16_t set3 , uint16_t set4 , uint16_t speed){
	angle_1 = Servo_GetAngle1();
	angle_2 = Servo_GetAngle2();
	angle_3 = Servo_GetAngle3();
	angle_4 = Servo_GetAngle4();
	while (angle_1 != set1 || angle_3 != set3 || angle_2 != set2 || angle_4 != set4) {
		if (angle_1 > set1) {
			--angle_1;
			Servo_SetAngle1(angle_1);
		} else if (angle_1 < set1) {
			++angle_1;
			Servo_SetAngle1(angle_1);
		}
		//
		if (angle_3 > set3) {
			--angle_3;
			Servo_SetAngle3(angle_3);
		} else if (angle_3 < set3) {
			++angle_3;
			Servo_SetAngle3(angle_3);
		}
		//
		if (angle_2 > set2) {
			--angle_2;
			Servo_SetAngle2(angle_2);
		} else if (angle_2 < set2) {
			++angle_2;
			Servo_SetAngle2(angle_2);
		}
		//
		if (angle_4 > set4) {
			--angle_4;
			Servo_SetAngle4(angle_4);
		} else if (angle_4 < set4) {
			++angle_4;
			Servo_SetAngle4(angle_4);
		}
		Delay_ms(1000/speed);
	}
}
void move_slow_stand(uint8_t previous_mode){//缓慢起身
	if (previous_mode == '0')
		return;
	angle_1 = Servo_GetAngle1();
	angle_2 = Servo_GetAngle2();
	angle_3 = Servo_GetAngle3();
	angle_4 = Servo_GetAngle4();
	while (angle_1 != 90 || angle_3 != 90 || angle_2 != 90 || angle_4 != 90) {
		if (angle_1 > 90) {
			--angle_1;
			Servo_SetAngle1(angle_1);
		} else if (angle_1 < 90) {
			++angle_1;
			Servo_SetAngle1(angle_1);
		}
		//
		if (angle_3 > 90) {
			--angle_3;
			Servo_SetAngle3(angle_3);
		} else if (angle_3 < 90) {
			++angle_3;
			Servo_SetAngle3(angle_3);
		}
		//
		if (angle_2 > 90) {
			--angle_2;
			Servo_SetAngle2(angle_2);
		} else if (angle_2 < 90) {
			++angle_2;
			Servo_SetAngle2(angle_2);
		}
		//
		if (angle_4 > 90) {
			--angle_4;
			Servo_SetAngle4(angle_4);
		} else if (angle_4 < 90) {
			++angle_4;
			Servo_SetAngle4(angle_4);
		}
		Delay_ms(10);
	}
}
void move_stretch(void){//坐下擦脸
	  for(i=0;i<65;i++)//后两足缓慢前抬
	{
    Servo_SetAngle2(90+i);
	Servo_SetAngle4(90-i);
	Delay_ms(5);
	}
	  for(i=0;i<20;i++)
	{
  Servo_SetAngle1(90-i);
	Servo_SetAngle3(90+i);
	Delay_ms(5);
	}
	Delay_ms(1000);
	for(i=0;i<60;i++)//右前足缓慢抬起
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	Delay_ms(1000);
	//下面是摇两次右前足
	Servo_SetAngle1(180);
	Delay_ms(500);
	Servo_SetAngle1(130);
	Delay_ms(500);
	Servo_SetAngle1(180);
	Delay_ms(500);
	Servo_SetAngle1(130);
	Delay_ms(500);
	Servo_SetAngle1(70);
	Delay_ms(5);//1:70;2:155;3:110;4:25
}
void move_two_hands(void){//交替抬手
	Servo_SetAngle3(20);
	Servo_SetAngle2(20);
	Delay_ms(200);
	Servo_SetAngle3(90);
	Servo_SetAngle2(90);
	Delay_ms(200);
	Servo_SetAngle1(160);
	Servo_SetAngle4(160);
	Delay_ms(200);
	Servo_SetAngle1(90);
	Servo_SetAngle4(90);
	Delay_ms(200);
}
void lan_yao(void){//伸懒腰
  for(i=0;i<75;i++)
	{
	Servo_SetAngle1(90+i);
	Servo_SetAngle3(90-i);
	Servo_SetAngle2(90+i/2);
	Servo_SetAngle4(90-i/2);
	Delay_ms(5);
	}
	Delay_ms(movedelay*50);
	for(i=0;i<75;i++)
	{
	Servo_SetAngle1(165-i);
	Servo_SetAngle3(15+i);
	Servo_SetAngle2(127-i/2);
	Servo_SetAngle4(53+i/2);
	Delay_ms(5);
	}
	Delay_ms(movedelay);
}
void move_sleep_p(void) {//趴下睡觉
	for(i=0;i<75;i++)
	{
		Servo_SetAngle1(90+i);
		Servo_SetAngle3(90-i);
		Delay_ms(10);
	}
	for(i=0;i<75;i++)
	{
		Servo_SetAngle4(90+i);
		Servo_SetAngle2(90-i);
		Delay_ms(10);
	}
}
void move_sleep_w(void) {//卧下睡觉
		for(i=0;i<75;i++)
	{
		Servo_SetAngle3(90+i);
		Servo_SetAngle1(90-i);
		Delay_ms(10);
	}
	for(i=0;i<75;i++)
	{
		Servo_SetAngle2(90+i);
		Servo_SetAngle4(90-i);
		Delay_ms(10);
	}
}

