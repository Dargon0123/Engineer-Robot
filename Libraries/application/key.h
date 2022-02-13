#ifndef __KEY_H__
#define __KEY_H__
#include "headfile.h"

typedef enum
{
  key1,
  key2,
  key3,
  key4   
} KEY_x;

void Key_Init(void);
uint8 Key_Status(KEY_x keyn);
void Velocity_Select(void);

//////////////////////测试区头文件声明区域
void Key_Test(void);




#endif