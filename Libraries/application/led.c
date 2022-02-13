#include "led.h"
void Led_Init(void)
{
  //K60 LED IO口初始化
  gpio_init(A14,GPO,1);  //D2
  gpio_init(A15,GPO,1);  //D3
  gpio_init(A16,GPO,1);  //D4
  gpio_init(A17,GPO,1);  //D5
  
  //外部 LED IO口初始化
  gpio_init(D9,GPO,0);  //前大灯
  gpio_init(D10,GPO,0);  //后刹车灯
  gpio_init(D11,GPO,0);  //左转向灯
  gpio_init(D12,GPO,0);  //右转向灯
  
  //红外 IO口初始化
  gpio_init(A9,GPI,0);   //红外 1
  gpio_init(A10,GPI,0);  //红外 2
  gpio_init(B11,GPI,0);  //红外 3
  gpio_init(B9,GPI,0);  //红外 4
  gpio_init(B10,GPI,0);  //红外 5
  gpio_init(A11,GPI,0);  //红外 6
}

//uint8 V1,V2,V3,V4,V5,V6;
//void get(void)
//{
//  V1 =Hongwai_Status(Hongwai_5);
//  V2 =Hongwai_Status(Hongwai_1);
//  V3 =Hongwai_Status(Hongwai_2);
//  V4 =Hongwai_Status(Hongwai_3);
//  V5 =Hongwai_Status(Hongwai_4);
//  V6 =Hongwai_Status(Hongwai_6);
//}
uint8 Hongwai_Status(Hongwai_x Hongwai)
{
  uint8 Hongwai_Value;
  switch (Hongwai)
  {
    case Hongwai_5:
      Hongwai_Value =gpio_get(A9);break;
    case Hongwai_1:
      Hongwai_Value =gpio_get(A10);break;
    case Hongwai_2:
      Hongwai_Value =gpio_get(B11);break; 
    case Hongwai_3:
      Hongwai_Value =gpio_get(B9);break;
    case Hongwai_4:
      Hongwai_Value =gpio_get(B10);break;
    case Hongwai_6:
      Hongwai_Value =gpio_get(A11);break;
  }
  return Hongwai_Value;

}


void LED_Turn(LED_x led)
{
  switch (led)
  {
    //核心板LED
    case LED2:
      gpio_turn(A14); break;
    case LED3:
      gpio_turn(A15); break;
    case LED4:
      gpio_turn(A16); break;
    case LED5:
      gpio_turn(A17); break;
    //外部增加LED  
    case Front_LED:
      gpio_turn(D9); break;
    case Brake_LED:
      gpio_turn(D10); break;
    case Turn_L_LED:
      gpio_turn(D11); break;
    case Turn_R_LED:
      gpio_turn(D12); break;    
  }
}

void LED_Open(LED_x led)
{
  switch (led)
  {
    case Front_LED:
      gpio_set(D9,1); break;
    case Brake_LED:
      gpio_set(D10,1); break;
    case Turn_L_LED:
      gpio_set(D11,1); break;
    case Turn_R_LED:
      gpio_set(D12,1); break; 
  }
}

void LED_Close(LED_x led)
{
  switch (led)
  {
    case Front_LED:
      gpio_set(D9,0); break;
    case Brake_LED:
      gpio_set(D10,0); break;
    case Turn_L_LED:
      gpio_set(D11,0); break;
    case Turn_R_LED:
      gpio_set(D12,0); break; 
  }
}
