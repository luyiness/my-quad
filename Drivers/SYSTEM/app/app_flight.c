//
// Created by luyi on 2026/5/29.
//

#include "app_flight.h"

//speed
static int16_t left_top = 0;
static int16_t right_top = 0;
static int16_t right_down = 0;
static int16_t left_down = 0;

static int8_t scale = 10;       //比例系数；用于减少摇杆对yaw roll pitch的变化

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
        left_top = 0;
        right_top = 0;
        right_down = 0;
        left_down = 0;

        //往前飞的话：pitch增加 也就是left_down和right_down转速增加
        left_top -= rd->pitch - 500;
        right_top -= rd->pitch - 500;
        left_down += rd->pitch - 500;
        right_down += rd->pitch - 500;

        //向右横滚的话：roll增加 也就是left_up和left_down转速增加
        left_top += rd->roll - 500;
        right_top -= rd->roll- 500;
        left_down += rd->roll - 500;
        right_down -= rd->roll - 500;

        //向右偏航的话：yaw增加 也就是right_top和left_down转速增加
        left_top -= rd->yaw - 500;
        right_top += rd->yaw- 500;
        left_down += rd->yaw - 500;
        right_down -= rd->yaw - 500;

        //按比例缩小：
        left_top = left_top / scale;
        right_top = right_top / scale;
        left_down = left_down / scale;
        right_down = right_down / scale;

        //暂时，电机转速直接=油门大小
        left_top += rd->throttle;
        right_top += rd->throttle;
        right_down += rd->throttle;
        left_down += rd->throttle;

        //补偿：根据实际情况进行细微调整
        // right_down += 20;
        //...

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

static Gyro_Data g_last;

void app_flight_get_gyroscope(Gyroscope_Data *gd)
{
    taskENTER_CRITICAL();       //进入临界区
    read_gyroscope_data(gd);
    taskEXIT_CRITICAL();

    //对gyro_data进行一阶低通滤波
    gd->gyro.x = Com_Filter_LowPass(gd->gyro.x, g_last.x);
    gd->gyro.y = Com_Filter_LowPass(gd->gyro.y, g_last.y);
    gd->gyro.z = Com_Filter_LowPass(gd->gyro.z, g_last.z);
    g_last = gd->gyro;

    //对accel_data进行卡尔曼滤波
    gd->accel.x = (int16_t)Com_Filter_KalmanFilter(&kfs[0], gd->accel.x);
    gd->accel.y = (int16_t)Com_Filter_KalmanFilter(&kfs[1], gd->accel.y);
    gd->accel.z = (int16_t)Com_Filter_KalmanFilter(&kfs[2], gd->accel.z);

}

void App_Flight_Get_EulerAngle(Gyroscope_Data* gd, EulerAngle_Struct* ea, float dt)
{
    Com_IMU_GetEulerAngle(gd, ea, dt);
    
    debug_printfln("euler:%f,%f,%f",ea->yaw,ea->pitch,ea->roll);
}