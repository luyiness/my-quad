//
// Created by luyi on 2026/5/27.
//

#ifndef REMOTE_TEST_COM_TYPE_H
#define REMOTE_TEST_COM_TYPE_H
#include <stdint.h>

//灯语：
//遥控状态：已连接上遥控，未连接上遥控
typedef enum
{
    CONNECTED,
    DISCONNECTED,
}remote_status_enum;

extern remote_status_enum remote_status;

//飞行状态：4个
typedef enum
{
    IDLE,
    NORMAL,
    FIX_HEIGHT,
    FAULT,
}flight_status_enum;

extern flight_status_enum flight_status;

//遥控数据：yaw,pitch,roll等
typedef struct
{
    int16_t throttle;      //参数值有正负; throttle 油门
    int16_t yaw;
    int16_t pitch;
    int16_t roll;
    uint8_t fix_height; //定高
    uint8_t shut;       //关机
}Remote_Data;

typedef enum
{
    COM_OK,
    COM_ERROR,
}Com_Status;


typedef enum
{
    FREE,       //大写是因为有冲突：Ambiguous symbol 'free'
    max,
    leave_max,
    min,
    unlock,
}throttle_status_enum;

extern throttle_status_enum thr_status;

//MPU6050
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}Gyro_Data;     //角速度angular velocity

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}Accel_Data;    //加速度acceleration

typedef struct
{
    Gyro_Data gyro;
    Accel_Data accel;
}Gyroscope_Data;

typedef struct
{
    float yaw;
    float pitch;
    float roll;
}EulerAngle_Struct;

#endif //REMOTE_TEST_COM_TYPE_H
