//
// Created by luyi on 2026/6/6.
//

#include "vl53l1x.h"

void vl53l1x_init(void)
{
	uint8_t byteData, sensorState=0;
	uint16_t wordData;
	// uint8_t ToFSensor = 1; // 0=Left, 1=Center(default), 2=Right
	uint16_t Distance;
	uint16_t SignalRate;
	uint16_t AmbientRate;
	uint16_t SpadNum;
	uint8_t RangeStatus;
	uint8_t dataReady;

	//重启芯片
	HAL_GPIO_WritePin(VX_XSHUT_GPIO_Port, VX_XSHUT_Pin, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(VX_XSHUT_GPIO_Port, VX_XSHUT_Pin, GPIO_PIN_SET);
	HAL_Delay(2);

	/* Those basic I2C read functions can be used to check your own I2C functions */
	VL53L1_RdByte(DEV_ADDR, 0x010F, &byteData);
	debug_printf("VL53L1X Model_ID: %X\n", byteData);
	VL53L1_RdByte(DEV_ADDR, 0x0110, &byteData);
	debug_printf("VL53L1X Module_Type: %X\n", byteData);
	VL53L1_RdWord(DEV_ADDR, 0x010F, &wordData);
	debug_printf("VL53L1X: %X\n", wordData);
	while(sensorState == 0){
		VL53L1X_BootState(DEV_ADDR, &sensorState);
		HAL_Delay(2);
	}
	debug_printf("Chip booted\n");

	/* This function must to be called to initialize the sensor with the default setting  */
	VL53L1X_SensorInit(DEV_ADDR);
	/* Optional functions to be used to change the main ranging parameters according the application requirements to get the best ranging performances */

	//设置测距参数
	VL53L1X_SetDistanceMode(DEV_ADDR, 2); /* 1=short, 2=long */
	VL53L1X_SetTimingBudgetInMs(DEV_ADDR, 100); /* in ms possible values [20, 50, 100, 200, 500] */
	VL53L1X_SetInterMeasurementInMs(DEV_ADDR, 100); /* in ms, IM must be > = TB */
	//  VL53L1X_SetOffset(DEV_ADDR,20); /* offset compensation in mm */
	//  VL53L1X_SetROI(DEV_ADDR, 16, 16); /* minimum ROI 4,4 */
	//	VL53L1X_CalibrateOffset(DEV_ADDR, 140, &offset); /* may take few second to perform the offset cal*/
	//	VL53L1X_CalibrateXtalk(DEV_ADDR, 1000, &xtalk); /* may take few second to perform the xtalk cal */
	debug_printf("VL53L1X Ultra Lite Driver Example running ...\n");

	//使能测距
	VL53L1X_StartRanging(DEV_ADDR);   /* This function has to be called to enable the ranging */

	// while(1){ /* read and display data */
	//   while (dataReady == 0){
	// 	  VL53L1X_CheckForDataReady(DEV_ADDR, &dataReady);
	// 	  HAL_Delay(2);
	// }
	// dataReady = 0;
	// VL53L1X_GetRangeStatus(DEV_ADDR, &RangeStatus);
	// VL53L1X_GetDistance(DEV_ADDR, &Distance);
	// VL53L1X_GetSignalRate(DEV_ADDR, &SignalRate);
	// VL53L1X_GetAmbientRate(DEV_ADDR, &AmbientRate);
	// VL53L1X_GetSpadNb(DEV_ADDR, &SpadNum);
	// VL53L1X_ClearInterrupt(DEV_ADDR); /* clear interrupt has to be called to enable next interrupt*/
	// debug_printf("%u, %u, %u, %u, %u\n", RangeStatus, Distance, SignalRate, AmbientRate,SpadNum);
}

void vl53l1x_getDistance(uint16_t* distance)
{
	VL53L1X_GetDistance(DEV_ADDR, distance);	//distance，16位，单位mm
	// debug_printf("distance: %X\n", *distance);
}
