#ifndef __PARAM_H__
#define __PARAM_H__

#include "stdint.h"

#define SERVER_ADDR_MAX_LEN 4*32

#define USE_WATCHDOG    1
#define UPLOAD_PIC      0

extern char gsm_imei[];
extern char server_addr[];
extern char server_port[];
extern char client_id[];
extern char username[];
extern char password[];
extern char sub_topic[];
extern char pub_topic[];

typedef struct _connect_state {
	int connected;
	int error_count;
} connect_state_t;

typedef struct _system_state {
    uint32_t state;
    uint32_t startup_times;
    uint32_t sensor_param;
    char server_addr[SERVER_ADDR_MAX_LEN];
} system_state_t;

extern system_state_t sys_state;

extern connect_state_t conn_state;

extern const char sys_info[];

int save_system_state(system_state_t* state);

int recovery_system_state(system_state_t* state);

#endif
