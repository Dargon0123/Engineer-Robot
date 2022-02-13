#include "key.h"
extern uint8 Display_Flag;
//按键初始化
void Key_Init(void)
{
  gpio_init(E3,GPI,1);  //按键KEY4
  gpio_init(E4,GPI,1);  //按键KEY3
  gpio_init(E5,GPI,1);  //按键KEY2
  gpio_init(E6,GPI,1);  //按键KEY1
}

//判断按键状态
uint8 Key_Status(KEY_x keyn)
{
  uint8 key_value;
  switch(keyn)
  {
    case key1:
      key_value =gpio_get(E6);break;
    case key2:
      key_value =gpio_get(E5);break;
    case key3:
      key_value =gpio_get(E4);break;
    case key4:
      key_value =gpio_get(E3);break;
  }
  return key_value;
}


////按键测试
void Key_Test(void)
{
  if(Key_Status(key1) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key1) == 0)
    {
      Display_Flag =1;            //此处由于显示OLED标志位
      LED_Turn(LED2);
      while(! (Key_Status(key1)) );
    }
  }
  
  if(Key_Status(key2) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key2) == 0)
    {
      Display_Flag =0;           //此处由于显示OLED标志位
      LED_Turn(LED3);
      while(! (Key_Status(key2)) );
      
    }
  }
}
uint8 Velacity_Mode =0;  
//速度选择
void Velocity_Select(void)
{
  if(Key_Status(key3) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key3) == 0)
    {
      Velacity_Mode ++;
      if(Velacity_Mode >3)
        Velacity_Mode =0;
      while(! (Key_Status(key2)) );
    }
  }
}

