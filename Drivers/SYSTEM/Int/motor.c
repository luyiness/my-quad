//
// Created by luyi on 2026/5/24.
//

#include "motor.h"

void motor_init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

//仅测试使用
void motor_start(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 100);  //档位0～999，越大电机转越快
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 100);

}