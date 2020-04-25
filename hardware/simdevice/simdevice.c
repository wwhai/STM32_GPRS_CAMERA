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
#include "stm32f10x_pwr.h"
#include "stdio.h"
#include "stdlib.h"
#include "simdevice.h"
#include "usart.h"
#include "delay.h"
#include "parameter.h"

#define DEBUG_LEVEL_VERBOSE 4
#define DEBUG_LEVEL_DEBUG   3
#define DEBUG_LEVEL_ERROR   2
#define DEBUG_LEVEL_INFO    1

#define DEGUG_LEVEL DEBUG_LEVEL_VERBOSE
#define MAX_LOOP_TIME       1800

gsm_data_record gsm_global_data = { "\0", 0, 0};

static const char *modetbl[2] = { "TCP", "UDP" };

int simdevice_check_status()
{
    int try_times = 5;

    /* hardware sync */
    do {
        if(simdevice_send_cmd("AT", "OK", NULL, GSM_CMD_WAIT_NORMAL)) {
            return 1;
        }
    } while(try_times--);

    return 0;
}

void simdevice_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void simdevice_poweron()
{
    do {
        if(simdevice_check_status()) {
            printf("%s %d:simdevice is already power on!\n", __func__, __LINE__);
            break;
        } else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_1);
            delay_ms(400);
            GPIO_SetBits(GPIOA, GPIO_Pin_1);
            delay_ms(1200);
            printf("%s %d:simdevice power on now!\n", __func__, __LINE__);
        }
    } while(1);
}

void simdevice_reboot()
{
    simdevice_send_cmd("AT+CPOWD=1", "OK", NULL, GSM_CMD_WAIT_NORMAL);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    delay_ms(400);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
    delay_ms(1200);
    printf("%s %d:simdevice reboot now!\n", __func__, __LINE__);
}

int simdevice_strstr(const char* truevalue, const char* falsevalue, int32_t times)
{
    time_out_loop (times) {
        if(NULL != strstr((const char*)gsm_global_data.frame_buf, truevalue)) {
            return 1;
        } else if(NULL != strstr((const char*)gsm_global_data.frame_buf, falsevalue)) {
            return 0;
        }
    }

    return -1;
}

int simdevice_check_reply(const char* truevalue, const char* falsevalue, int32_t times)
{
    int ret = -1;
    uint32_t remain_time = times;

    do {
        if (remain_time > MAX_LOOP_TIME) {
            ret = simdevice_strstr(truevalue, falsevalue, MAX_LOOP_TIME);
            remain_time -= MAX_LOOP_TIME;
        } else {
            ret = simdevice_strstr(truevalue, falsevalue, remain_time);
            remain_time -= remain_time;
        }

        if (ret == 0) {
            printf("\nsim800c return false!\n");
            return 0;
        } else if (ret == 1) {
            printf("\nsim800c return true!\n");
            return 1;
        }
    } while (remain_time > 0);

    printf("\nsim800c return time out!\n");

    return 0;
}

int simdevice_send_cmd (const char *cmd, const char* expect, const char* error, int32_t waittime)
{
    memset(&gsm_global_data, 0, sizeof(gsm_data_record));
    memset(gsm_global_data.frame_buf, 0, GSM_DATA_RECORD_MAX_LEN);
    gsm_global_data.frame_len = 0;

#ifdef AT_DEBUG
    printf("[DEBUG]: send cmd:%s\n", cmd);
#endif
    if((uint32_t)cmd < 0xFF) {
        simdevice_send_byte((uint32_t)cmd);
    } else {
        usart_dump(GSM_UART, (uint8_t*)cmd, strlen(cmd));
        simdevice_usart("\r\n", cmd );
    }

    delay_ms(50);

    return simdevice_check_reply(expect, error, waittime);
}

int simdevice_send_default (const char *cmd, int32_t waittime)
{
    return simdevice_send_cmd(cmd, DEFAULT_EXPECT, DEFAULT_ERROR, waittime);
}

int simdevice_get_imei(char* imei)
{
    memset(&gsm_global_data, 0, sizeof(gsm_data_record));
    gsm_global_data.frame_len = 0;

    while (!simdevice_send_default("AT+CGSN", GSM_CMD_WAIT_LONG));

    delay_ms(200);

    sscanf(gsm_global_data.frame_buf, "\n%s\n", imei);
    
    printf("Got IMEI:%s\n", imei);

    if (strlen(imei) < 10) {
        simdevice_get_imei(imei);
    }
    
    return 0;
}

int simdevice_net_init()
{
    if(!simdevice_send_cmd(BEARER_PROFILE_GPRS, "OK", "ALREADY", GSM_CMD_WAIT_LONG)) {
        return 0;
    }

    return 0;
}

int simdevice_close_net()
{
    int ret = 0;
    ret = simdevice_send_cmd("AT+CIPCLOSE=1", "CLOSE OK", NULL, GSM_CMD_WAIT_LONG);
    ret = simdevice_send_cmd("AT+CIPSHUT", "SHUT OK", NULL, GSM_CMD_WAIT_LONG);

    return ret;
}

int simdevice_check_connection()
{
    if (!simdevice_send_cmd("AT+CIPSTATUS", "CONNECT OK", DEFAULT_ERROR, GSM_CMD_WAIT_LONG)) {
        return 0;
    } else {
        return 1;
    }
}

int simdevice_tcpudp_connect(uint8_t mode, const char* ipaddr, const char* port)
{
    char net_info[256] = {0};

    snprintf(net_info, 256, "AT+CIPSTART=\"%s\",\"%s\",\"%s\"", modetbl[mode],
             ipaddr, port);

    if(!simdevice_send_cmd(net_info, "ALREADY", DEFAULT_ERROR, GSM_CMD_WAIT_LONG)) {
        return 0;
    }

    if (!simdevice_check_connection()) {
        return 0;
    }

    return 1;
}

/* assume network status was connected */
int simdevice_net_send(char* data)
{
    if (simdevice_send_cmd("AT+CIPSEND", ">", DEFAULT_ERROR, GSM_CMD_WAIT_LONG) > 0) {
        simdevice_usart("%s", data);
        delay_us(100);
        if (simdevice_send_cmd((char*) 0X1A, "OK", DEFAULT_ERROR, GSM_CMD_WAIT_LLONG)) {
            printf("%s:%d,send success!\n", __func__, __LINE__);
            return 1;
        } else {
            printf("%s:%d,send failed!\n", __func__, __LINE__);
            return -1;
        }
    } else {
        simdevice_send_default((char*) 0X1B, GSM_CMD_WAIT_LONG);	//ESC,取消发送
    }

    return -1;
}

int simdevice_send_raw(uint8_t *data, uint16_t len)
{
    if (simdevice_send_cmd("AT+CIPSEND", ">", DEFAULT_ERROR, GSM_CMD_WAIT_LONG)) {
        delay_us(500);
        usart_dump(GSM_UART, data, len);
        delay_us(50);
        if (simdevice_send_cmd((char*) 0X1A, "OK", DEFAULT_ERROR, GSM_CMD_WAIT_LLONG)) {
            printf("%s:%d,send success!\n", __func__, __LINE__);
            return 1;
        } else {
            printf("%s:%d,send failed!\n", __func__, __LINE__);
            return -1;
        }
    } else {
        simdevice_send_cmd((char*) 0X1B, DEFAULT_EXPECT, "FAIL", GSM_CMD_WAIT_LONG);	//ESC,取消发送
    }

    return -1;
}

int simdevice_http_init()
{
    int ret = -1;

    simdevice_send_cmd("AT+HTTPTERM", NULL, NULL, GSM_CMD_WAIT_NONE);

    /* init network before init http */
    simdevice_send_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", NULL, GSM_CMD_WAIT_NONE);
    ret = simdevice_check_reply("OK", "ERROR", GSM_CMD_WAIT_LONG);
    if(ret) {
        printf("Success to set to AT+SAPBR=3,1,GPRS!\n");
    } else {
        // actualy we already init network in tcp step
        printf("Failed to set to AT+SAPBR=3,1,GPRS!\n");
        return -1;
    }

    simdevice_send_cmd("AT+SAPBR=3,1,\"APN\",\"CMNET\"", "OK", NULL, GSM_CMD_WAIT_NONE);
    ret = simdevice_check_reply("OK", "ERROR", GSM_CMD_WAIT_LONG);
    if(ret) {
        printf("Success to set to AT+SAPBR=3,1,APN!\n");
    } else {
        printf("Failed to set to AT+SAPBR=3,1,APN!\n");
        return -1;
    }

    simdevice_send_cmd("AT+SAPBR=1,1", "OK", NULL, GSM_CMD_WAIT_NONE);
    ret = simdevice_check_reply("OK", "ERROR", GSM_CMD_WAIT_LONG);
    if(ret) {
        printf("Success to set to AT+SAPBR=1,1!\n");
    } else {
        printf("Failed to set to AT+SAPBR=1,1!\n");
        return -1;
    }

    do {
        ret = simdevice_send_cmd("AT+HTTPINIT", "OK", NULL, GSM_CMD_WAIT_SHORT);
        if(ret) {
            printf("Success to init to http!\n");
            break;
        } else {
            printf("Failed to init to http!\n");
        }
    } while(1);

    return 0;
}

int simdevice_http_get(char* url)
{
    char url_at[2048] = {0};
    int ret = -1;
    int i = 0;
    int ret_start = 0;

    //simdevice_send_cmd("AT+HTTPPARA=\"CONTENT\",\"application/octet-stream\"", "OK", NULL, GSM_CMD_WAIT_SHORT);
    simdevice_send_cmd("AT+HTTPPARA=\"CID\",1", "OK", NULL, GSM_CMD_WAIT_NONE);

    //将请求的url格式化到命令中
    snprintf(url_at, 2048, "AT+HTTPPARA=\"URL\",\"http://%s\"", url);
    printf("%s %d:%s\n", __func__, __LINE__, url_at);

    ret = simdevice_send_cmd(url_at, "OK", NULL, GSM_CMD_WAIT_NONE);
    if(simdevice_check_reply("OK", "ERROR", GSM_CMD_WAIT_LONG)) {
        printf("%s %d:Success to connect to server!\n", __func__, __LINE__);
    } else {
        printf("%s %d:Failed to connect to server!\n", __func__, __LINE__);
        return -1;
    }

    memset(gsm_global_data.frame_buf, 0, GSM_DATA_RECORD_MAX_LEN);
    simdevice_send_cmd("AT+HTTPACTION=0", "OK", NULL, GSM_CMD_WAIT_NONE);

    // waiting for action cmd return
    printf("\nDowloading....");
    do {
        if((bool) strstr((const char*)gsm_global_data.frame_buf, "+HTTPACTION")) {
            printf("\n%s %d:HTTP GET finished!\n", __func__, __LINE__);
            break;
        } else if((bool) strstr((const char*)gsm_global_data.frame_buf, "ERROR")) {
            return -1;
        } else {
            //printf(">");
            //delay_ms(100);
        }
    } while(1);

    for(i=0;i<GSM_DATA_RECORD_MAX_LEN - 20;i++) {
        if(gsm_global_data.frame_buf[i] == '+' &&
           gsm_global_data.frame_buf[i+1] == 'H' &&
           gsm_global_data.frame_buf[i+2] == 'T' &&
           gsm_global_data.frame_buf[i+3] == 'T' &&
           gsm_global_data.frame_buf[i+4] == 'P') {
            ret_start = i;
            printf("%s %d:Found data start pos!\n", __func__, __LINE__);
            break;
        }
    }

    //寻找200OK返回，没有的话就返回错误
#ifdef GSM_DEVICE_SIM800
    if(gsm_global_data.frame_buf[ret_start+15] == '2' &&
       gsm_global_data.frame_buf[ret_start+16] == '0' &&
       gsm_global_data.frame_buf[ret_start+17] == '0') {
        printf("Http return 200 OK!\n");
        ret = 0;
    } else {
        printf("Http return error code!\n");
        ret = -1;
    }
#else
    if(gsm_global_data.frame_buf[ret_start+14] == '2' &&
       gsm_global_data.frame_buf[ret_start+15] == '0' &&
       gsm_global_data.frame_buf[ret_start+16] == '0') {
        printf("Http return 200 OK!\n");
        ret = 0;
    } else {
        printf("Http return error code!\n");
        ret = -1;
    }
#endif

    return ret;
}

int simdevice_http_read(int start, int size, char** data)
{
    int ret = -1;
    int i = 0;
    int read_len = 0;
    int ret_start = 0;
    char read_range[48] = {0};
    char cmd_str[16] = {0};

    snprintf(read_range, 48, "AT+HTTPREAD=%d,%d", start, size);

    do {
        gsm_global_data.frame_len = 0;
        if(size == 0) {
            ret = simdevice_send_cmd("AT+HTTPREAD", "+HTTPREAD", NULL, GSM_CMD_WAIT_NONE);
        } else {
            ret = simdevice_send_cmd(read_range, "+HTTPREAD", NULL, size ? (size/10):2000);
            if(simdevice_check_reply("+HTTPREAD", "ERROR", GSM_CMD_WAIT_LONG)) {
                ret = 1;
            } else {
                ret = -1;
            }
        }

        if(ret) {
            printf("Success to read data from server!\n");
            break;
        } else {
            printf("Failed to read data, retry...!\n");
            delay_ms(1000);
        }
    } while(1);

    for(i=0;i<GSM_DATA_RECORD_MAX_LEN - 20;i++) {
        if(gsm_global_data.frame_buf[i] == '+' &&
           gsm_global_data.frame_buf[i+1] == 'H' &&
           gsm_global_data.frame_buf[i+2] == 'T' &&
           gsm_global_data.frame_buf[i+3] == 'T' &&
           gsm_global_data.frame_buf[i+4] == 'P') {
            ret_start = i;
            printf("Found data start pos!\r\n");
            break;
        }
    }

    gsm_global_data.frame_buf[GSM_DATA_RECORD_MAX_LEN -1] = '\0';

    // 获取读取到的数据长度信息
    sscanf(gsm_global_data.frame_buf+ret_start, "%[^:]:%d", cmd_str, &read_len);
    // 长度信息所占长度
    i = snprintf(cmd_str, 16, "%d", read_len);

    // +HTTPREAD: xxxx
    *data = gsm_global_data.frame_buf+ret_start + 1 + 9 + 1 + i + 1;
#ifdef GSM_DEVICE_SIM800
    *data = gsm_global_data.frame_buf+ret_start + 1 + 9 + 1 + i + 1 + 1;
#endif

    printf("Read http data pos:%p,len:%d\n", *data, read_len);

    return read_len;
}

int simdevice_read_tcp(uint8_t** data, uint16_t len)
{
    /* read simdevice data in manual */
    return simdevice_send_cmd("AT+CIPRXGET=2,1460", "OK", "ERROR", GSM_CMD_WAIT_NORMAL);
}

/**
 * get simdevice bat voltage, return mV
 */
int simdevice_get_voltage()
{
    int voltage = 0;

    memset(&gsm_global_data, 0, sizeof(gsm_data_record));
    gsm_global_data.frame_len = 0;

    while(!simdevice_send_cmd("AT+CBC", "+CBC", DEFAULT_ERROR, GSM_CMD_WAIT_LONG));

    delay_ms(200);

    sscanf(gsm_global_data.frame_buf, "%*[^,],%*[^,],%d%*[^\n]\n%*s", &voltage);

    printf("[||||||18650:%d mv||||||||||]o\n", voltage);

    return voltage;
}

void simdevice_user_config()
{
    simdevice_gpio_init();
    simdevice_poweron();

    /* set simdevice baurd rate */
    simdevice_send_default("AT+IPR=115200", GSM_CMD_WAIT_NORMAL);
    
    /* simdevice close eho */
    while(0 >= simdevice_send_default("ATE0", GSM_CMD_WAIT_LONG)) {
        delay_ms(200);
    }

    /* simdevice check simcard */
    while(!simdevice_send_default("AT+IFC=0,0", GSM_CMD_WAIT_LONG)) {
        delay_ms(100);
    }

    /* simdevice reg */
    while(!simdevice_send_cmd("AT+CREG?", "0,1", NULL, GSM_CMD_WAIT_SHORT)) {
        delay_ms(500);
    }

#if USE_WATCHDOG
     watchdog_feed();
#endif

    /* simdevice check gsm service */
    while(!simdevice_send_default("AT+CSPN?", GSM_CMD_WAIT_LONG)) {
        delay_ms(100);
    }

    /* read simdevice data in manual */
    simdevice_send_default("AT+CIPRXGET=1", GSM_CMD_WAIT_LONG);

    printf("simdevice core init OK!\n");

    simdevice_close_net();

    simdevice_net_init();

    /* simdevice reg */
    while(!simdevice_send_cmd("AT+CGATT?", "1", DEFAULT_ERROR, GSM_CMD_WAIT_LONG)) {
        delay_ms(300);
    }
    
    simdevice_send_default("AT+CIICR", GSM_CMD_WAIT_LONG);
    simdevice_send_default("AT+CIFSR", GSM_CMD_WAIT_LONG);
}

int get_gps_info(double* lng, double* lat)
{
    char* gps_start = NULL;
    char* info_p = NULL;

    simdevice_send_cmd("AT+CGNSINF", "+CGNSINF", DEFAULT_ERROR, GSM_CMD_WAIT_LONG);

    gps_start = strstr(gsm_global_data.frame_buf, "+CGNSINF: 1,1,");

    if (gps_start == NULL) {
        return -1;
    }

    info_p = strstr(gps_start, "\x0d\x0a");

    if (info_p == NULL) {
        return -1;
    }

    *info_p = '\0';
    info_p = strtok((gps_start), ",");
    info_p = (char*)strtok(NULL, ",");
    info_p = (char*)strtok(NULL, ",");
    //printf("--->%s\n", info_p);
    info_p = (char*)strtok(NULL, ",");
    sscanf(info_p, "%lf", lng);
    //printf("lng--->%f\n", *lng);
    info_p = (char*)strtok(NULL, ",");
    sscanf(info_p, "%lf", lat);
    //printf("lat--->%lf\n", *lat);
    info_p = (char*)strtok(NULL, ",");
    //printf("height--->%s\n", info_p);

    return 0;
}

int simdevice_http_post(const char *uri, const char *body, int len, char *resp)
{
    char httpData[32];

    snprintf(httpData, 32, HTTP_DATA, len, 10000);
    if (!simdevice_send_cmd(httpData, DOWNLOAD, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }

    delay_ms(1000);

    usart_dump(GSM_UART, (uint8_t*)body, len);

    delay_ms(200);

    if (!simdevice_send_cmd(HTTP_POST, HTTP_200, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }

    return -1;
}

int simdevice_http_session_set(const char *uri)
{
    int ret = 0;
    char httpPara[1024*8];
    bool https = false;

    snprintf(httpPara, 1024*8, HTTP_PARA, uri);

    if (!simdevice_send_cmd(HTTP_CID, OK, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }

    if (!simdevice_send_cmd(httpPara, OK, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }

    https = strncmp(HTTPS_PREFIX, uri, strlen(HTTPS_PREFIX)) == 0;

    if (!simdevice_send_cmd(https ? HTTPS_ENABLE : HTTPS_DISABLE, OK, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }
    /*
       if (!simdevice_send_cmd(HTTP_CONTENT, OK, NULL, GSM_CMD_WAIT_LONG)) {
       return -1;
       }
       */
    return ret;
}

int simdevice_http_param_set(const char* params)
{
    char httpPara[128];

    snprintf(httpPara, 128, HTTP_USER_DATA, params);

    if (!simdevice_send_cmd(httpPara, OK, NULL, GSM_CMD_WAIT_LONG)) {
        return -1;
    }

    return 0;
}

//连接到TCP服务器
int connect_to_server(const char* ip, const char* port, int timeout)
{
    int times = 0;

    /* connect to update server */
    do {
        times++;
        if(!simdevice_tcpudp_connect(0, ip, port)) {
            printf("Failed to connect to server %d times!\n", times);
        } else {
            printf("Success to connect to server!\n");
            return 1;
        }
        delay_ms(100);
    } while(times < timeout);

    return 0;
}

int simdevice_read_resp(char *response)
{

    return 0;
}
