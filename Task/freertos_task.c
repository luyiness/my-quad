//
// Created by luyi on 2026/5/18.
//

#include "include/freertos_task.h"

TaskHandle_t printf_task_handle;
TaskHandle_t power_task_handle;
TaskHandle_t flight_task_handle;
TaskHandle_t led_task_handle;
TaskHandle_t communicate_task_handle;

void printf_task(void *pvParameters)
{
    uint32_t tick = xTaskGetTickCount();        //获取OS的时间
    while (1)
    {
        // debug_printfln("Hello World");
        lcd_show_string(15, 110, 200, 24, 24, "printf_task ...", BLUE);
        vTaskDelayUntil(&tick, printf_task_period);
    }
}

void power_task(void *pvParameters)
{
    //定时启动一次IP5305T，防止自动关机
    uint32_t tick = xTaskGetTickCount();
    while (1)
    {
        int_ip5305t_start();
        vTaskDelayUntil(&tick, power_task_period);
    }
}

void flight_task(void *pvParameters)
{
    motor_init();
    uint32_t tick = xTaskGetTickCount();
    while (1)
    {
        // motor_start();
        motor_set(&motor_l0);
        vTaskDelayUntil(&tick, flight_task_period);
    }
}

//暂时赋值、用于测试-------
remote_status_enum remote_status = CONNECTED;
flight_status_enum flight_status = IDLE;
//----------------------

void led_task(void *pvParameters)
{
    uint32_t tick = xTaskGetTickCount();
    while (1)
    {
        switch (remote_status) {
        case CONNECTED:
            led_on(&led1);
            led_on(&led2);
            break;
        case DISCONNECTED:
            led_off(&led1);
            led_off(&led2);
            break;
        default:
            break;
        }
        switch (flight_status) {
        case IDLE:
            led_blink_with_period(&led3, 500);
            led_blink_with_period(&led4, 500);
            break;
        case NORMAL:
            led_blink_with_period(&led3, 100);
            led_blink_with_period(&led4, 100);
            break;
        case FIX_HEIGHT:
            led_on(&led3);
            led_on(&led4);
            break;
        case FAULT:
            led_off(&led3);
            led_off(&led4);
            break;
        }
        vTaskDelayUntil(&tick, led_task_period);
    }
}

void communicate_task(void *pvParameters)
{
    Int_SI24R1_Init();
    uint32_t tick = xTaskGetTickCount();
    while (1)
    {
        
        vTaskDelayUntil(&tick, communicate_task_period);
    }
}

void Freertos_start(void)
{
    xTaskCreate(printf_task, "printf_task", printf_task_stacksize, NULL,
        printf_task_priority, &printf_task_handle);

    //power
    xTaskCreate(power_task, "power_task", power_task_stacksize, NULL,
        power_task_priority, &power_task_handle);

    //flight
    xTaskCreate(flight_task, "flight_task", flight_task_stacksize, NULL,
        flight_task_priority, &flight_task_handle);

    //led
    led_init();
    xTaskCreate(led_task, "led_task", led_task_stacksize, NULL,
        led_task_priority, &led_task_handle);

    //communicate
    xTaskCreate(communicate_task, "communicate_task", communicate_task_stacksize, NULL,
        communicate_task_priority, &communicate_task_handle);

    vTaskStartScheduler();
}