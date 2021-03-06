/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60DN10_cmt.h
* @brief      		CMT_PWM函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/




#ifndef _MK60DN10_cmt_h
#define _MK60DN10_cmt_h


#include "MK60D10.h"
#include "common.h"
#include "MK60DN10_port.h"




#define CMT_PRECISON 3030u     //定义占空比精度，意思即使把一个周期分成多少分，分多越多这样精度越高，设置占空比的时候duty的范围就是0-FTM0_PRECISON  最大只能设置为65535


void cmt_pwm_init(uint16 period, uint16 duty);
void cmt_pwm_duty(uint16 duty);
void cmt_pwm_freq(uint16 freq, uint16 duty);

#endif
