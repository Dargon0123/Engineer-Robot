/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MK60DN10_dac.h
 * @brief      		DAC函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60DN10_dac_h
#define _MK60DN10_dac_h


#include "misc.h"
#include "MK60D10.h"
#include "common.h"
#include "MK60DN10_port.h"
#include "MK60DN10_gpio.h"
#include "MK60DN10_port_cfg.h"

typedef enum DACn       //DAC模块
{
    dac0,
    dac1
} DACn;


void dac_init(DACn dacn,uint16 volta);
void dac_out(DACn dacn, uint16 volta);

#endif
