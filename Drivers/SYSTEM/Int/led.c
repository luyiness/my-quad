//
// Created by luyi on 2026/5/24.
//

#include "led.h"


led_struct led1;
 led_struct led2;
 led_struct led3;
 led_struct led4;

void led_init(void)
{
    led1.gpio_type = L1_GPIO_Port;
    led2.gpio_type = L2_GPIO_Port;
    led3.gpio_type = L3_GPIO_Port;
    led4.gpio_type = L4_GPIO_Port;
    led1.pin = L1_Pin;
    led2.pin = L2_Pin;
    led3.pin = L3_Pin;
    led4.pin = L4_Pin;
}

void led_on(const led_struct *led)
{
    HAL_GPIO_WritePin(led->gpio_type, led->pin, GPIO_PIN_RESET);
}

void led_off(const led_struct *led)
{
    HAL_GPIO_WritePin(led->gpio_type, led->pin, GPIO_PIN_SET);
}

void led_toggle(const led_struct *led)
{
    HAL_GPIO_TogglePin(led->gpio_type, led->pin);
}

//灯以period为周期进行闪烁
void led_blink_with_period(led_struct *led, uint16_t period)
{
    TickType_t current_tick = xTaskGetTickCount();
    if (current_tick - led->last_tick >= period)
    {
        HAL_GPIO_TogglePin(led->gpio_type, led->pin);
        led->last_tick = current_tick;
    }
}

// void led_on(void)
// {
//     HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_SET);
//     HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_SET);
//     HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_SET);
//     HAL_GPIO_WritePin(L4_GPIO_Port, L4_Pin, GPIO_PIN_SET);
// }
//
// void led_off(void)
// {
//     HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_RESET);
//     HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_RESET);
//     HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_RESET);
//     HAL_GPIO_WritePin(L4_GPIO_Port, L4_Pin, GPIO_PIN_RESET);
// }
//
// void led_toggle(void)
// {
//     HAL_GPIO_TogglePin(L1_GPIO_Port, L1_Pin);
//     HAL_GPIO_TogglePin(L2_GPIO_Port, L2_Pin);
//     HAL_GPIO_TogglePin(L3_GPIO_Port, L3_Pin);
//     HAL_GPIO_TogglePin(L4_GPIO_Port, L4_Pin);
// }