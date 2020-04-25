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

#ifndef SIM900A_SIM900A_H_
#define SIM900A_SIM900A_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"
#include "usart.h"

/*
 * 开启此宏的话GSM模块返回的信息也会从调试串口输出
 * 建议在调试期间开启，使用时关闭
 */
#define AT_DEBUG

#define BEARER_PROFILE_GPRS         "AT+SAPBR=3,1,\"Contype\",\"GPRS\""
#define BEARER_PROFILE_APN          "AT+SAPBR=3,1,\"APN\",\"%s\""
#define QUERY_BEARER                "AT+SAPBR=2,1"
#define OPEN_GPRS_CONTEXT           "AT+SAPBR=1,1"
#define CLOSE_GPRS_CONTEXT          "AT+SAPBR=0,1"
#define HTTP_INIT                   "AT+HTTPINIT"
#define HTTP_CID                    "AT+HTTPPARA=\"CID\",1"
#define HTTP_PARA                   "AT+HTTPPARA=\"URL\",\"%s\""
#define HTTP_USER_DATA              "AT+HTTPPARA=\"USERDATA\",\"%s\""
#define HTTP_GET                    "AT+HTTPACTION=0"
#define HTTP_POST                   "AT+HTTPACTION=1"
#define HTTP_DATA                   "AT+HTTPDATA=%d,%d"
#define HTTP_READ                   "AT+HTTPREAD"
#define HTTP_CLOSE                  "AT+HTTPTERM"
#define HTTP_CONTENT                "AT+HTTPPARA=\"CONTENT\",\"application/json\""
#define HTTPS_ENABLE                "AT+HTTPSSL=1"
#define HTTPS_DISABLE               "AT+HTTPSSL=0"
#define NORMAL_MODE                 "AT+CFUN=1,1"
#define REGISTRATION_STATUS         "AT+CREG?"
#define SIGNAL_QUALITY              "AT+CSQ"
#define READ_VOLTAGE                "AT+CBC"
#define SLEEP_MODE                  "AT+CSCLK=1"

#define OK                          "OK"
#define DOWNLOAD                    "DOWNLOAD"
#define HTTP_200                    ",200,"
#define HTTPS_PREFIX                "https://"
#define CONNECTED                   "+CREG: 0,1"
#define BEARER_OPEN                 "+SAPBR: 1,1"

#define DEFAULT_EXPECT  "OK"
#define DEFAULT_ERROR   "ERROR"

#define GSM_DEVICE_SIM800								/* 使用的sim800系列开启此宏 */
#define GSM_DATA_RECORD_MAX_LEN		(2*1024)			/* GSM模块接收缓冲区长度 	*/
#define GSM_IMEI_LEN				(15 + 1)			/* IMEI长度 				*/
#define GSM_CMD_WAIT_NONE			(10)				/* 发送指令不等待结果 		*/
#define GSM_CMD_WAIT_SHORT			(100)				/* 等待100ms 				*/
#define GSM_CMD_WAIT_NORMAL			(500)				/* 等待500ms 				*/
#define GSM_CMD_WAIT_LONG			(1800)				/* 等待2s 					*/
#define GSM_CMD_WAIT_LLONG			(4500)				/* 等待2s 					*/
#define GSM_UART					(USART3)			/* GSM模块连接在串口2上     */

#define	simdevice_usart(fmt, ...)	    usart_printf(GSM_UART, fmt, ##__VA_ARGS__)
#define simdevice_send_byte(byte)		USART_SendData(GSM_UART, byte)

typedef struct _gsm_data_record {
	char frame_buf[GSM_DATA_RECORD_MAX_LEN];
	int frame_len;
	char finished;
} gsm_data_record;

extern gsm_data_record gsm_global_data;
/* 发送命令并等待返回 */
bool simdevice_cmd_with_reply(const char * cmd, const char * reply1,
		const char * reply2, uint32_t waittime);
/* 进行网络连接 */
int simdevice_tcpudp_connect(uint8_t mode, const char* ipaddr, const char* port);
/* 获取模块的IMEI串号 */
int simdevice_get_imei(char* imei);
/* GPRS 网络初始化 */
int simdevice_net_init(void);
/* 关闭网络连接 */
int simdevice_close_net(void);
/* IO口初始化 */
void simdevice_gpio_init(void);
/* 检查模块是否已经开启 */
int simdevice_check_status(void);
/* 模块开机 */
void simdevice_poweron(void);
/* 模块关机 */
void simdevice_poweroff(void);
/* 网络发送数据 */
int simdevice_net_send(char* data);
/* HTTP 初始化 */
int simdevice_http_init(void);
/* 执行http get请求 */
int simdevice_http_get(char* url);
/* 读取HTTP返回数据 */
int simdevice_http_read(int start, int size, char** data);
/* 发送数据 */
int simdevice_send_raw(uint8_t* data, uint16_t len);
/* 检查TCP连接 */
int simdevice_check_connection(void);
/* 默认初始化设置流程 */
void simdevice_user_config(void);
/* 读取接收到的TCP消息 */
int simdevice_read_tcp(uint8_t** data, uint16_t len);
/* 获取GPS地理信息(带有GPS功能的模块sim868之类) */
int get_gps_info(double* lng, double* lat);

int read_from_simdevice(void);
/* HTTP会话设置  */
int simdevice_http_session_set(const char *uri);
/* 设置http协议相关参数 */
int simdevice_http_param_set(const char* params);
/* 发起http post 请求 */
int simdevice_http_post(const char *uri, const char *body, int len, char *resp);
/* 检查返回值  */
int simdevice_check_reply(const char* truevalue, const char* falsevalue, int32_t times);
/* 发送命令并自定义并检查给定的返回值 */
int simdevice_send_cmd (const char *cmd, const char* expect, const char* error, int32_t waittime);
/* 发送命令等待标准返回值:OK,ERROR */
int simdevice_send_default (const char *cmd, int32_t waittime);
/* 连接到TCP服务器  */
int connect_to_server(const char* ip, const char* port, int timeout);
/* 模块软重启  */
void simdevice_reboot(void);
/* 获取供电电压  */
int simdevice_get_voltage(void);

#endif /* SIM900A_SIM900A_H_ */
