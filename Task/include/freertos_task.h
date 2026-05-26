//
// Created by luyi on 2026/5/18.
//

#ifndef QUAD_TEST_TASK_H
#define QUAD_TEST_TASK_H

#include "Com_debug.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ip5305t.h"
#include "motor.h"
#include "util.h"
#include "led.h"
#include "SI24R1.h"

//printf task
void printf_task(void *pvParameters);
#define printf_task_priority 2
#define printf_task_stacksize 128
#define printf_task_period 1000     //delay time

//power task
void power_task(void *pvParameters);
#define power_task_priority 3
#define power_task_stacksize 128
#define power_task_period 1000      //可设<24s，这里设为10s

//飞控 task
void flight_task(void *pvParameters);
#define flight_task_priority 4      //飞控任务和通讯任务是此项目的核心两个任务，他俩的调度应该是"此起彼伏"的(飞控执行一次 然后通讯执行一次 循环)
#define flight_task_stacksize 128
#define flight_task_period 6    //根据飞控程序调试的经验得出，是完成pid计算的最小周期时间

//灯控 task
void led_task(void *pvParameters);
#define led_task_priority 3
#define led_task_stacksize 128
#define led_task_period 50    //相当于灯闪的周期

//communication task
void communicate_task(void *pvParameters);
#define communicate_task_priority 4
#define communicate_task_stacksize 128
#define communicate_task_period 6

void Freertos_start(void);


#endif //QUAD_TEST_TASK_H
