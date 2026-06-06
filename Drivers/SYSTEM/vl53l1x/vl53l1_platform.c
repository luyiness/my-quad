
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	//dev就是I2C的设备地址：0x52, 从参数index（内存地址）获取数据、一次写入8位，写入pdata
	HAL_I2C_Mem_Write(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, 1000);
	return 0; // to be implemented
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	HAL_I2C_Mem_Read(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, pdata, count, 1000);
	return 0; // to be implemented
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	//参数size=1、一次写一个字节
	HAL_I2C_Mem_Write(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
	return 0; // to be implemented
}

static uint8_t I2CBuffer[4] = {0};
//写一个16位数据
int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	I2CBuffer[0] = data >> 8;		//高位在前，big-endian
	I2CBuffer[1] = data & 0xFF;
	HAL_I2C_Mem_Write(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, I2CBuffer, 2, 1000);	//高字节先发，低字节后发

	//老师的写法
	// data = __REV16(data);
	// HAL_I2C_Mem_Write(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)&data, 2, 1000);
	return 0; // to be implemented
}

//写一个32位数据
int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	I2CBuffer[0] = (data >> 24) & 0xFF;
	I2CBuffer[1] = (data >> 16) & 0xFF;
	I2CBuffer[2] = (data >> 8) & 0xFF;
	I2CBuffer[3] = data & 0xFF;
	HAL_I2C_Mem_Write(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, I2CBuffer, 4, 1000);	//高字节先发，低字节后发

	return 0; // to be implemented
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	HAL_I2C_Mem_Read(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, data, 1, 1000);
	return 0; // to be implemented
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	// //chatgpt的写法
	// uint8_t buffer[2] = {0};
	// HAL_I2C_Mem_Read(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, buffer, 2, 1000);
	// *data = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];

	//老师的写法
	HAL_I2C_Mem_Read(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, 2, 1000);
	*data = __REV(*data);
	return 0; // to be implemented
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	HAL_I2C_Mem_Read(&hi2c2, dev, index, I2C_MEMADD_SIZE_16BIT, (uint8_t *)data, 4, 1000);
	*data = __REV(*data);
	return 0; // to be implemented
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	HAL_Delay(wait_ms);
	return 0; // to be implemented
}
