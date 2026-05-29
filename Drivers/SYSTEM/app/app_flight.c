//
// Created by luyi on 2026/5/29.
//

#include "app_flight.h"

//speed
static int16_t left_top = 0;
static int16_t right_top = 0;
static int16_t right_down = 0;
static int16_t left_down = 0;

void app_flight_set_motors(Remote_Data *rd)
{
    switch (flight_status)
    {
    case IDLE:
        left_top = 0;
        right_top = 0;
        right_down = 0;
        left_down = 0;
        break;
    case NORMAL:
        //暂时，电机转速直接=油门大小
        left_top = rd->throttle;
        right_top = rd->throttle;
        right_down = rd->throttle;
        left_down = rd->throttle;
        break;
    case FIX_HEIGHT:
        //todo
        break;
    case FAULT:
        //电机转速缓慢降低
        if (left_top > 10)
        {
            left_top -= 10;
        }
        if (right_top > 10)
        {
            right_top -= 10;
        }
        if (right_down > 10)
        {
            right_down -= 10;
        }
        if (left_down > 10)
        {
            left_down -= 10;
        }

        //通知flight task：进行状态切换FAULT->IDLE， （在flight task中做）
        if (left_top < 10 && right_top < 10 && right_down < 10 && left_down < 10)
        {
            xTaskNotifyGive(comm_task_handle);
        }
        break;
    }

    //安全停机； 要停机时的PID计算转速减小的慢、这代码是让转速快速=0
    if (left_top < 20)
    {
        left_top =0;
    }
    if (right_top < 20)
    {
        right_top =0;
    }
    if (right_down < 20)
    {
        right_down =0;
    }
    if (left_down < 20)
    {
        left_down =0;
    }

    motor_setAll(left_top,left_down,right_top,right_down);
}
