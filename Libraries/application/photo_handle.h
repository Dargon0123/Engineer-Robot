#ifndef __PHOTO_HANDLE_H__
#define __PHOTO_HANDLE_H__
#include "headfile.h"
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) ) //用于限副处理
#define ABS(x) ( (x)>=0?(x):-(x) ) 
#define Display_On 1
#define Display_Off 0
//---------------定义全局变量区域-----------------------
typedef struct
{
  uint8 one ;
  uint8 two ;
  uint8 three;
  uint8 four;
  uint8 five;
  uint8 six;
}Times_Falg_t;



//---------------函数头文件声明区域--------------------

void image_binaryzation(void);
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void Get_Center_Line(void);



//---------------调试区头文件声明--------------------
void nrf_send_array(uint8 *dat, uint16 length);
uint8 Value_Change_Times(uint8 a);

void OLED_Display(void);
uint8 fs(int x,uint8 y);
  

#endif















