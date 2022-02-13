/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr.c
 * @brief      		中断函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#include "isr.h"
#include "scheduler.h"
   
extern uint8 Dajin_Finish_Flag;
uint8 Receive_Buffer[4];
uint8 Receive_P;
uint8 Receive_D;
int16 Receive_Pwm;

uint8 R_Data;
uint8 Statu =0;
uint8 R_High,R_Low;
uint16 KS103_Distance;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void PORTA_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	PORTA->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTA_FLAG_CLR(A1);

}


void PORTC_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	PORTC->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTC_FLAG_CLR(C1);
    VSYNC();
}


void DMA0_IRQHandler(void)
{
	DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
    if( (Dajin_Finish_Flag ==1) ) // && (mt9v032_finish_flag ==1)
    {
//        image_binaryzation();    //将图像二值化    最大5ms        
        Get_Center_Line(); //提取赛道中线      最大1ms
//        Road_Control();    //控制舵机
        LED_Turn(LED3);
    }
}


uint8 Receive_Ok =1;
extern uint8 KS_103_Finish_Flag;
extern uint8 Velacity_Mode;
void PIT0_IRQHandler(void)   //5ms中断服务函数
{
  static uint8 i,count =0;
   
  PIT_FlAG_CLR(pit0);
  i++;
  
  
  if(i ==50) 
  {
    i =0;
    LED_Turn(LED2);
    LED_Control();
//    LED_Turn(Turn_L_LED); 
  }
//  if(i >50) i =0;
//  Road_Control();    //控制舵机
//  Key_Test();        //用于控制显示OLED标志位
//  Velocity_Select();
  Get_Encoder();     //读取编码器数值
//  if(Velacity_Mode ==1)
    Motor_Control();   //控制电机
//  else if(Velacity_Mode ==2 )
//    Motor_Control_Fast();
  Road_Control();    //控制舵机
  
  
  //超声波发送指令
  if( Receive_Ok ==1) //Receive_Ok ==1 || 
  {
    count ++;
    if(count >=3) 
    {
      count =0;
      KS_103_Send();
      
      
    }
  }


}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当UART3启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if(UART3->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
        //用户需要处理接收数据
        mt9v032_cof_uart_interrupt();
    }
    if(UART3->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART1中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当UART1启用中断功能且发生中断的时候会自动执行该函数 (蓝牙使用)
//-------------------------------------------------------------------------------------------------------------------
extern uint8 Tx1Buffer[256];
extern uint8 Tx1Counter ;
extern uint8 count1 ;
void Uart1_Send(uint8 *DataToSend,uint8 data_num)
{
  uint8 i;
  for(i =0;i <data_num;i++)
  {
    Tx1Buffer[count1 ++] =*(DataToSend +i);
    
  }
}

void KS_103_Send(void)
{
      uart_putchar(uart1,0x55);
//      systick_delay(100 *25);           //延时 20us
//      uart_putchar(uart1,0x02);
//      systick_delay(100 *25);
//      uart_putchar(uart1,0x0f);        //2m
//      systick_delay(100 *25);
      Statu =0;
}
void UART1_RX_TX_IRQHandler(void)
{
//  static uint8 num =0;
  if(UART1->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
        //用户需要处理接收数据
       R_Data =  UART1->D; 
       if(Statu ==0)
       {
         R_High =R_Data;
         
         Statu =1;
       }
       else if(Statu ==1)
       {
         R_Low =R_Data;
         Statu =2;
         Receive_Ok =1;
       }       
       
       
//       if(Statu ==0)
//       {
//         Receive_Buffer[0] =R_Data;  //上位机每次发送4个 8位数据 进行赋值
//         Statu =1;
//       }
//       else if(Statu ==1)
//       {
//         Receive_Buffer[1] =R_Data;
//         Statu =2;
//       }
//       else if(Statu ==2)
//       {
//         Receive_Buffer[2] =R_Data;
//         Statu =3;
//       }
//       else if(Statu ==3)
//       {
//         Receive_Buffer[3] =R_Data;
//         Statu =0;
//       }
//       
//       //读取接收数值
//      if( (Receive_Buffer[0] ==0x01) && (Receive_Buffer[1] ==0x01) )  //
//      {
//        Receive_P =Receive_Buffer[2] ;
//      }
//      else if( (Receive_Buffer[0] ==0x02) && (Receive_Buffer[1] ==0x02) )
//      {
//        Receive_D =Receive_Buffer[2] ;
//      }
//      else if( (Receive_Buffer[0] ==0x03) && (Receive_Buffer[1] ==0x03) )
//      {
//        R_High =Receive_Buffer[2];
//        R_Low =Receive_Buffer[3];
//        Receive_Pwm =R_High;
//        Receive_Pwm <<=R_Low;
//      }
       
      LED_Turn(LED4); 
    }
  if(Statu ==2)
  {
    KS103_Distance =R_High;
    KS103_Distance <<=8;
    KS103_Distance +=R_Low;
    Statu =0;
    Receive_Ok =1;
    KS103_Distance /=10;  //单位 CM
  }
  
  if(UART1->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
    {
        //用户需要处理发送数据
//      ANO_DT_Send_Actual(5,0,0,0,0,0,0,0,0);
//      if(Tx1Counter !=count1)
//        UART1->D =Tx1Buffer[Tx1Counter ++];
      
    }
  
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PIT2中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               进行任务规划函数
//-------------------------------------------------------------------------------------------------------------------

uint16 Time_1h,Time_1m,Time_1s,Time_1ms;
//Loop_t Loop;
void PIT2_IRQHandler(void)
{
//  static uint8 i;

  PIT_FlAG_CLR(pit2);
//  i++;
//  if(i >100) i =0;
//  if(i ==100) LED_Turn(LED3);
  
//  if( (Dajin_Finish_Flag ==1)) // && (mt9v032_finish_flag ==1)
//  {
//        image_binaryzation();    //将图像二值化    最大5ms        
//        Get_Center_Line(); //提取赛道中线      最大1ms
//        Road_Control();    //控制舵机
//        LED_Turn(LED3);
//  }
  
//  Zebra_Stop();                                 //声明测试使用
//  if(Time_1ms <999)
//  {
//    Time_1ms ++;
//    Loop_Check();
//  }
//  else
//  {
//    Time_1ms =0;
//    if(Time_1s <59)
//    {
//      Time_1s ++;
//    }
//    else
//    {
//      Time_1s =0;
//      if(Time_1m <59 )
//      {
//        Time_1m ++;
//      }
//      else
//      {
//        Time_1m =0;
//        if(Time_1h <23)
//        {
//          Time_1h ++;
//        }
//        else
//        {
//          Time_1h =0;
//        }
//      }
//    }
//  }
}




/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位


DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/
                


