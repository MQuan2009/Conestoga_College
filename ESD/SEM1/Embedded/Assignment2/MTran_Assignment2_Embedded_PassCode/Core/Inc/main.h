/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define RED_Pin GPIO_PIN_4
#define RED_GPIO_Port GPIOA
#define COMMON_Pin GPIO_PIN_5
#define COMMON_GPIO_Port GPIOA
#define GREEN_Pin GPIO_PIN_6
#define GREEN_GPIO_Port GPIOA
#define BLUE_Pin GPIO_PIN_7
#define BLUE_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define NUM_PASSCODE   10
#define NUM_ATTEMPT     3
#define USERCODE_MATCH  1
// User Name
#define USER1  'A'
#define USER2  'B'
#define USER3  'C'
#define USER4  'D'
#define USER5  'E'
#define USER6  'F'
#define USER7  'G'
#define USER8  'H'
#define USER9  'J'
#define USER10 'K'

// Pass Code for 10 users
#define PASSCODE1  1000
#define PASSCODE2  2000
#define PASSCODE3  3000
#define PASSCODE4  4000
#define PASSCODE5  5000
#define PASSCODE6  6000
#define PASSCODE7  7000
#define PASSCODE8  8000
#define PASSCODE9  9000
#define PASSCODE10 9999

// Unique pass Code for 10 users
#define UNI_CODE1  1000
#define UNI_CODE2  2000
#define UNI_CODE3  3000
#define UNI_CODE4  4000
#define UNI_CODE5  5000
#define UNI_CODE6  6000
#define UNI_CODE7  7000
#define UNI_CODE8  8000
#define UNI_CODE9  9000
#define UNI_CODE10 9999

// LED colour
#define RED   1
#define GREEN 2
#define WHITE 3

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
