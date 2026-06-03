//
// Created by luyi on 2026/6/3.
//

#include "com_pid.h"

static void com_pid_calculate(PID_Struct *pid, float dt)
{
    float error = pid->measure - pid->desire;   // 测量值 - 目标值 = 误差

    float proportional = pid->kp * error;   //比例
    pid->integral += error * dt;        //积分
    float derivative = (error - pid->last_error) / dt;  //微分

    pid->output = proportional + pid->integral + derivative;

    pid->last_error = error;
}

//串级pid
void com_pid_cascade(PID_Struct *pid_out, PID_Struct *pid_in, float dt)
{
    com_pid_calculate(pid_out, dt);     //先计算外环
    pid_in->desire = pid_out->output;   //内环目标值=外环输出
    com_pid_calculate(pid_in, dt);      //再计算内环
}