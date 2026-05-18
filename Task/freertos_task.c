//
// Created by luyi on 2026/5/18.
//

#include "include/freertos_task.h"

TaskHandle_t printf_task_handle;

void printf_task(void *pvParameters)
{
    uint32_t tick = xTaskGetTickCount();        //获取OS的时间
    while (1)
    {
        debug_printfln("Hello World");
        vTaskDelayUntil(&tick, printf_task_period);
    }
}

void Freertos_start(void)
{
    xTaskCreate(printf_task, "printf_task", printf_task_stacksize, NULL,
        printf_task_priority, &printf_task_handle);

    vTaskStartScheduler();
}