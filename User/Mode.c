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
//***各种状态执行函数***//
//想让动作执行一次后停止，就在末尾加上“	move_mode = '0';”，否则动作持续反复执行，

extern uint8_t move_mode1 ;//用于接收蓝牙串口数据的变量
extern uint8_t move_mode3 ;//用于接收语音识别串口数据的变量
extern uint8_t move_mode ;//决定状态变量
extern uint8_t previous_mode ;//用于保存上一次接收串口数据的变量
extern uint16_t bz_flag;//避障标志位 ON/OFF 默认off
extern uint16_t hw_flag;//红外标志位 ON/OFF 默认off
extern int happiness; //开心指数
extern int stamina;  //体力值
char read[8]={0};  //朗读开心值或体力值的字符串变量
extern uint16_t ff; //连续前进计数
extern uint16_t bb; //连续后退计数
extern uint16_t ll; //连续左转计数
extern uint16_t rr; //连续右转计数

/**************语音合成芯片设置命令*********************/
//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//m[0~16]:0背景音乐为静音，16背景音乐音量最大
//v[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快

void mode_forward(void)//前进
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸	
			move_forward();
			LED13_Turn();//翻转电平状态，由1变为0，灯由灭变亮
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
void mode_behind(void)//后退
{
			OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸
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
void mode_left(void)//左转
{
			OLED_ShowImage(0, 0, 128, 64, BMP3); //左转脸
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
void mode_right(void)//右转
{
			OLED_ShowImage(0, 0, 128, 64, BMP4); //右转脸
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

void mode_swing_qianhou(void)//前后摇摆
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
			move_shake_qianhou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}
void mode_swing_zuoyou(void)//左右摇摆
{
			OLED_ShowImage(0, 0, 128, 64, BMP11); //迷糊脸
			move_shake_zuoyou();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
}

void mode_dance(void)//跳舞
{
			OLED_ShowImage(0, 0, 128, 64, BMP5); //特殊脸
			move_dance();
			LED13_Turn();
			previous_mode = move_mode;
			stamina -=5;
			happiness +=5;
	
}
void mode_stand(void)//立正
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_stand();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]旺旺");
			Delay_s(1);
			stamina -=1;
	
	
}
void mode_slowstand(void)//起身
{
			if (move_mode3 == 'q'){//只有是通过语音交互，才会触发语音合成
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]叫我干嘛");Delay_s(2);
			}
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=1;
}

void mode_strech(void)//坐下擦脸
{
			if (move_mode3 == 's'){//只有是通过语音交互，才会触发语音合成
			SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]遵命");Delay_ms(500);
			}
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP2);//前进脸
			move_stretch();
			OLED_ShowImage(0, 0, 128, 64, BMP12);//猫猫脸
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=2;
}
void mode_hello(void)//打招呼
{
			if (previous_mode != '5' && previous_mode != 'D') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
			}
	OLED_ShowImage(0, 0, 128, 64, BMP12);//猫猫脸
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
	for(i=0;i<60;i++)//右前足缓慢抬起
	{
		Servo_SetAngle1(70+i);
		Delay_ms(4);
	}
	Delay_ms(50);
	//下面是摇两次右前足
	SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5] 哈喽呀");
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
void mode_twohands(void)//交替抬手
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_stand();
			move_two_hands();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;
			happiness +=2;
	
}
void mode_lanyao(void)//懒腰
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP9);//开心脸
			lan_yao();
			OLED_ShowImage(0, 0, 128, 64, BMP1);//立正脸
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina +=5;
			happiness +=1;
	
}
void mode_headup(void)//抬头
{
			OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
			move_slow_stand(previous_mode);
			OLED_ShowImage(0, 0, 128, 64, BMP10);//调皮脸
			move_head_up();
			LED13_Turn();
			previous_mode = move_mode;
			move_mode = '0';
			stamina -=5;
	
}
void mode_sleeppa(void)//趴下睡觉
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
			}
			if (rand()%2) {//随机产生两种表情中的一种
				OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
			}
			else{
				OLED_ShowImage(0, 0, 128, 64, BMP8); //酣睡脸
			}
			move_sleep_p();
			SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]晚安");Delay_s(1);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_sleepwo(void)//卧下睡觉
{
			if (previous_mode != '5' && previous_mode != 'q') {
				OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
				move_slow_stand(previous_mode);
				Delay_s(1);
			}
			if (rand()%2) {//随机产生两种表情中的一种
				OLED_ShowImage(0, 0, 128, 64, BMP6); //普通睡觉脸
			}
			else{
				OLED_ShowImage(0, 0, 128, 64 , BMP8); //酣睡脸
			}
			move_sleep_w();
			SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]晚安");Delay_s(1);
			previous_mode = move_mode;
			move_mode = '0';
			stamina =100;
			happiness +=15;
}
void mode_nanshou(void)//难受
{
	OLED_ShowImage(0, 0, 128, 64, BMP2); //前进脸	
	move_sleep_w();
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]呜呜");Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_biaobai(void)//表白
{
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]你是杭州西湖的美景，是无需增添修饰的烂漫");Delay_s(8);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]拥你入怀，就是春风和煦");Delay_s(5);
	Servo_SetAngle1(90);//
	Servo_SetAngle3(90);//收回两只前脚
	OLED_ShowImage(0, 0, 128, 64, BMP9); //开心脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]望你眼眸，便是鸟语花香");Delay_s(5);
	OLED_ShowImage(0, 0, 128, 64, BMP7); //love花痴脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]爱你的，小呆");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(5);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=5;
}
void mode_yuansu(void)//元素周期表
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]氢氦锂铍硼");Delay_s(2);Delay_ms(500);
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]碳氮氧氟氖");Delay_s(2);Delay_ms(500);
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]钠镁铝硅磷");Delay_s(2);Delay_ms(500);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5] 小呆最可爱");
	move_shake_qianhou();
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
	happiness +=2;
}
void mode_xiaoxun(void)//校训
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	Servo_SetAngle1(135);//抬起右前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]精勤求学");Delay_s(2);//两个字刚好1s
	OLED_ShowImage(0, 0, 128, 64, BMP10); //调皮脸	
	Servo_SetAngle3(45);//抬起左前脚
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]敦笃励志");Delay_s(2);
	Servo_SetAngle2(45);//右后脚向后抬
	Servo_SetAngle4(135);//左后脚向后抬
	OLED_ShowImage(0, 0, 128, 64, BMP12); //猫猫脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]果毅力行");Delay_s(2);
	OLED_ShowImage(0, 0, 128, 64, BMP11); //开心迷糊脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]忠恕任事");
	move_shake_qianhou();
	move_shake_qianhou();
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
	stamina -=2;
}
void mode_world(void)//世界之光
{
	OLED_ShowImage(0, 0, 128, 64, BMP5); //嘿嘿脸	
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]为世界之光");
	Servo_SetAngle2(135);//右后脚向前抬
	Servo_SetAngle4(45);//左后脚向前抬
	Delay_s(2);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_xiaodai(void)//语音唤醒小呆
{
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正脸
	SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]我在");
	Delay_s(1);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_happiness(void)//OLED展示开心值
{
	OLED_ShowChinese(0,0,"开心值，");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_happiness(void)//展示开心值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_happiness();
	sprintf(read,"[v12][m0][t5]当前的开心值为%d",happiness);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(happiness>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]和主人在一起真开心");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(4);
	}
	else if(happiness<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_happiness();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]陪小呆玩一会叭");
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(3);
	previous_mode = move_mode;
	move_mode = '0';
}
void OLED_stamina(void)//OLED展示体力值
{
	OLED_ShowChinese(0,0,"体力值，");
	if(stamina==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,16);
}
void mode_stamina(void)//展示体力值
{
	OLED_ShowImage(0, 8, 128, 48, BMP1); //立正脸
	OLED_stamina();
	sprintf(read,"[v12][m0][t5]当前的体力值为%d",stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(4);
	if(stamina>=60){
		OLED_ShowImage(0, 8, 128, 48, BMP9); //开心脸
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]小呆精力满满，和我一起玩叭");
		move_shake_qianhou();
		move_shake_qianhou();Delay_s(6);
	}
	else if(stamina<=10){
		OLED_ShowImage(0, 8, 128, 48, BMP2);
		OLED_stamina();
		SYN_FrameInfo(2, (uint8_t *)"[v12][m0][t5]让小呆休息一下叭");
		if (rand()%2) //随机产生两种动作中的一种
			{move_sleep_p();}
		else{move_sleep_w();}
	}Delay_s(4);
	previous_mode = move_mode;
	move_mode = '0';
}
void mode_index(void)//展示开心值和体力值
{
	OLED_Clear();
	OLED_ShowChinese(0,0,"开心值，");
	if(happiness==100)
	{OLED_ShowNum(64,0,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,0,happiness,2,OLED_8X16);}
	OLED_ShowChinese(0,16,"体力值，");
	if(stamina==100)
	{OLED_ShowNum(64,16,100,3,OLED_8X16);}
	else {OLED_ShowNum(64,16,stamina,2,OLED_8X16);}
	OLED_UpdateArea(0,0,128,32);
	sprintf(read,"[v12][m0][t5]当前的开心值为%d，体力值为%d",happiness,stamina);
	SYN_FrameInfo(2,(u8*)read);Delay_s(10);
	previous_mode = move_mode;
	move_mode = '0';
}

