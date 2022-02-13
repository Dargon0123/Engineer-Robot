#include "scheduler.h"

extern uint8 image_threshold;  //图像阈值uint8 image_threshold;  //图像阈值
extern uint8 imagebin[ROW][COL];


Loop_t Loop;
void Loop_Check(void)
{
  Loop.cnt_2ms ++;
  Loop.cnt_5ms ++;
  Loop.cnt_10ms ++;
  Loop.cnt_20ms ++;
  Loop.cnt_50ms ++;
  Loop.cnt_200ms ++;
  Loop.cnt_500ms ++;
}



void Duty_Loop(void)
{
  if(Loop.cnt_2ms >=2)
  {
    Loop.cnt_2ms =0;
    Duty_2ms();
  }
  if(Loop.cnt_5ms >=5)
  {
    Loop.cnt_5ms  =0;
    Duty_5ms();
  }
  if(Loop.cnt_10ms >=10)
  {
    Loop.cnt_10ms =0;
    Duty_10ms();
  }
  if(Loop.cnt_20ms >=20)
  {
    Loop.cnt_20ms =0;
    Duty_20ms();
  }
  if(Loop.cnt_50ms >=50)
  {
    Loop.cnt_50ms =0;
    Duty_50ms();
  }
  
  if(Loop.cnt_200ms >=200)
  {
    Loop.cnt_200ms =0;
    Duty_200ms();
  } 
  
  if(Loop.cnt_500ms >=500)
  
  {
    Loop.cnt_500ms =0;
    Duty_500ms();
  }

}  
  
  
void Duty_2ms(void)
{
//  static uint8 i;  
//  i++;
//  if(i >100) i =0;
//  if(i ==100) LED_Turn(LED3);
//  Get_Center_Line(); //提取赛道中线      最大1ms
}
  
  
void Duty_5ms(void)
{
//  static uint8 i;  
//  i++;
//  if(i >100) i =0;
//  if(i ==100) LED_Turn(LED4);
//   image_binaryzation();    //将图像二值化    最大5ms
//    Get_Center_Line(); //提取赛道中线      最大1ms
  
}  
  
void Duty_10ms(void)
{
//  static uint8 i;  
//  i++;
//  if(i >100) i =0;
//  if(i ==100) LED_Turn(LED5);
//  if(mt9v032_finish_flag)
//  {
//    mt9v032_finish_flag =0;
//    image_binaryzation();    //将图像二值化    最大5ms
//    Get_Center_Line(); //提取赛道中线      最大1ms
//  }
}  
  
 
void Duty_20ms(void)
{
//  static uint8 i;  
//  i++;
//  if(i >100) i =0;
//  if(i ==100) LED_Turn(LED2);
  
}


void Duty_50ms(void)       //Not effect
{
  //      OLED_Fill(0x25);
//      OLED_P6x8Str(0,0,"test:");
//      OLED_P6x8Str(60,4,"test:");
//     
//      OLED_P6x8Str(60,0,"test:");
//static uint8 i;  
//  i++;
//  if(i >20) i =0;
//  if(i ==20) LED_Turn(LED2);
//  OLED_Display();
//  Get_Center_Line(); //提取赛道中线      最大1ms
}



void Duty_200ms(void)  //函数跑大津法程序
{
//  static uint8 i;  
//  i++;
//  if(i >5) i =0;
//  if(i ==5) LED_Turn(LED3);
//  if(mt9v032_finish_flag)
//  { 
//    mt9v032_finish_flag =0;
//    image_threshold = otsuThreshold(image[0],COL,ROW);  //大津法计算阈值
//    image_binaryzation();    //将图像二值化    最大5ms
//    Get_Center_Line(); //提取赛道中线      最大1ms
//  }
  
}


void Duty_500ms(void) 
{
//        uart_putchar(uart2,0x00);uart_putchar(uart2,0xff);uart_putchar(uart2,0x01);uart_putchar(uart2,0x01);//发送命令
//        uart_putbuff(uart2, (uint8_t *)imagebin, ROW*COL);  //发送图像 
//  Get_Center_Line(); //提取赛道中线      最大1ms
}



  
  
  
  
  
  
  
  
  