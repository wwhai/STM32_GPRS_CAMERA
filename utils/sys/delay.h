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

#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__

#include "stm32f10x.h"
#include "stdint.h"

extern uint8_t  __fac_us;
extern uint16_t __fac_ms;

/*
 * Notice: Never use delay function in this range.
 */
#define time_out_loop(nms)                                                  \
  uint32_t __sys_tick__start__ = 0x00000001;					            \
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;                                  \
  SysTick->LOAD = (uint32_t)nms * __fac_ms;                                 \
  SysTick->VAL = 0x00;                                                      \
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			                        \
  for(;(__sys_tick__start__ & 0x01) && !(__sys_tick__start__ & (1 << 16));  \
  __sys_tick__start__ = SysTick->CTRL)

void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif /* _SYST_DELAY_H_ */
