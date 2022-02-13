#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "headfile.h"
//定义结构体
typedef struct
{
  int16 cnt_1ms;
  int16 cnt_2ms;
  int16 cnt_5ms;
  int16 cnt_10ms;
  int16 cnt_20ms;
  int16 cnt_50ms;
  int16 cnt_200ms;  //用于初期大津法处理
  int16 cnt_500ms;  //用于初期发送图像处理时间
  int16 time;
}Loop_t;
extern Loop_t Loop;

enum 
{
  TimeTest_1ms =0,
  TimeTest_2ms,
  TimeTest_5ms,
  TimeTest_10ms,
  TimeTest_20ms,
  TimeTest_50ms,
};


void Loop_Check(void);
void Duty_Loop(void);

void Duty_2ms(void);
void Duty_5ms(void);
void Duty_10ms(void);
void Duty_20ms(void);
void Duty_50ms(void);
void Duty_200ms(void);
void Duty_500ms(void); 














#endif