#ifndef __USER_TASKS_H__
#define __USER_TASKS_H__

#include "stdint.h"

void update_current_time(void);

void user_task_loop(void);

void ov2640_user_config(void);

extern uint8_t jpeg_buf[];

#endif
