/* Copyright (C) www.lightembedded.com
 * 2018 - xianlee.wu xianleewu@163.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "stdint.h"
#include "delay.h"

uint8_t  __fac_us = 0;
uint16_t __fac_ms = 0;

void delay_init()
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    __fac_us=SystemCoreClock/8000000;
    __fac_ms=(uint16_t)__fac_us*1000;
}

/* 延时nus
 * nus为要延时的us数.
 */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->LOAD=nus*__fac_us; 					//时间加载
    SysTick->VAL=0x00;        					//清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数
    do {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL =0X00;      					//清空计数器
}
/*
 * 延时nms
 * 注意nms的范围
 * SysTick->LOAD为24位寄存器,所以,最大延时为:
 * nms<=0xffffff*8*1000/SYSCLK
 * SYSCLK单位为Hz,nms单位为ms
 * 对72M条件下,nms<=1864
 */
void delay_ms(uint16_t nms)
{
#if 1
    while (nms--) {
        delay_us(1000);
    }
#else
    uint32_t temp;
    SysTick->LOAD=(uint32_t)nms*__fac_ms;	    //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00;							//清空计数器
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数
    do {
        temp=SysTick->CTRL;
    } while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
    SysTick->VAL =0X00;       					//清空计数器
#endif
}
