//
// Created by luyi on 2026/5/29.
//

#ifndef QUAD_TEST_APP_FLIGHT_H
#define QUAD_TEST_APP_FLIGHT_H

#include "motor.h"
#include <com_type.h>
#include "freertos_task.h"
#include "task.h"

void app_flight_set_motors(Remote_Data *rd);

extern TaskHandle_t comm_task_handle;

#endif //QUAD_TEST_APP_FLIGHT_H
