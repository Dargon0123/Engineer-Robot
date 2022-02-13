#ifndef __PATH_CONTROL_H__
#define __PATH_CONTROL_H__
#include "headfile.h"
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) ) //用于限副处理
#define ABS(x) ( (x)>=0?(x):-(x) )  

typedef struct
{
  float kp;			 //比例系数
  float ki;			 //积分系数
  float kd;		 	 //微分系数
}PID_arg_t;

typedef struct
{
  float bias;
  float last_bias;
  float last_last_bias;
  float feedback_d;
  float differential;
  float integral;
}PID_val_t;




int16 PID_Position_Calcualte(float feedback,
                             float expect,
                             PID_arg_t *pid_arg,
                             PID_val_t *pid_val);

int16 PID_Speed_Calcualte(uint8 feedback,
                         int expect,
                         PID_arg_t *pid_arg,
                         PID_val_t *pid_val);



void PID_Init(void);
void Road_Control(void);
void Motor_Control(void);
void Motor_Control_Fast(void);
void LED_Control(void);
void Hongwai_Control(void);
void KS103_Disatance_Control(void);

void Array_Rank(uint8 str[],uint8 len);
void Array_Filter(uint8 str[],uint8 len);









#endif