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

#include "stdio.h"
#include "sys.h"

#ifndef __USART_H__
#define __USART_H__

#define DEBUG_USART			USART1		//指定调试串口

extern u16 USART_RX_STA;        //接收状态标记
extern u16 USART_RX_CNT;				//接收的字节数

void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void usart_printf(USART_TypeDef* USARTx, char *Data, ...);
void usart_dump(USART_TypeDef* USARTx, uint8_t* addr, uint32_t len);

#endif /* USART_USART_H_ */
