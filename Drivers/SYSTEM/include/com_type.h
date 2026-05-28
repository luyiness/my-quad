//
// Created by luyi on 2026/5/27.
//

#ifndef REMOTE_TEST_COM_TYPE_H
#define REMOTE_TEST_COM_TYPE_H
#include <stdint.h>

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

#endif //REMOTE_TEST_COM_TYPE_H
