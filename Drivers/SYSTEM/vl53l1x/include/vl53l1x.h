//
// Created by luyi on 2026/6/6.
//

#ifndef QUAD_TEST_VL53L1X_H
#define QUAD_TEST_VL53L1X_H

#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"
#include "gpio.h"
#include "Com_debug.h"

void vl53l1x_init(void);

void vl53l1x_getDistance(uint16_t* distance);

#endif //QUAD_TEST_VL53L1X_H
