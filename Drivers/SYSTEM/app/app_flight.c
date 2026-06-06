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

//pid
static PID_Struct pid_pitch = {.kp=0.0f,.ki=0.0f,.kd=0.0f};        //pitch角
static PID_Struct pid_pitch_gyro = {.kp=0.0f,.ki=0.0f,.kd=0.0f};   //pitch的角速度，就是以y为轴转的速度

static PID_Struct pid_roll = {.kp=0.0f,.ki=0.0f,.kd=0.0f};         //roll
static PID_Struct pid_roll_gyro = {.kp=0.0f,.ki=0.0f,.kd=0.0f};    //roll的角速度，就是以x为轴转的速度

static PID_Struct pid_yaw = {.kp=0.0f,.ki=0.0f,.kd=0.0f};          //yaw
static PID_Struct pid_yaw_gyro = {.kp=0.0f,.ki=0.0f,.kd=0.0f};

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
        /**
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
        */

        left_top = LIMIT(rd->throttle, 0, 800);     //防止油门拉的太高导致PID失效
        right_top = LIMIT(rd->throttle, 0, 800);
        right_down = LIMIT(rd->throttle, 0, 800);
        left_down = LIMIT(rd->throttle, 0, 800);

        //pitch         //注意这些正负号：pitch角要增大的话是:--++（output为正数），pitch角要减小的话是++--，只有这两种情况！
        left_top -= pid_pitch_gyro.output;
        right_top -= pid_pitch_gyro.output;
        left_down += pid_pitch_gyro.output;
        right_down += pid_pitch_gyro.output;

        //roll          //+-+- or -+-+
        left_top += pid_roll_gyro.output;
        right_top -= pid_roll_gyro.output;
        left_down += pid_roll_gyro.output;
        right_down -= pid_roll_gyro.output;

        //roll          //-++- or +--+
        left_top -= pid_yaw_gyro.output;
        right_top += pid_yaw_gyro.output;
        left_down += pid_yaw_gyro.output;
        right_down -= pid_yaw_gyro.output;

        //
        left_top = LIMIT(left_top, 0, 1000);
        right_top = LIMIT(right_top, 0, 1000);
        right_down = LIMIT(right_down, 0, 1000);
        left_down = LIMIT(left_down, 0, 1000);

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
    if (rd->throttle < 30)
    {
        left_top = 0;
        right_top = 0;
        right_down = 0;
        left_down = 0;
    }

    if (left_top > 100)
    {
        left_top = 100;
    }
    if (right_top > 100)
    {
        right_top = 100;
    }
    if (right_down > 100)
    {
        right_down = 100;
    }
    if (left_down > 100)
    {
        left_down = 100;
    }

    // if (left_top < 20)
    // {
    //     left_top =0;
    // }
    // if (right_top < 20)
    // {
    //     right_top =0;
    // }
    // if (right_down < 20)
    // {
    //     right_down =0;
    // }
    // if (left_down < 20)
    // {
    //     left_down =0;
    // }

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


//pid计算
void App_Flight_PID(Gyroscope_Data* gd, EulerAngle_Struct* ea, Remote_Data *rd, float dt)
{
    //一共pitch roll yaw 三个Cascade PID control system
    //1、pitch
    pid_pitch.desire = (rd->pitch - 500) * 0.02;      //外环目标值 是摇杆数字量，但需限定一下范围 也是转换成角度（因为摇杆的数字量不动时=500、动时几百的波动 太大）
    pid_pitch.measure = ea->pitch;      //外环测量值 是姿态解算的角度
    pid_pitch_gyro.measure = gd->gyro.y * Gyro_G;     //内环测量值 是y轴角速度，需转成度/s
    com_pid_cascade(&pid_pitch, &pid_pitch_gyro, dt);

    //2、roll
    pid_roll.desire = (rd->roll - 500) * 0.02;
    pid_roll.measure = ea->roll;
    pid_roll_gyro.measure = gd->gyro.x * Gyro_G;
    com_pid_cascade(&pid_roll, &pid_roll_gyro, dt);

    //3、yaw
    pid_yaw.desire = (rd->yaw - 500) * 0.02;
    pid_yaw.measure = ea->yaw;
    pid_yaw_gyro.measure = gd->gyro.z * Gyro_G;
    com_pid_cascade(&pid_yaw, &pid_yaw_gyro, dt);
}