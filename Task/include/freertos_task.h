//
// Created by luyi on 2026/5/18.
//

#ifndef QUAD_TEST_TASK_H
#define QUAD_TEST_TASK_H

#include "Com_debug.h"

#include "FreeRTOS.h"
#include "task.h"

//printf task
void printf_task(void *pvParameters);
#define printf_task_priority 2
#define printf_task_stacksize 128
#define printf_task_period 1000     //delay time


void Freertos_start(void);


#endif //QUAD_TEST_TASK_H
