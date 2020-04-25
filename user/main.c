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
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#include "simdevice.h"
#include "timer.h"
#include "parameter.h"
#include "user_task.h"
#include "exti.h"
#include "ov2640.h"

/*
 +---------------------------------------------------------------------+
 |                             配置说明                                |
 +---------------------------------------------------------------------+
 |       如果你想要用自己的账户，请在parameter.c里填入您的账户         |
 +-------------+-------------------------------------------------------+
 | server_addr | MQTT服务器地址                                        |
 +-------------+-------------------------------------------------------+
 | server_port | MQTT服务器端口                                        |
 +-------------+-------------------------------------------------------+
 | client_id   | MQTT的CLIENTID 可以不填                               |
 +-------------+-------------------------------------------------------+
 | username    | MQTT的用户名                                          |
 +-------------+-------------------------------------------------------+
 | password    | MQTT的用户密码                                        |
 +-------------+-------------------------------------------------------+
 | sub_topic   | 设备订阅的主题，接收来自该主题的消息                  |
 +-------------+-------------------------------------------------------+
 | pub_topic   | 设备发布的主题，发送消息到该主题                      |
 +-------------+-------------------------------------------------------+

 +-------------+-------------------------------------------------------+
 |       控制消息说明，发消息到订阅的主题可以控制继电器的状态          |
 +-------------------------------------------+-------------------------+
 | {"cmd":"relay","type":"control",data:"1"} | 继电器闭合              |
 +-------------------------------------------+-------------------------+
 | {"cmd":"relay","type":"control",data:"0"} | 继电器断开              |
 +-------------------------------------------+-------------------------+
 | {"cmd":"relay","type":"time",data:"10"}   | 继电器闭合10秒后断开    |
 +-------------------------------------------+-------------------------+
 */

int main(void)
{
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    usart1_init(921600);
    usart3_init(115200);
    timer3_init(10000, 7199);
    simdevice_gpio_init();
	exti_init();
    ov2640_gpio_init();
    
    printf("%s", sys_info);

    ov2640_user_config();

#if UPLOAD_PIC
    simdevice_reboot();
    simdevice_user_config();
    simdevice_get_imei(gsm_imei);
    printf("sim800c device %s init ok!\n", gsm_imei);
#endif

    while(1) {
        user_task_loop();
    }
}

