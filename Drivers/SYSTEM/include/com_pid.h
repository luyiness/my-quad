//
// Created by luyi on 2026/6/3.
//

#ifndef QUAD_TEST_COM_PID_H
#define QUAD_TEST_COM_PID_H

typedef struct
{
    float kp;         // 比例增益  恢复是否及时  过大会震荡 过小恢复慢
    float ki;         // 积分增益  消除稳态误差
    float kd;         // 微分增益  消除过调
    float desire;     // 目标值
    float measure;    // 测量值
    float integral;   // 误差积分
    float last_error; // 上一次的误差
    float output;
} PID_Struct;

void com_pid_cascade(PID_Struct *pid_out, PID_Struct *pid_in, float dt);

#endif //QUAD_TEST_COM_PID_H
