#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "ov2640.h"
#include "user_task.h"

uint32_t frame_len = 0;

//外部中断初始化函数
void exti_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//外部中断，需要使能AFIO时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	exti_set_enable(0);
}

void exti_set_enable(int isenable)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = isenable;	//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI15_10_IRQHandler(void)
{
	jpeg_buf[frame_len]=OV2640_DATA;
	frame_len++;
	EXTI_ClearITPendingBit(EXTI_Line13);  //清除LINE15线路挂起位
	EXTI_ClearFlag(EXTI_Line13);
}
