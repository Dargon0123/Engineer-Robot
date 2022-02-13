#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "headfile.h"


/*---------------------TB66612 IO口定义-------------------------*/
#define AIN1   B6
#define AIN2   B7
#define STBY   B8

#define AIN1_Low()   gpio_set (AIN1, 0)		//IO口输出低电平
#define AIN1_High()  gpio_set (AIN1, 1)		//IO口输出高电平

#define AIN2_Low()   gpio_set (AIN2, 0)		//IO口输出低电平
#define AIN2_High()  gpio_set (AIN2, 1)		//IO口输出高电平

#define STBY_Low()   gpio_set (STBY, 0)		//IO口输出低电平
#define STBY_High()  gpio_set (STBY, 1)		//IO口输出高电平


/*---------------------FTIM PWM 初始化-------------------------*/
void Pwm_FTM0_Init(void);
void Set_Pwm(int pwm1,int pwm2);
void Pwm_Test(void);
void Zebra_Stop(void);

/*---------------------FTIM 编码器 初始化-------------------------*/
void Encoder_FTM1_Init(void);
void Get_Encoder(void);
void TB6612_IO_Init(void);

/*-----------------------------CMT 舵机控制的初始化----------------------------*/
void CMT_Init(void);
void Set_Steer_Pwm(uint16 pwm);



void Stero_Test(void);
void Pit_Timer_Init(void);



#endif