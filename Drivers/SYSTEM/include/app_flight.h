//
// Created by luyi on 2026/5/29.
//

#ifndef QUAD_TEST_APP_FLIGHT_H
#define QUAD_TEST_APP_FLIGHT_H

#include "motor.h"
#include <com_type.h>
#include "freertos_task.h"
#include "task.h"
#include "mpu6050.h"
#include "com_filter.h"
#include "com_imu.h"

void app_flight_set_motors(Remote_Data *rd);

extern TaskHandle_t comm_task_handle;

void app_flight_get_gyroscope(Gyroscope_Data *gd);

void App_Flight_Get_EulerAngle(Gyroscope_Data* gd, EulerAngle_Struct* ea, float dt);

#endif //QUAD_TEST_APP_FLIGHT_H
