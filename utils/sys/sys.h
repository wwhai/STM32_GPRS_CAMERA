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

#ifndef __SYS_H__
#define __SYS_H__

#include "stm32f10x.h"
#include "stdint.h"

#define SYSTEM_SUPPORT_OS		0

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //????
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //????

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //????
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //????

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //????
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //????

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //????
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //????

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //????
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //????

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //????
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //????

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //????
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //????

void WFI_SET(void);
void INTX_DISABLE(void);
void INTX_ENABLE(void);
void MSR_MSP(u32 addr);
uint16_t get_flash_size(void);

/**
 * 初始化独立看门狗
 * prer:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^prer.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 */
void watchdog_init(uint8_t prer, uint16_t rlr);

void watchdog_feed(void);

#endif
