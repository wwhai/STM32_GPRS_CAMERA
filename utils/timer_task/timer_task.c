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

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "timer_task.h"
#include "timer.h"

uint64_t curr_time = 0;

/******************************************
 * Checking timeout and run functions
 *******************************************/

int timer_task_executer(timer_task_t* task, void* arg)
{
    int ret = -1;
    uint64_t start_time = curr_time;

    if (curr_time - task->last_time >= task->next_time &&
        task->next_time > 0) {
        ret = task->func(arg);
        if (0 > ret) {
            printf("%s:%d:execute task failed\n", __func__, __LINE__);
        } else {
            task->last_time = start_time;
        }
    }

    return ret;
}

void update_current_time()
{
    curr_time = sys_time_sec;
}
