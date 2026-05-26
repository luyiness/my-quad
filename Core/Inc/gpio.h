/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
#define LED0_GPIO_PORT                  GPIOB
#define LED0_GPIO_PIN                   GPIO_PIN_5
#define LED0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)             /* PB口时钟使能 */

#define LED1_GPIO_PORT                  GPIOE
#define LED1_GPIO_PIN                   GPIO_PIN_5
#define LED1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

//PB2： 5305T定时唤醒
#define POWER_KEY_Pin   GPIO_PIN_2
#define POWER_KEY_GPIO_Port   GPIOB

//灯控系统
#define L1_Pin   GPIO_PIN_12
#define L1_GPIO_Port   GPIOA

#define L2_Pin   GPIO_PIN_11
#define L2_GPIO_Port   GPIOA

#define L3_Pin   GPIO_PIN_15
#define L3_GPIO_Port   GPIOB

#define L4_Pin   GPIO_PIN_14
#define L4_GPIO_Port   GPIOB

#define SI24R1_CE_Pin   GPIO_PIN_8        //对应SI24R1的CE引脚
#define SI24R1_CE_GPIO_Port   GPIOA

#define SI24R1_NSS_Pin   GPIO_PIN_4
#define SI24R1_NSS_GPIO_Port   GPIOA

/******************************************************************************************/
/* LED端口定义 */
#define LED0(x)   do{ x ? \
HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)      /* LED0翻转 */

#define LED1(x)   do{ x ? \
HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)      /* LED1翻转 */

/* LED取反定义 */
#define LED0_TOGGLE()   do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)        /* 翻转LED0 */
#define LED1_TOGGLE()   do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)        /* 翻转LED1 */

//spi通讯时，用于拉低/拉高片选
#define SI24R1_CS_L (HAL_GPIO_WritePin(SI24R1_NSS_GPIO_Port, SI24R1_NSS_Pin, GPIO_PIN_RESET))
#define SI24R1_CS_H (HAL_GPIO_WritePin(SI24R1_NSS_GPIO_Port, SI24R1_NSS_Pin, GPIO_PIN_SET))
// SI24R1使能/失能
#define SI24R1_CE_L (HAL_GPIO_WritePin(SI24R1_CE_GPIO_Port, SI24R1_CE_Pin, GPIO_PIN_RESET))
#define SI24R1_CE_H (HAL_GPIO_WritePin(SI24R1_CE_GPIO_Port, SI24R1_CE_Pin, GPIO_PIN_SET))

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

