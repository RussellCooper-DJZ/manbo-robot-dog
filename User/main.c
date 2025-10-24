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
*程序名称：电子桌面宠物“小呆”
*当前版本：V1.0-2024/6/9
*
*****************************************************
*/

uint8_t RxData;			//用于接收串口数据的变量
uint8_t move_mode1 = '0';//用于接收蓝牙串口数据的变量
uint8_t move_mode3 = '0';//用于接收语音识别串口数据的变量
uint8_t move_mode = '0';//决定状态变量
uint8_t previous_mode = '0';//用于保存上一次接收串口数据的变量
 uint16_t T=100; // 记录距离 给初值大于10cm(不触发避障) 
uint16_t bz_flag;//避障标志位 ON/OFF 默认off
uint16_t hw_flag;//红外标志位 ON/OFF 默认off
int happiness; //开心指数
int stamina;  //体力值
uint8_t t1=0 ; //存储红外的信号 前
uint8_t t2=0 ; //存储红外的信号 后
uint8_t t3=0 ; //存储红外的信号 左
uint8_t t4=0 ; //存储红外的信号 右
uint16_t ff=0; //连续前进计数
uint16_t bb=0; //连续后退计数
uint16_t ll=0; //连续左转计数
uint16_t rr=0; //连续右转计数

int main(void)
{
	/*模块初始化*/
	LED_Init();			//LED初始化
	OLED_Init();		//OLED初始化
	USART1_Init();		//蓝牙串口初始化
	USART2_Init();		//语音合成串口初始化
	USART3_Init();		//语音识别串口初始化
	Servo_Init();		//舵机初始化
	hongwai_init();     //红外初始化
	UltrasonicWave_Init(); //超声波初始化
	happiness=200;//初始化开心指数
	stamina=500;//初始化体力值
	bz_flag=0;//避障标志位 ON/OFF 默认off
	hw_flag=0;//红外标志位 ON/OFF 默认off
	/*进入默认状态*/
	OLED_Clear();
	OLED_ShowImage(0, 0, 128, 64, BMP1); //立正
 
/**************语音合成芯片设置命令*********************/
//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
//m[0~16]:0背景音乐为静音，16背景音乐音量最大
//v[0~16]:0朗读音量为静音，16朗读音量最大
//t[0~5]:0朗读语速最慢，5朗读语速最快

	SYN_FrameInfo(0, (uint8_t *)"[v12][m0][t5]旺旺");Delay_s(1);//唤醒后狗叫一声,音量12适合演示，10适合调试
	
	while (1)
	{  
		//***体力值与开心值反应设置***//
		if(stamina>0)//体力值大于零才会听令
	{
		move_mode1 = USART1_GetRxData();		//获取蓝牙串口接收的数据
		move_mode3 = USART3_GetRxData();		//获取语音识别串口接收的数据
		if (USART1_GetRxFlag()) {//设置蓝牙接收优先级大于语音识别
			move_mode = move_mode1;
		}
		else if (USART3_GetRxFlag()){
			move_mode = move_mode3;
		}
	}
		if(stamina<0)//体力值小于0，执行“难受”状态
	{
		mode_nanshou();
		stamina=0;
	}
		if(stamina==0)//体力值为0后，只接受睡觉指令
	{
		if(USART1_GetRxData()=='2'|USART3_GetRxData()=='2')
		{mode_sleepwo();}
		else if(USART1_GetRxData()=='p'|USART3_GetRxData()=='p')
		{mode_sleeppa();}
	}
		if(stamina>500)//体力值不大于1000
		{stamina=500;}
		
		if(happiness<0){//开心值小于0后，执行“难受”状态；给出增加开心值的指令后摆脱难受状态
		happiness=0;
		mode_nanshou();
		}
		if(happiness>200)//开心值不大于1000
		{happiness=200;}
		
		//***红外悬崖检测模块设置***//
		if(move_mode == 'H') //红外开
		{hw_flag=1;}
		if(move_mode == 'h') //红外关
		{hw_flag=0;
			//以下给四个传感器输出清零
			t1=0;t2=0;t3=0;t4=0;
		}
		if(hw_flag){
			Edge_detect();//打开红外悬崖检测模式	
		}
		
	   //***超声避障模块设置***//
		if(move_mode == 'x') //超声开
		{
			bz_flag=1;
		}
		if(move_mode == 'c') //超声关
		{
			bz_flag=0;
			T=100;
		}
		if(bz_flag){
		Bizhang();//打开超声避障模式
		}
		
		
		//***互动设置***//
		if (move_mode == 'f') { //前进
			mode_forward();
		} else if (move_mode == 'b') { //后退
			mode_behind();
		} else if (move_mode == 'l') { //左转
			mode_left();
		} else if (move_mode == 'r') { //右转
			mode_right();
		} else if (move_mode == 'w') { //前后摇摆
			mode_swing_qianhou();
		} else if (move_mode == 'z') { //左右摇摆
			mode_swing_zuoyou();
		} else if (move_mode == 'd') { //跳舞
			mode_dance();
		} else if (move_mode == '5') { //立正
			mode_stand();
		} else if (move_mode == 'q' && previous_mode != '0') { //起身
			mode_slowstand();
		} else if (move_mode == 's' && previous_mode != 's') { //坐下
			mode_strech();
		} else if (move_mode == 'j') { //交替抬手
			mode_twohands();
		} else if (move_mode == 'y') { //伸懒腰
			mode_lanyao();
		} else if (move_mode == '1') { //抬头
			mode_headup();
		} else if (move_mode == 'p' && previous_mode != 'p') { //趴下睡觉
			mode_sleeppa();
		} else if (move_mode == '2' && previous_mode != '2') { //卧下睡觉
			mode_sleepwo();
		} else if (move_mode == 'n') { //难受
			mode_nanshou();
		} else if (move_mode == 'B') { //表白
			mode_biaobai();
		} else if (move_mode == 'Y') { //背元素周期表
			mode_yuansu();
		} else if (move_mode == 'X') { //背校训
			mode_xiaoxun();
		} else if (move_mode == 'W') { //为世界之光
			mode_world();
		} else if (move_mode == 'o') { //打招呼
			mode_hello();
		} else if (move_mode == 'U') { //被叫“小呆”
			mode_xiaodai();
		} else if (move_mode == 'K') { //展示开心值
			mode_happiness();
		} else if (move_mode == 'T') { //展示体力值
			mode_stamina();
		} else if (move_mode == 'Z') { //展示开心值和体力值
			mode_index();
		} 


	}
}
