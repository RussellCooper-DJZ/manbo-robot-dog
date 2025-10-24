#ifndef __MOVEMENT_H
#define __MOVEMENT_H
#include "stm32f10x.h"    
void move_stand(void);
void move_forward(void);
void move_behind(void);
void move_right(void);
void move_right_hw(void);
void move_left(void);
void move_left_hw(void);
void move_hello(void);
void move_shake_qianhou(void);
void move_shake_zuoyou(void);
void move_dance(void);
void move_butt_up(void);
void move_head_up(void);
void move(uint16_t set1 , uint16_t set2 , uint16_t set3 , uint16_t set4 , uint16_t speed);
void move_slow_stand(uint8_t previous_mode);
void move_stretch(void);
void move_two_hands(void);
void lan_yao(void);
void move_sleep_p(void);
void move_sleep_w(void);



#endif
