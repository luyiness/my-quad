//
// Created by luyi on 2026/5/28.
//

#include "app_communication.h"

uint8_t rx_buffer[TX_PLOAD_WIDTH] = {0};
uint32_t checksum = 0;
const uint8_t TAIL = TX_PLOAD_WIDTH - 4;

Com_Status app_comm_receive_data(Remote_Data *rd)
{
    //接收数据
    uint8_t result = Int_SI24R1_RxPacket(rx_buffer);
    if (result != 0)
    {
        debug_printfln("receive failed");
        return COM_ERROR;
    }

    //头部校验字段
    if (rx_buffer[0] != HEAD_FRAME_0 || rx_buffer[1] != HEAD_FRAME_1 || rx_buffer[2] != HEAD_FRAME_2)
    {
        debug_printfln("Head Verification failed");
        return COM_ERROR;
    }

    //尾部校验字段，后4个Byte
    checksum = 0;
    for (uint8_t i = 0; i < TAIL; i++)
    {
        checksum += rx_buffer[i];
    }
    if (checksum != (rx_buffer[TAIL] | rx_buffer[TAIL+1] << 8
        | rx_buffer[TAIL+2] << 16 | rx_buffer[TAIL+3] << 24))
    {
        debug_printfln("checksum failed");
        return COM_ERROR;
    }

    //解析remote_data数据
    rd->throttle = rx_buffer[3] | rx_buffer[4] << 8;
    rd->yaw = rx_buffer[5] | rx_buffer[6] << 8;
    rd->pitch = rx_buffer[7] | rx_buffer[8] << 8;
    rd->roll = rx_buffer[9] | rx_buffer[10] << 8;
    rd->fix_height = rx_buffer[11];
    rd->shut = rx_buffer[12];

    debug_printfln(": %u,%u,%u,%u", rd->throttle, rd->yaw, rd->pitch, rd->roll);

    return COM_OK;
}

uint32_t at_max_tick = 0;
uint32_t at_min_tick = 0;
//解锁； 就是初始连接时上拉throttle到顶、下拉throttle到底 实现解锁
Com_Status app_comm_unlock(Remote_Data *rd)
{
    switch (thr_status)
    {
    case free:
        if (rd->throttle > 900)
        {
            thr_status = max;
            at_max_tick = xTaskGetTickCount();      //记录进入max状态的时间点
        }
        break;
    case max:       //摇杆保持在max持续1s才能->leave_max，否则->free
        if (rd->throttle < 900)
        {
            if (xTaskGetTickCount() - at_max_tick > 1000)
            {
                thr_status = leave_max;
            }else
            {
                thr_status = free;
            }
        }
        break;
    case leave_max:
        if (rd->throttle < 100)
        {
            thr_status = min;
            at_min_tick = xTaskGetTickCount();
        }
        break;
    case min:       //摇杆保持在min持续1s才能->unlock，否则->free
        if (xTaskGetTickCount() - at_max_tick < 1000)
        {
            if (rd->throttle > 100)
            {
                thr_status = free;
            }
        }else
        {
            thr_status = unlock;
        }
        break;

    case unlock:
        return COM_OK;
    }
    return COM_ERROR;
}

//状态机：切换flight_status（飞行器的状态）
void app_comm_connect_status(Remote_Data *rd)
{
    switch (flight_status)
    {
    case IDLE:
        if (remote_status == CONNECTED)
        {
            Com_Status s = app_comm_unlock(rd);   //解锁
            if (s == COM_OK)
            {
                flight_status = NORMAL;     //IDLE->NORMAL
            }
        }
        break;
    case NORMAL:
        if (remote_status == CONNECTED)
        {
            if (rd -> fix_height == 1)
            {
                flight_status = FIX_HEIGHT;     //NORMAL->FIX_HEIGHT
            }
        } else
        {
            flight_status = FAULT;     //NORMAL->FAULT
        }
        break;
    case FIX_HEIGHT:
        if (remote_status == CONNECTED)
        {
            if (rd -> fix_height == 0)
            {
                flight_status = NORMAL;     //FIX_HEIGHT->NORMAL
            }
        } else
        {
            flight_status = FAULT;     //FIX_HEIGHT->FAULT
        }
        break;
    case FAULT:

        //在motor task中执行故障的逻辑：等待电机缓慢减速至0； 等待通知
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //
        flight_status = IDLE;       //FAULT->IDLE
        thr_status = free;      //油门状态改成空闲
        break;
    }
}