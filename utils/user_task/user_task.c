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
#include "stdlib.h"
#include "string.h"
#include "simdevice.h"
#include "delay.h"
#include "parameter.h"
#include "timer_task.h"
#include "user_task.h"
#include "ov2640.h"
#include "exti.h"

#define JPEG_MAX_LEN    (1024*40)

uint8_t jpeg_buf[JPEG_MAX_LEN] = {0};
uint16_t jpeg_len = 0;

#define OV2640_JPEG_WIDTH	640
#define OV2640_JPEG_HEIGHT	480

void ov2640_speed_ctrl(void)
{
	u8 clkdiv, pclkdiv;
    
    clkdiv = 1;
    pclkdiv = 28;

	SCCB_WR_Reg(0XFF, 0X00);		
	SCCB_WR_Reg(0XD3, pclkdiv);
	SCCB_WR_Reg(0XFF, 0X01);
	SCCB_WR_Reg(0X11, clkdiv);
}

void ov2640_user_config()
{
    printf("ov264 init.....\n");
    while(ov2640_init()) {
        delay_ms(200);
        printf("tring to init ov2640\n");
    }

    printf("ov264 init finished!\n");

    OV2640_OutSize_Set(OV2640_JPEG_WIDTH, OV2640_JPEG_HEIGHT);
    OV2640_JPEG_Mode();
    ov2640_speed_ctrl();
}

int take_jpeg_func(void* arg)
{
	int i=0;
    jpeg_len = 0;

    //丢弃1帧，等待OV2640自动调节好（曝光白平衡之类的）
    while(OV2640_VSYNC == 1);	 
    while(OV2640_VSYNC == 0);	  

	while(OV2640_VSYNC == 1) {
		while(OV2640_HREF) {
			while(OV2640_PCLK==0); 
			jpeg_buf[jpeg_len]=OV2640_DATA;
			while(OV2640_PCLK==1);  
			jpeg_len++;
		}
	}

	usart_dump(USART1, jpeg_buf, jpeg_len);

    return 0;
}

timer_task_t take_jpeg_task = {
    .last_time = 0,
    .next_time = 10,
    .func = take_jpeg_func,
};

void user_task_loop()
{
    update_current_time();
    take_jpeg_func(NULL);
    //timer_task_executer(&take_jpeg_task, NULL);
}

void timer_task_loop()
{

}
