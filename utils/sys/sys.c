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

#include "sys.h"

void WFI_SET(void)
{
	__ASM volatile("wfi");
}

void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}

void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");
}

__asm void MSR_MSP(u32 addr)
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

uint16_t get_flash_size(void)
{
    return *(u16*)(0x1FFFF7E0); //闪存容量寄存器
}

/**
 * 初始化独立看门狗
 * prer:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^prer.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 */
void watchdog_init(uint8_t prer, uint16_t rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   /* 使能对寄存器IWDG_PR和IWDG_RLR的写操作 */
    IWDG_SetPrescaler(prer);                        /* 设置IWDG预分频值:设置IWDG预分频值 */
    IWDG_SetReload(rlr);                            /* 设置IWDG重装载值 */
    IWDG_ReloadCounter();                           /* 按照IWDG重装载寄存器的值重装载IWDG计数器 */
    IWDG_Enable();                                  /* 使能IWDG */
}

/**
 * 喂独立看门狗
 */
void watchdog_feed()
{
    IWDG_ReloadCounter();    /*reload*/
}
