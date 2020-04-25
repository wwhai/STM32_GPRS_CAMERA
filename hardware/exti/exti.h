#ifndef __EXTI_H__
#define __EXTI_H__

#include "sys.h"
#include "stdint.h"

extern uint32_t frame_len;

void exti_init(void);
void exti_set_enable(int isenable);

#endif

