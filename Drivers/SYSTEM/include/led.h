//
// Created by luyi on 2026/5/24.
//

#ifndef QUAD_TEST_LED_H
#define QUAD_TEST_LED_H

#include <gpio.h>
#include "FreeRTOS.h"
#include "task.h"

typedef struct
{
    GPIO_TypeDef *gpio_type;
    uint16_t pin;
    uint16_t last_tick;    //用于记录上次闪的时间点
}led_struct;

extern led_struct led1;
extern led_struct led2;
extern led_struct led3;
extern led_struct led4;

void led_init(void);
void led_on(const led_struct *led);
void led_off(const led_struct *led);
void led_toggle(const led_struct *led);
void led_blink_with_period(led_struct *led, uint16_t period);

//灯语：
//遥控状态：已连接上遥控，未连接上遥控
typedef enum
{
    CONNECTED,
    DISCONNECTED,
}remote_status_enum;

extern remote_status_enum remote_status;

//飞行状态：4个
typedef enum
{
    IDLE,
    NORMAL,
    FIX_HEIGHT,
    FAULT,
}flight_status_enum;

extern flight_status_enum flight_status;

#endif //QUAD_TEST_LED_H
