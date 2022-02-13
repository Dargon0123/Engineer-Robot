/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		headfile.h
 * @brief      		ͷ�ļ����ܿ�
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#ifndef _headfile_h
#define _headfile_h


//--------������------------
#include "MK60D10.h"
#include "MK60DN10_port_cfg.h"
#include "common.h"
#include "misc.h"
#include "stdio.h"
#include "math.h"
#include "Ano_Link.h"

//--------MK60������--------
#include "MK60DN10_rtc.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_systick.h"
#include "MK60DN10_port.h"
#include "MK60DN10_uart.h"
#include "MK60DN10_pit.h"
#include "MK60DN10_lptmr.h"
#include "MK60DN10_ftm.h"
#include "MK60DN10_adc.h"
#include "MK60DN10_dac.h"
#include "MK60DN10_flash.h"
#include "MK60DN10_spi.h"
#include "MK60DN10_i2c.h"
#include "MK60DN10_wdog.h"
#include "MK60DN10_dma.h"
#include "MK60DN10_cmt.h"
#include "MK60DN10_sdhc.h"

#include "isr.h"

//fatfs�ļ�ϵͳ
#include "ff.h"

//--------��ɿƼ���Ʒ���̿�--------
#include "SEEKFREE_MT9V032.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_TSL1401.h"
#include "SEEKFREE_7725.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"

//--------APPӦ�ú���--------
#include "motor.h" 
#include "photo_handle.h"
#include "key.h"
#include "led.h"
#include "path_control.h"
#include "scheduler.h"

#endif
