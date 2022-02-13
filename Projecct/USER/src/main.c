/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main.c
 * @brief      		主循环
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/
/*-----------------------------------------------------------------
*                                                                 *
问题区域；
 //问题发现：换道区，左侧标志位为1 （已解决）
   外道出换道区域可以，扫美妙左边线数 为 7 防止扫描不到下面（已更改）
*                                                                 *
----------------------------------------------------------------*/
   

/*-----------------------------------------------------------------
*                                                                 *
*------------------------2019-03-25 v3.1中线修改版（升级）-----------------------------------*
*                                                                 *
----------------------------------------------------------------*/
//问题发现：换道区，左侧标志位为1 

#include "headfile.h"

extern uint8  imagebin[ROW][COL];
uint8 image_threshold;  //图像阈值
uint8 Dajin_Finish_Flag =0;
uint8 Display_Flag =0;
extern uint16 Total_Rad;     //拟定速度全为正值，后期调整刹车速度可以为负值
extern float Velocity;
extern int Target;  //电机速度目标值
extern int Target_view;  //舵机目标值，用于看波形
uint32 use_time;
int16 Encoder_Value;

uint8 KS_103_Finish_Flag =0;  //超声波第一次发送完成标志位

  int main(void)
{  
  get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
   uart_init(uart2,115200);  //串口2，蓝牙 匿名波形使用
  uart_init(uart1,9600);  //串口1，超声波使用
  uart_rx_irq_en(uart1); 
  Key_Init();
  Led_Init();
  Encoder_FTM1_Init();  //编码器初始化 A12、A13
  CMT_Init();             //舵机PWM初始化
  Pwm_FTM0_Init();      //PWM输出初始化 C1,C2
  PID_Init();
  camera_init();
  Pit_Timer_Init();     //PIT定时器的初始化
  OLED_Init();
//        NRF_Dev_Init();  
  //相关的库函数在 MK60DN10_ftm.c 里面
  //MK60DN共有三个FTM模块FTM0、FTM1、FTM2，每一个模块只能产生一个频率的PWM，意味着一个模块下所有的通道频率必须一致，
  //三个模块就只能产生三种不同频率的PWM。同一个模块不同通道占空比可以不一样。
//	ftm_pwm_init(ftm0,ftm_ch0,50,500);//初始化ftm0模块，0通道为50HZ，占空比为百分之50，默认精度为1000 引脚对应查看MK60DN10_port_cfg.h
//	ftm_pwm_init(ftm1,ftm_ch0,50,500);//初始化ftm0模块，0通道为50HZ，占空比为百分之50，默认精度为1000 引脚对应查看MK60DN10_port_cfg.h
//	ftm_pwm_init(ftm2,ftm_ch0,50,500);//初始化ftm0模块，0通道为50HZ，占空比为百分之50，默认精度为1000 引脚对应查看MK60DN10_port_cfg.h
  for(;;)
    {
//      OLED_Display();
//         Duty_Loop(); 
      Key_Test();        //用于控制显示OLED标志位
      Velocity_Select(); //选择速度模式
      if(mt9v032_finish_flag)
      { 
//        if(KS_103_Finish_Flag ==0)
//        {
//          KS_103_Send();
//          KS_103_Finish_Flag =1;
////            uart_rx_irq_en(uart1); 
//        }
        mt9v032_finish_flag = 0;         
        pit_time_start(pit1);                   //大津法运行时间 130MS
        image_threshold = otsuThreshold(image[0],COL,ROW);  //大津法计算阈值  局部优化 33 ms  
        Dajin_Finish_Flag =1;
//        Duty_Loop();
//        use_time = pit_time_get(pit1)/bus_clk_mhz;          //计算大津法程序消耗时间，单位微秒。
////                    本例程大津法时间绝对很长，想直接用必然不可行，需自行优化。
////                    有人说：你咋不直接优化好呢。我说：你想得美，要不要我直接把车做好给你啊。
//        pit_close(pit1); 
//        pit_time_start(pit1);
        //匿名发送波形、蓝牙发送
//        ANO_DT_Send_Actual((uint16)Velocity,Target,0,0,0,0,0,0,0); //(uint16)Velocity
//               printf("Encoder %d\r\n",(int)Velocity);
//        image_binaryzation();    //将图像二值化    最大5ms  
////        uart_putchar(uart1, 0x5a);
//        Get_Center_Line(); //提取赛道中线      最大1ms
        use_time = pit_time_get(pit1)/bus_clk_mhz;          //计算大津法程序消耗时间，单位微秒。
                    //本例程大津法时间绝对很长，想直接用必然不可行，需自行优化。
                    //有人说：你咋不直接优化好呢。我说：你想得美，要不要我直接把车做好给你啊。
        pit_close(pit1); 
//#if  Display_Off    //|| Display_Flag 
        if(Display_Flag)
          OLED_Display();
       
//#endif 
////        //发送二值化图像至上位机  
//////        //串口发送                             最大350ms
//        if(Display_Flag)
//        {       
//          uart_putchar(uart2,0x00);uart_putchar(uart2,0xff);uart_putchar(uart2,0x01);uart_putchar(uart2,0x01);//发送命令
//          uart_putbuff(uart2, (uint8_t *)imagebin, ROW*COL);  //发送图像  
//        }
//        if(KS_103_Finish_Flag ==0)
//        {
//          KS_103_Send();
////          KS_103_Finish_Flag =1;
//        }
        //蓝牙发送
//        if(Display_Flag)
//        {       
//          uart_putchar(uart1,0x00);uart_putchar(uart1,0xff);uart_putchar(uart1,0x01);uart_putchar(uart1,0x01);//发送命令
//          uart_putbuff(uart1, (uint8_t *)imagebin, ROW*COL);  //发送图像  
//        }
//            //NRF24L01 无线传送
////           // nrf_send_array(imagebin[0],COL*ROW);
        LED_Turn(LED5);  
      }
//Duty_Loop();
//      Pwm_Test(); //电机测试
//      Stero_Test();
//      LED_Turn(LED5);
    }

}


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//        //中间巡白色区  
//        for(i =60;i >0;i-=10)   //查看并提取5行元素
//        {
//          for(j =60;j <90;j++)
//          { 
//            if( (imagebin[i][j] ==0xff) && (imagebin[i][j+1] ==0xff) && (imagebin[i][j+2] ==0xff) ) 
//            {Find_White_Range_Flag ++; break;}              
//          } 
//          
//        }
//        if(Find_White_Range_Flag >=1) 
//        {Straight_Road_Select_Flag =0; }
//        else 
//        {Straight_Road_Select_Flag =1;break;}
//        
//        Find_White_Range_Flag =0;                           //标志位清零
//        Straight_Road_Flag_view =Straight_Road_Select_Flag;      
//          
//        //中间识别菱形区域  //斑马线识别区域   
//        for(j =30;j <120;j++) 
//        {
//          if( (imagebin[60][j] == 0x00) && (imagebin[60][j+1] == 0xff) )
//          {
//            Black_Turn_White_Flag_D ++;
//          }
//          if( (imagebin[30][j] == 0x00) && (imagebin[30][j+1] == 0xff) )
//            if((j >40) && (j <110)) 
//              Black_Turn_White_Flag_Z ++;
//        }
//        //用于显示全局变量值
//        Diamond_view =Black_Turn_White_Flag_D;
//        Zebra_view =Black_Turn_White_Flag_Z;
//        
//        if( (Black_Turn_White_Flag_D ==2)  )  
//          Diamond_Shape_Flag =1;
//        else
//          Diamond_Shape_Flag =0;
//        
//        if( (Black_Turn_White_Flag_Z > 2)  )   
//          Zebra_Crossing_Flag =1;    //？？？数值需具体调试得知
//        else 
//          Zebra_Crossing_Flag =0; 
//        
//        Zebra_Crossing_Flag_view =Zebra_Crossing_Flag; 


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//  switch(Road_Select_Flag)   //代码已屏蔽
//  {
//  case 0:   //普通直行
//    {
//      switch(Straight_Road_Select_Flag)   // 普通中间有白线直道
//      {
//      case 0:
//      {
//        //中间巡白线
//        for(i =60;i >0;i-=5)
//        {
//          for(j =60;j <90;j++)
//          { 
//            if( (imagebin[i][j] ==0xff) && (imagebin[i][j+1] ==0xff)) 
//               {Straight_Road_Select_Flag =0;break;}
//               else 
//              {Straight_Road_Select_Flag =1;}
//              
//          } 
//          if(Straight_Road_Select_Flag ==0) break;
//        }
//        Straight_Road_Flag_view =Straight_Road_Select_Flag;
//        if(Straight_Road_Select_Flag ==1 ) break;
//       
//        //中间识别菱形区域  //斑马线识别区域   
//        for(j =30;j <120;j++) 
//        {
//          if( (imagebin[60][j] == 0x00) && (imagebin[60][j+1] == 0xff) )
//          {
//            Black_Turn_White_Flag_D ++;
//          }
//          if( (imagebin[30][j] == 0x00) && (imagebin[30][j+1] == 0xff) )
//            if((j >40) && (j <110)) 
//              Black_Turn_White_Flag_Z ++;
//        }
//        
//        Black_Turn_White_Flag_view =Black_Turn_White_Flag_Z;
//        
//        if( (Black_Turn_White_Flag_D ==2)  )  
//          Diamond_Shape_Flag =1;
//        else
//          Diamond_Shape_Flag =0;
//        
//        if( (Black_Turn_White_Flag_Z > 2)  )   
//          Zebra_Crossing_Flag =1;    //？？？数值需具体调试得知
//        else 
//          Zebra_Crossing_Flag =0; 
//        
//        Zebra_Crossing_Flag_view =Zebra_Crossing_Flag;
//     /////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>                 
//        for(i =60;i >50;i-=2)  //从每一行开始扫描               
//        {
//          
//          //右边巡白线
//          for(j =100;j <150;j++)
//          {
//          
//            if( (imagebin[i][j] == 0x00) && (imagebin[i][j+1] == 0xff) )
//            {                
//              Right_White =j;
//              Right_White_Last =Right_White;
//              Right_White_Flag =1;
//              break;               
//            }
//            else 
//            {  
//              if( (imagebin[i][110] ==0x00) && (imagebin[i+2][110] ==0x00))
//              {
//              Right_White =j; 
//              Right_White_Flag =0; 
//              }
//            }
//            
//          }
//          //左边巡白线
//          for(j =40;j >0;j--)
//          {
//            if((imagebin[i][j] == 0x00) && (imagebin[i][j-1] == 0xff) )
//            {
//              if(j <40)  //单加判断条件，直道进入弯道之前。更改 原值40
//              {
//                Left_White =j;
//                Left_White_Last =Left_White;
//                Left_White_Flag =1;
//                break;
//              }
//            }
//            else 
//            {  
//              if( (imagebin[i][40] ==0x00) && (imagebin[i+2][40] ==0x00))     //加判断条件
//              {
//              Left_White =j-1;
//              Left_White_Flag =0;
//              }
//            }
//          }
//          
//          
//
//         
//          Right_White_Flag_view =Right_White_Flag;
//          Right_White_view =Right_White;
//          Left_White_Flag_view =Left_White_Flag;
//          Left_White_view = Left_White;
//            
//         //进行道路检测
//          //直道判断
//          if(Right_White_Flag ==1 && Left_White_Flag ==1 )
//      //      if( (Right_White -Left_White >= Black_Line_Min) && (Right_White -Left_White <= Black_Line_Max) )
//            {
//              Middle_Line =(Right_White +Left_White)/2;
//              Middle_Line_Last =Middle_Line;
//            }
//          //左弯道判断
//          if(Right_White_Flag ==1 && Left_White_Flag ==0 )      
//            Road_Select_Flag =1;           //>>>>>>>>
//           
//          
//          //右弯道判断
//          if(Right_White_Flag ==0 && Left_White_Flag ==1 )
//            Road_Select_Flag =2;
//          
//          if(Zebra_Crossing_Flag ==1)
//          {Road_Select_Flag =0;Middle_Line =Middle_Line_Last;}
//          //两边都未寻到
//          if(Right_White_Flag ==0 && Left_White_Flag ==0 )
//            Middle_Line =Middle_Line_Last;
//          
//          
//          //中线显示
//          Mid_Line_view =Middle_Line;
//          Fit_Middle[(i-50)/2] =Middle_Line;          
//          imagebin[i][Middle_Line] =0xff;   //显示所巡中线
//          imagebin[i][75] =0xff;  //显示赛道标准中线
//      }
//    break;
//    }
//    case 1:  //起点直道判断    后边加break
//    {
//        //中间巡白线
//        for(i =60;i >20;i-=5)
//        {
//          for(j =90;j <149;j++)
//          {
//            if(imagebin[i][j] ==0x00 && imagebin[i][j+1] ==0xff)
//              {Straight_Road_Select_Flag =0;break;}               //后面有加continue
//            else
//            {Straight_Road_Select_Flag =1;}                       //未寻到白线
//          }
//          if(Straight_Road_Select_Flag ==1 ) break;
//         
////          for(j =55;j <90;j++)
////          { 
////            if( (imagebin[i][j] ==0xff) && (imagebin[i][j+1] ==0xff)) 
////              {Straight_Road_Select_Flag =0; break;}
////            else 
////              {Straight_Road_Select_Flag =1; }            
////          }
////          if(Straight_Road_Select_Flag ==0) break;
//          
//        }
//        
//        Straight_Road_Flag_view =Straight_Road_Select_Flag;
//        if(Straight_Road_Select_Flag ==0 ) break;                 //从这开始写程序
//        
//        
//        //进行两边巡白线
//        for(i =60;i >50;i-=2)
//        {        
//        //右边巡白线
//        for(j =100;j <150;j++)                                    //？？？后期可做列数大小的检测调整
//        {      
//          if(imagebin[i][j] == 0x00 && imagebin[i][j+1] == 0xff)
//          {
//            Right_White =j;
//            Right_White_Last =Right_White;
//            Right_White_Flag =1;
//            break;
//          }
//          else
//          { 
//            Road_Inside_Flag =1; //进入内侧赛道
//            Right_White =j;
//            Right_White_Flag =0;            
//          }          
//        }
//        //左边巡白线
//        for(j =50;j >0;j--)
//        {
//          if(imagebin[i][j] == 0x00 && imagebin[i][j-1] == 0xff)
//          {
//            Left_White =j;
//            Left_White_Last =Left_White;
//            Left_White_Flag =1;
//            break;
//          }
//          else
//          {
//            Road_Outside_Flag =1;
//            Left_White =j-1;
//            Left_White_Flag =0;            
//          }
//        }
//        
//        //查看数据
//        Right_White_Flag_view =Right_White_Flag;
//        Right_White_view =Right_White;
//        Left_White_Flag_view =Left_White_Flag;
//        Left_White_view = Left_White;
//        //直道判断
//        if(Right_White_Flag ==1 && Left_White_Flag ==1 )
//          {
//            Middle_Line =(Right_White +Left_White)/2;
//            Middle_Line_Last =Middle_Line;
//          }
//         //判断进入中间
//        if( (Road_Outside_Flag ==1) || (Road_Inside_Flag ==1) ) 
//          {
//            Middle_Line =Middle_Line_Last;
//            
//          }
////        if(Right_White_Flag ==0 || Left_White_Flag ==0 )  Straight_Road_Select_Flag =1;
////          Straight_Road_Flag_view =Straight_Road_Select_Flag;      //后续进行删除
//        
//          Mid_Line_view =Middle_Line;
//          Fit_Middle[(i-50)/2] =Middle_Line;
//          imagebin[i][Middle_Line] =0xff;   //显示所巡中线
//          imagebin[i][75] =0xff;  //显示赛道标准中线
//      } 
//      break;
//      
//    }
//
//    }  //对应第二个switch
//    if(Road_Select_Flag ==3) Middle_White_Line_Flag ++; 
//    break;
//    }
//      
//  case 1:  //左弯道
//    {
//      for( j =30;j >0;j--)                                    //出弯道判断，从中间向两侧判断
//      {
//        if( (imagebin[60][j] ==0x00) && (imagebin[60][j-1] ==0xff) )
//          if(j <20)        
//            { 
//              Road_Select_Flag =0;
//              break;
//            }
//      }
//      
//      if(Road_Select_Flag ==0)    break;
//      
//      for(i =60;i >50;i-=2)                                         
//      {        
//        //从图像最左侧巡白线
//        for(j =0;j <150;j++)
//        {
//          if(imagebin[i][j] ==0xff && imagebin[i][j+1] ==0x00)
//          {
//            //判断右侧赛道白线
//            if(j >70)
//            {
//              Right_White =j;
////            Left_White_Last =Left_White;
//              Right_White_Flag =1;
//              break;
//            }
//            if(j <40)        
//            { 
//              Road_Select_Flag =0;
//              break;
//            }
//          }
//          
//          if(Road_Select_Flag ==0)    break;
//        }
//        
//        //提取弯道中线
//        if(Right_White_Flag ==1)     Middle_Line =Right_White/2-10;
//        Mid_Line_view =Middle_Line;
//        Fit_Middle[(i-50)/2] =Middle_Line;
//        imagebin[i][Middle_Line] =0xff;   //显示所巡中线
//        imagebin[i][75] =0xff;  //显示赛道标准中线
//      }
////      if(Road_Select_Flag ==3) Middle_White_Line_Flag ++;
//      break;
//    }
//    
//  case 2:  //右弯道
//    {
////      //巡中间白色横线
////      if( (imagebin[100][30] == 0xff) && (imagebin[100][40] == 0xff) && (imagebin[100][50] == 0xff) && (imagebin[100][60] == 0xff) )
////        {
////          Road_Select_Flag =3;
////          break;
////        }
////      //从右侧巡白线
//      for(i =60;i >50;i-=2)
//      {        
//        for(j =150;j >0;j--)                            
//        {
//          if( (imagebin[i][j] == 0x00)  &&  (imagebin[i][j-1] == 0xff) )
//          {
//            //判断左侧赛道白线
//            if(j <50)
//            {
//              Left_White =j;
//    //        Left_White_Last =Left_White;
//              Left_White_Flag =1;
//              break;
//            }
//            else if(j >130)       
//              Road_Select_Flag =0;
//            
//          }
//          
//          //提取弯道中线
//          if(Left_White_Flag == 1)    Middle_Line =(150 -Left_White)/2 +Left_White;
//          Mid_Line_view =Middle_Line;
//          Fit_Middle[(i-50)/2] =Middle_Line;
//          imagebin[i][Middle_Line] =0xff;   //显示所巡中线
//          imagebin[i][75] =0xff;  //显示赛道标准中线
//        }
//      }
//      if(Road_Select_Flag ==3) Middle_White_Line_Flag ++;
//      break;
//    }
//    
////  case 3:  //起点路线直行
////    {
////      //巡中间白色横线
////      if( (imagebin[100][30] == 0xff) && (imagebin[100][40] == 0xff) && (imagebin[100][50] == 0xff) && (imagebin[100][60] == 0xff) )
////        {
////          Road_Select_Flag =0;
//////            Middle_White_Line_Flag ++;           
////          break;
////        }
////      for(i =60;i >40;i--)
////      {        
////        //右边巡白线
////        for(j =100;j <150;j++)                                      //？？？后期可做列数大小的检测调整
////        {      
////          if(imagebin[i][j] == 0x00 && imagebin[i][j+1] == 0xff)
////          {
////            Right_White =j;
////            Right_White_Last =Right_White;
////            Right_White_Flag =1;
////            break;
////          }
////          else
////          { 
////            Road_Inside_Flag =1; //进入内侧赛道
////            Right_White =j;
////            Right_White_Flag =0;            
////          }          
////        }
////        //左边巡白线
////        for(j =50;j >0;j--)
////        {
////          if(imagebin[i][j] == 0x00 && imagebin[i][j-1] == 0xff)
////          {
////            Left_White =j;
////            Left_White_Last =Left_White;
////            Left_White_Flag =1;
////            break;
////          }
////          else
////          {
////            Road_Outside_Flag =1;
////            Left_White =j-1;
////            Left_White_Flag =0;            
////          }
////        }
////        //直道判断
////        if(Right_White_Flag ==1 && Left_White_Flag ==1 )
////          {
////            Middle_Line =(Right_White +Left_White)/2;
////            Middle_Line_Last =Middle_Line;
////          }
////         //判断进入中间
////          if( (Road_Outside_Flag ==1) || (Road_Inside_Flag ==1) ) Middle_Line =Middle_Line_Last;
////           
////          Mid_Line_view =Middle_Line;
////          Fit_Middle[(i-40)/2] =Middle_Line;
////          imagebin[i][Middle_Line] =0xff;   //显示所巡中线
////          imagebin[i][75] =0xff;  //显示赛道标准中线
////      }  
////      if(Road_Select_Flag ==0) Middle_White_Line_Flag ++;
////      break;
////    } 
//  }

