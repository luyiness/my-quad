//
// Created by luyi on 2026/5/24.
//

#include "ip5305t.h"

void int_ip5305t_start(void)
{
    //模拟power-key按下
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(80);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);

}
