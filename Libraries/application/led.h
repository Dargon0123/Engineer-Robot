#ifndef __LED_H__
#define __LED_H__
#include "headfile.h"

typedef enum
{
  LED2,
  LED3,
  LED4,
  LED5,  
  Front_LED,
  Brake_LED,
  Turn_L_LED,
  Turn_R_LED
} LED_x;

typedef enum
{
  Hongwai_1,
  Hongwai_2,
  Hongwai_3,
  Hongwai_4,
  Hongwai_5,
  Hongwai_6
}Hongwai_x;

void Led_Init(void);
void LED_Turn(LED_x led);
void LED_Open(LED_x led);
void LED_Close(LED_x led);
uint8 Hongwai_Status(Hongwai_x Hongwai);




void get(void);














#endif