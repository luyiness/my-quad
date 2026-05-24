//
// Created by luyi on 2026/5/24.
//

#ifndef QUAD_TEST_MOTOR_H
#define QUAD_TEST_MOTOR_H

#include "tim.h"
void motor_init(void);

void motor_start(void);

typedef struct{
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint16_t speed;
}Motor_Struct;

extern Motor_Struct motor_l0;
extern Motor_Struct motor_l1;
extern Motor_Struct motor_r0;
extern Motor_Struct motor_r1;

void motor_setAll(uint16_t l0, uint16_t l1, uint16_t r0, uint16_t r1);
void motor_set(const Motor_Struct *motor);


#endif //QUAD_TEST_MOTOR_H
