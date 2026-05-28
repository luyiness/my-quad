//
// Created by luyi on 2026/5/28.
//

#ifndef QUAD_TEST_APP_COMMUNICATION_H
#define QUAD_TEST_APP_COMMUNICATION_H

#include "SI24R1.h"
#include "com_type.h"
#include "led.h"

#define HEAD_FRAME_0 'A'
#define HEAD_FRAME_1 'B'
#define HEAD_FRAME_2 'C'

Com_Status app_comm_receive_data(Remote_Data *rd);

void app_comm_connect_status(Remote_Data *rd);

#endif //QUAD_TEST_APP_COMMUNICATION_H
