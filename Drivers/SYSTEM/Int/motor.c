//
// Created by luyi on 2026/5/24.
//

#include "motor.h"

Motor_Struct motor_l0;
Motor_Struct motor_l1;
Motor_Struct motor_r0;
Motor_Struct motor_r1;

void motor_init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

    Motor_Struct motor_l0 = {.htim = &htim3, .channel = TIM_CHANNEL_1, .speed = 0};
    Motor_Struct motor_l1 = {&htim4, TIM_CHANNEL_4, 0};
    Motor_Struct motor_r0 = {&htim2, TIM_CHANNEL_2, 0};
    Motor_Struct motor_r1 = {&htim1, TIM_CHANNEL_3, 0};
}

//仅测试使用
void motor_start(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 100);  //档位0～999，越大电机转越快
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 100);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 100);

}

//设置4个电机的转速，若=1就不设置；
void motor_setAll(uint16_t l0, uint16_t l1, uint16_t r0, uint16_t r1)
{
    if(l0 != 1)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, l0);
    }
    if(l1 != 1)
    {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, l1);
    }
    if(r0 != 1)
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, r0);
    }
    if(r1 != 1)
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, r1);
    }
}

void motor_set(const Motor_Struct *motor)
{
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, motor->speed);
}