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

#include "stm32f10x.h"
#include "stdarg.h"
#include "usart.h"
#include "simdevice.h"

void usart1_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //Usart1 NVI
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void usart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//USART2 NVI
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//USART2_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#if 0
	//USART2_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;// | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure);

	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

void usart3_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;	

    //Usart3 NVI
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART4_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_TC, DISABLE);  //关闭发送中断 
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭接收中断
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
    USART_Cmd(USART3, ENABLE);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                      //使能DMA1时钟  
    DMA_DeInit(DMA1_Channel3);                                              //初始化DMA1通道3  串口3接收通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;                  //串口3接收数据的地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)gsm_global_data.frame_buf;      //数据存放的复制
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //数据传送方向  串口3作为数据源
    DMA_InitStructure.DMA_BufferSize = GSM_DATA_RECORD_MAX_LEN;                    //接收数据个数       
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外围设备 串口3 地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // DMA存储地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外围设备 串口3 数据宽度 8bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //DMA存储 数据宽度 8bits
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                         //循环获取数据
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                   //串口3通道优先为中等
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //关闭内存到内存
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  //使能串口3 DMA功能
    DMA_Cmd(DMA1_Channel3, ENABLE);
}

char *
itoa(n, base)
    long n;   /* abs k16 */
    int base;
{
    register char *p;
    register int minus;
    static char buf[36];
    p = &buf[36];
    *--p = '/0';
    if (n < 0) {
        minus = 1;
        n = -n;
    }
    else
        minus = 0;
    if (n == 0)
        *--p = '0';
    else
        while (n > 0) {
            *--p = "0123456789abcdef"[n % base];
            n /= base;
        }
    if (minus)
        *--p = '-';
    return p;
}

void usart_printf(USART_TypeDef* USARTx, char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];

    va_list ap;
    va_start(ap, Data);

    while (*Data != 0) {
        if (*Data == 0x5c) {
            switch (*++Data) {
                case 'r':
                    USART_SendData(USARTx, 0x0d);
                    Data++;
                    break;

                case 'n':
                    USART_SendData(USARTx, 0x0a);
                    Data++;
                    break;

                default:
                    Data++;
                    break;
            }
        } else if (*Data == '%') {
            switch (*++Data) {
                case 's':
                    s = va_arg(ap, const char *);
                    for (; *s; s++) {
                        USART_SendData(USARTx, *s);
                        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
                            ;
                    }
                    Data++;
                    break;

                case 'd':
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);
                    for (s = buf; *s; s++) {
                        USART_SendData(USARTx, *s);
                        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
                            ;
                    }
                    Data++;
                    break;
                default:
                    Data++;
                    break;
            }
        } else
            USART_SendData(USARTx, *Data++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
            ;
    }
}

void usart_dump(USART_TypeDef* USARTx, uint8_t* addr, uint32_t len)
{
    uint8_t* out =addr;
    uint32_t out_len = len;

    while(out && out_len--) {
        USART_SendData(USARTx, *(out++));
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief  This function handles USART1 Handler.
 * @param  None
 * @retval None
 */
void USART1_IRQHandler(void)
{
    char ch;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        ch = USART_ReceiveData(USART1);

        USART_SendData(USART1, ch);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }

    if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET) {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
        ch = USART_ReceiveData(USART1);
    }
}

void USART2_IRQHandler(void)
{
    char ch;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        ch = USART_ReceiveData(USART1);

        USART_SendData(USART1, ch);
        while( USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }

    if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET) {
        USART_ClearFlag(USART2, USART_FLAG_ORE);
        USART_ReceiveData(USART2);
        ch = USART_ReceiveData(USART2);
    }
}


void USART3_IRQHandler(void)
{
    char ch;

    //读数据寄存器非空
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        ch = USART_ReceiveData(USART3);
        //预留1个字节写结束符
        if (gsm_global_data.frame_len < (GSM_DATA_RECORD_MAX_LEN - 1)) {
            gsm_global_data.frame_buf[gsm_global_data.frame_len++] = ch;
        }
#ifdef AT_DEBUG
        /* 开启DEBUG会影响HTTP数据接收 */
        USART_SendData(DEBUG_USART, ch);
        while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
#endif
    }

    if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET) {
        USART_ClearFlag(USART3, USART_FLAG_ORE);
        ch = USART_ReceiveData(USART3);
        gsm_global_data.frame_len = GSM_DATA_RECORD_MAX_LEN - DMA_GetCurrDataCounter(DMA1_Channel3); //算出接本帧数据长度
        gsm_global_data.frame_buf[GSM_DATA_RECORD_MAX_LEN - 1] = '\0';
        printf("[SIMDEVICE]:%s\n", gsm_global_data.frame_buf);
        USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //清除中断标志
        DMA_Cmd(DMA1_Channel3, DISABLE );  //关闭DEBUG_USART TX DMA1 所指示的通道      
        DMA_SetCurrDataCounter(DMA1_Channel3, GSM_DATA_RECORD_MAX_LEN);//DMA通道的DMA缓存的大小
        DMA_Cmd(DMA1_Channel3, ENABLE);  //使能DEBUG_USART TX DMA1 所指示的通道 
    }
}

//******************************************************************************
// Hosting of stdio functionality through USART1
//******************************************************************************

#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
    static int last;

    if ((ch == (int)'\n') && (last != (int)'\r')) {
        last = (int)'\r';

        while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(DEBUG_USART, last);
    } else {
        last = ch;
    }

    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
        ;

    USART_SendData(DEBUG_USART, ch);

    return (ch);
}

int fgetc(FILE *f)
{
    char ch;

    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

    ch = USART_ReceiveData(DEBUG_USART);

    return((int)ch);
}

int ferror(FILE *f)
{
    /* Your implementation of ferror */
    return EOF;
}

void _ttywrch(int ch)
{
    static int last;

    if ((ch == (int)'\n') && (last != (int)'\r')) {
        last = (int)'\r';

        while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
            ;

        USART_SendData(DEBUG_USART, last);
    } else {
        last = ch;
    }

    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET)
        ;

    USART_SendData(DEBUG_USART, ch);
}

void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}
