#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "simdevice.h"
#include "parameter.h"
#include "stmflash.h"

/*-----------------------|FLASH BASE|PROGRAM|USER DATA|*/
#define PARAM_ADDR      (0x08000000 + 0x10000 - 0x2000)

/* 需要在这里更改服务器地址端口，用户名密码等信息 */
char gsm_imei[16] = {0};
char server_addr[128] = {"relay.mqtt.iot.gz.baidubce.com"};
char server_port[8] = "1883";
char client_id[128] = {0};
char username[128] = {"relay/relay"};
char password[128] = {"Zqu9im83wlVp8++FgyVJb9d+J4EKd4ytXpa9w02N2hE="};
char sub_topic[64] = {"relay"};
char pub_topic[64] = {"topic_server"};
/*************************************************/


const char sys_info[] = {
"     _      _         _      _    _____             _                _      _            _   \n"
"    | |    (_)       | |    | |  |  ___|           | |              | |    | |          | |  \n"
"    | |     _   __ _ | |__  | |_ | |__   _ __ ___  | |__    ___   __| |  __| |  ___   __| |  \n"
"    | |    | | / _` || '_ \\ | __||  __| | '_ ` _ \\ | '_ \\  / _ \\ / _` | / _` | / _ \\ / _` |  \n"
"    | |____| || (_| || | | || |_ | |___ | | | | | || |_) ||  __/| (_| || (_| ||  __/| (_| |  \n"
"    \\_____/|_| \\__, ||_| |_| \\__|\\____/ |_| |_| |_||_.__/  \\___| \\__,_| \\__,_| \\___| \\__,_|  \n"
"               __/  |                                                                         \n"
"               |____/                                                                        \n"
"                                                                                             \n"
    "      Powered by http://softhardware.taobao.com  http://wiki.lightembedded.com 2018-08-29\n"
};

connect_state_t conn_state = {0, 0};

system_state_t sys_state = {
    .state = 2,
    .startup_times = 0,
    .sensor_param = 0,
    .server_addr = "www",
};

int save_system_state(system_state_t* state)
{
    STMFLASH_Write(PARAM_ADDR, (uint16_t*)state, sizeof(system_state_t)/2); //16bit one time

    return 0;
}

int recovery_system_state(system_state_t* state)
{
    STMFLASH_Read(PARAM_ADDR, (uint16_t*)state, sizeof(system_state_t)/2);
    state->startup_times++;

    return 0;
}

/**
 * 初始化独立看门狗
 * prer:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^prer.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 */
void IWDG_Init(u8 prer, u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
    IWDG_SetPrescaler(prer);    /*设置IWDG预分频值:设置IWDG预分频值*/
    IWDG_SetReload(rlr);     /*设置IWDG重装载值*/
    IWDG_ReloadCounter();    /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
    IWDG_Enable();        /*使能IWDG*/
}

/**
 * 喂独立看门狗
 */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();    /*reload*/
}
