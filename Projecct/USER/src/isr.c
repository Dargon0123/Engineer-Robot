/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr.c
 * @brief      		�жϺ�����
 * @company	   		�ɶ���ɿƼ����޹�˾
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
//  @brief      PROTA�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��A�������жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void PORTA_IRQHandler(void)
{
    //����жϱ�־��һ�ַ���ֱ�Ӳ����Ĵ�����ÿһλ��Ӧһ������
	PORTA->ISFR = 0xffffffff;
	//ʹ�����Ǳ�д�ĺ궨����������жϵ�����
	//PORTA_FLAG_CLR(A1);

}


void PORTC_IRQHandler(void)
{
    //����жϱ�־��һ�ַ���ֱ�Ӳ����Ĵ�����ÿһλ��Ӧһ������
	PORTC->ISFR = 0xffffffff;
	//ʹ�����Ǳ�д�ĺ궨����������жϵ�����
	//PORTC_FLAG_CLR(C1);
    VSYNC();
}


void DMA0_IRQHandler(void)
{
	DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
    if( (Dajin_Finish_Flag ==1) ) // && (mt9v032_finish_flag ==1)
    {
//        image_binaryzation();    //��ͼ���ֵ��    ���5ms        
        Get_Center_Line(); //��ȡ��������      ���1ms
//        Road_Control();    //���ƶ��
        LED_Turn(LED3);
    }
}


uint8 Receive_Ok =1;
extern uint8 KS_103_Finish_Flag;
extern uint8 Velacity_Mode;
void PIT0_IRQHandler(void)   //5ms�жϷ�����
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
//  Road_Control();    //���ƶ��
//  Key_Test();        //���ڿ�����ʾOLED��־λ
//  Velocity_Select();
  Get_Encoder();     //��ȡ��������ֵ
//  if(Velacity_Mode ==1)
    Motor_Control();   //���Ƶ��
//  else if(Velacity_Mode ==2 )
//    Motor_Control_Fast();
  Road_Control();    //���ƶ��
  
  
  //����������ָ��
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
//  @brief      UART3�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��UART3�����жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if(UART3->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        mt9v032_cof_uart_interrupt();
    }
    if(UART3->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������

    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART1�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��UART1�����жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú��� (����ʹ��)
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
//      systick_delay(100 *25);           //��ʱ 20us
//      uart_putchar(uart1,0x02);
//      systick_delay(100 *25);
//      uart_putchar(uart1,0x0f);        //2m
//      systick_delay(100 *25);
      Statu =0;
}
void UART1_RX_TX_IRQHandler(void)
{
//  static uint8 num =0;
  if(UART1->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
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
//         Receive_Buffer[0] =R_Data;  //��λ��ÿ�η���4�� 8λ���� ���и�ֵ
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
//       //��ȡ������ֵ
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
    KS103_Distance /=10;  //��λ CM
  }
  
  if(UART1->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
//      ANO_DT_Send_Actual(5,0,0,0,0,0,0,0,0);
//      if(Tx1Counter !=count1)
//        UART1->D =Tx1Buffer[Tx1Counter ++];
      
    }
  
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PIT2�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��������滮����
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
//        image_binaryzation();    //��ͼ���ֵ��    ���5ms        
//        Get_Center_Line(); //��ȡ��������      ���1ms
//        Road_Control();    //���ƶ��
//        LED_Turn(LED3);
//  }
  
//  Zebra_Stop();                                 //��������ʹ��
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
�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ������DMA0�жϣ�Ȼ��͵�����ȥ���ĸ���DMA0���жϺ������ƣ��ҵ���дһ�������Ƶĺ�������
void DMA0_IRQHandler(void)
{
    ;
}
�ǵý����жϺ������־λ


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
                


