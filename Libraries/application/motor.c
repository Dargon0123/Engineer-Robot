#include "motor.h"
#define PI 3.1415926
#define Wheel_D 57
int pwm_view;
extern uint8 Receive_Pwm;
extern uint8 Stop_All_Flag;
int16 Total_Rad;     //拟定速度全为正值，后期调整刹车速度可以为负值
float Velocity;
extern uint8 Diamond_Shape_Flag ; //菱形、斑马线识别标志位区
extern uint8 Zebra_Crossing_Flag ;


/*-----------------------------FTM0 PWM模式的初始化----------------------------*/
void Pwm_FTM0_Init(void)
{
  ftm_pwm_init(ftm0,ftm_ch0,10*1000,0);//初始化FTM0CH0，50HZ，占空比为0，满占空比为1000 C1
  ftm_pwm_init(ftm0,ftm_ch1,10*1000,0);//初始化FTM0CH0，50HZ，占空比为0，满占空比为1000 C2
}

void Set_Pwm(int pwm1,int pwm2)
{
  ftm_pwm_duty(ftm0, ftm_ch0, pwm1);
  ftm_pwm_duty(ftm0, ftm_ch1, pwm2);
}

int Pwm =0;
void Pwm_Test(void)
{
//  pwm_view =Receive_Pwm;
//  static uint16 count =0;
//  static uint8 Stop_Flag =1;
  if(Key_Status(key3) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key3) == 0)
    { 
      Pwm +=10;
      LED_Turn(LED3);
      while(! (Key_Status(key3)) );
    }
  }
  
  if(Key_Status(key4) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key4) == 0)
    {
      Pwm -=10 ;
      LED_Turn(LED4);
      while(! (Key_Status(key4)) );
      
    }
  }
//  if(Diamond_Shape_Flag ==1 && Total_Rad >360 && Total_Rad <420 && Zebra_Crossing_Flag ==0) Pwm =60; 
  
  if(Stop_All_Flag >=3) Pwm =0;
  Pwm =ABS(Pwm);
//  Pwm =LIMIT(Pwm,0,150);
  pwm_view =Pwm;  
  Set_Pwm(Pwm,0);
//  if(Stop_Flag ==1)
//  {
//    if(Zebra_Crossing_Flag ==1 && Total_Rad >390)
//    {
//      Set_Pwm(0,60);
//      Pwm =0;
//      count ++;
//      if(count >800) 
//      {
//        count =0;
//        Pwm =200;
//        Stop_Flag =0;
//      }
//    }
//  }
  
}

void Zebra_Stop(void)
{
  static uint8 Stop_Flag =1;
  if(Stop_Flag ==1)
  {    
    if(Zebra_Crossing_Flag ==1 && Total_Rad >390) 
    {
      Set_Pwm(0,40);
      Pwm =180;
      Stop_Flag =0;
      pit_delay(pit0, 5000);      
    }    
  }
}
/*-----------------------------FTM1 编码器模式的初始化----------------------------*/

void Encoder_FTM1_Init(void)
{
  ftm_quad_init(ftm1); //初始化FTM1为编码器模式
}

int16 encoder =0;

void Get_Encoder(void)
{
  static uint16 count =0;
  encoder =ftm_quad_get(ftm1);
  ftm_quad_clean(ftm1);
//  if(encoder >30000 ) encoder =65536 -encoder; //反转
  if(encoder <0 ) 
  {
    encoder =-encoder; //反转
    Velocity =-(encoder /2150.4f /6 *1000* 60);  //计算车模速度 rad/min (encoder /6 *1000  /2150 *60) 单位cm/s
  }
  else 
    Velocity =encoder /2150.4f /6 *1000* 60;  //计算车模速度 rad/min (encoder /6 *1000  /2150 *60) 单位cm/s

  count +=encoder;
  if( count >=1024)    // 轮子一圈的脉冲数1024 *105 /50 (记录编码器圈数)
  {
    count =0;
    Total_Rad ++;   //记录编码器总圈数
  }              
}


void TB6612_IO_Init(void)
{
  gpio_init (B6,GPO,1);    //OLED显示用B口
  gpio_init (B7,GPO,0);
  gpio_init (B8,GPO,1);
  AIN1_High();
  AIN2_Low();
  STBY_High();
}


/*-----------------------------CMT 舵机控制的初始化----------------------------*/
void CMT_Init(void)
{
  cmt_pwm_init(330, 0);  //控制舵机PWM D7  控制周期相当于3030us  设置直行状态中间值设置为1500us
}

void Set_Steer_Pwm(uint16 pwm)
{
  cmt_pwm_duty(pwm);   //设置占空比为50

}


void Stero_Test(void)
{
  static uint16 Pwm =1500;
  if(Key_Status(key1) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key1) == 0)
    { 
      Pwm +=10;
      LED_Turn(LED3);
      while(! (Key_Status(key1)) );
    }
  }
  
  if(Key_Status(key2) == 0)
  {
    systick_delay_ms(10);
    if(Key_Status(key2) == 0)
    {
      Pwm -=10;
      LED_Turn(LED4);
      while(! (Key_Status(key2)) );
      
    }
  }
  Set_Steer_Pwm(Pwm);
}



/*--------------------------PIT 定时器测试---------------------------*/
void Pit_Timer_Init(void)
{
  //中断0
  pit_init_ms(pit0,6);				//定时1ms后中断 
  set_irq_priority(PIT0_IRQn,2);		//设置优先级,根据自己的需求设置
  enable_irq(PIT0_IRQn);			//打开pit0的中断开关
  
//  //中断2
//  pit_init_ms(pit2,6);				//定时1ms后中断
//  set_irq_priority(PIT2_IRQn,1);		//设置优先级,根据自己的需求设
//  enable_irq(PIT2_IRQn);			//打开pit0的中断开关
  
  
  EnableInterrupts;				//打开总的中断开关
}


//
//Right_Scan_Range =LIMIT( (Right_White),10,140 );  //限副扫描边界           
//    Left_Scan_Range  =LIMIT( (Left_White),10,140 );
//    //右边巡白线
//    if(Zebra_In_Flag ==0)
//    {
//      //向内扫描
//      for(j =Right_Scan_Range;j >(Right_Scan_Range-10);j--)
//      {
//        
//        if((imagebin[i][j+1] == 0xff) &&(imagebin[i][j] == 0xff)&& (imagebin[i][j-1] == 0x00))
//        {
//          Right_White_Flag =1;
//          White_Point =j;
//          while(imagebin[i][j] == 0xff) 
//          {
//            j++;    //右边向内扫描
//            if( (j-White_Point) >10) {Right_White_Flag =0;  break;}       
//          }
//          if(Right_White_Flag ==1 ) {Right_White =(White_Point +j)/2; Right_White_Last =Right_White; break;}            
//        }
//        else {Right_White =Right_White_Last;}
//      }
//      //向外扫描
//      for(j =Right_Scan_Range;j <(Right_Scan_Range+10);j++)  
//      {
//        if(Right_White_Flag ==1 ) break;
//        if((imagebin[i][j-1] == 0xff) &&(imagebin[i][j] == 0xff)&& (imagebin[i][j+1] == 0x00))
//        {
//          Right_White_Flag =1;
//          White_Point =j;
//          
//          {Right_White =(White_Point -5); Right_White_Last =Right_White; break;}        
//        }
//        else {Right_White =Right_White_Last;}
//      }
//      
//        
//      
//    }  
//    
//    //左边巡白线
//    if(Zebra_Out_Flag ==0)
//    { //左边向内扫描
//      for(j =Left_Scan_Range;j <(Left_Scan_Range+5);j++)
//      {
//         
//        if((imagebin[i][j-1] == 0xff) &&(imagebin[i][j] == 0xff)&& (imagebin[i][j+1] == 0x00))
//        {
//          Left_White_Flag =1;
//          White_Point =j;
//          for( j=White_Point ;j>1;j--)
//          {
//                //左边向外扫描
//            if((imagebin[i][j+1] == 0xff) &&(imagebin[i][j] == 0xff)&& (imagebin[i][j-1] == 0x00)) break;
//              
////              if( (White_Point-j) >7) {Left_White_Flag =0;  break;}       
//          }
//          if(Left_White_Flag ==1 ) {Left_White =(White_Point +j)/2; Left_White_Last =Left_White; break;}            
//        }
//        else {Left_White =Left_White_Last;}
//      }
//      //左边向外扫描
//      for(j =Left_Scan_Range;j >(Left_Scan_Range-9);j--)  
//      {
//        if(Left_White_Flag ==1 ) break;
//        if((imagebin[i][j+1] == 0xff) &&(imagebin[i][j] == 0xff)&& (imagebin[i][j-1] == 0x00))
//        {
//          Left_White_Flag =1;
//          White_Point =j;
//          
//          if(Left_White_Flag ==1 ) {Left_White =(j+5); Left_White_Last =Left_White; break;}        
//        }
//        else {Left_White =Left_White_Last;}
//      }
//      
//      











