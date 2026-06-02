//
// Created by luyi on 2026/6/2.
//

#ifndef QUAD_TEST_COM_IMU_H
#define QUAD_TEST_COM_IMU_H

#include "com_type.h"
#include "math.h"

/* 表示四元数的结构体 */
typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
} Quaternion_Struct;

extern float RtA;
extern float Gyro_G;
extern float Gyro_Gr;

void Com_IMU_GetEulerAngle(Gyroscope_Data *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float dt);
float Com_IMU_GetNormAccZ(void);


#endif //QUAD_TEST_COM_IMU_H
