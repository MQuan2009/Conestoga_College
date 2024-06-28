/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/*
* FILE : main.c
* PROJECT : Pass Code - Assignment #2
* PROGRAMMER : Minh Quan Tran
* FIRST VERSION : 2024-06-27
* DESCRIPTION : This file is the main code file to operate a Pass Code openning door
*/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t passCode[10] = {PASSCODE1, PASSCODE2, PASSCODE3, PASSCODE4, PASSCODE5,
						 PASSCODE6, PASSCODE7, PASSCODE8, PASSCODE9, PASSCODE10};
uint16_t uniquePassCode[10] = {UNI_CODE1, UNI_CODE2, UNI_CODE3, UNI_CODE4, UNI_CODE5,
						 	   UNI_CODE6, UNI_CODE7, UNI_CODE8, UNI_CODE9, UNI_CODE10};
char passCodeOwner[10] = {USER1, USER2, USER3, USER4, USER5,
						  USER6, USER7, USER8, USER9, USER10};
bool speakerOff = true;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void changeSpeakerFrequency(TIM_HandleTypeDef *htim, uint32_t newFrequency);
void turnOffSpeaker(void);
void playCorrectSound(void);
void playWrongSound(void);
bool checkPassCode(uint16_t usrPassCode);
uint8_t checkOwnerCode(uint16_t usrPassCode);
void setLed(int ledColour);
void resetPassCode(char usrReset);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* FUNCTION : changeSpeakerFrequency
*
* DESCRIPTION : This function is used to change the timer frequency in order for the speaker
* to have different sound.
* PARAMETERS :
* TIM_HandleTypeDef *htim: Timer
* uint32_t newFrequency:  New desire frequency
*
* RETURNS : void
*/
void changeSpeakerFrequency(TIM_HandleTypeDef *htim, uint32_t newFrequency)
{
	// Calculate new prescaler
	uint32_t timer_clock = 4000000; // ABP Timer Clock is 4MHz
	uint32_t period = 39; // Configured period value
	uint32_t prescaler = (timer_clock / (period + 1)) / newFrequency - 1;

	htim->Instance->ARR = period;
	htim->Instance->PSC = prescaler;

	// Update the timer registers
	__HAL_TIM_SET_COUNTER(htim, 0);
	__HAL_TIM_SET_AUTORELOAD(htim, period);
	__HAL_TIM_SET_PRESCALER(htim, prescaler);

	// Restart the timer PWM generation
	if(speakerOff)
	{
		HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
		speakerOff = false;
	}
}

/* FUNCTION : turnOffSpeaker
*
* DESCRIPTION : This function is used to turn off the speaker
* PARAMETERS : void
*
* RETURNS : void
*/
void turnOffSpeaker(void)
{
	speakerOff = true;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

/* FUNCTION : playCorrectSound
*
* DESCRIPTION : This function used the changeSpeakerFrequency and turnOffSpeaker function to
*  create a tune for the speaker.
*
* PARAMETERS : void
*
* RETURNS : void
*/
void playCorrectSound(void)
{
	changeSpeakerFrequency(&htim1, 200);
	HAL_Delay(200);
	turnOffSpeaker();
	changeSpeakerFrequency(&htim1, 500);
	HAL_Delay(200);
	turnOffSpeaker();
	changeSpeakerFrequency(&htim1, 800);
	HAL_Delay(200);

	// Turn off sound
	turnOffSpeaker();
}

/* FUNCTION : playWrongSound
*
* DESCRIPTION : This function used the changeSpeakerFrequency and turnOffSpeaker function to
*  create a tune for the speaker.
*
* PARAMETERS : void
*
* RETURNS : void
*/
void playWrongSound(void)
{
	changeSpeakerFrequency(&htim1, 800);
	HAL_Delay(200);
	turnOffSpeaker();
	changeSpeakerFrequency(&htim1, 500);
	HAL_Delay(200);
	turnOffSpeaker();
	changeSpeakerFrequency(&htim1, 200);
	HAL_Delay(200);

	// Turn off sound
	turnOffSpeaker();
}

/* FUNCTION : checkPassCode
*
* DESCRIPTION : This function compare with user pass code and the real passcode.
*
* PARAMETERS :
* uint16_t usrPassCode: user input pass code
*
* RETURNS : bool
*/
bool checkPassCode(uint16_t usrPassCode)
{
	for (int i = 0; i < NUM_PASSCODE; i++)
	{
		if (usrPassCode == passCode[i] || usrPassCode == uniquePassCode[i])
		{
			return true;
		}
	}
	return false;
}

/* FUNCTION : checkOwnerCode
*
* DESCRIPTION : This function compare with user pass code and the real passcode and
* find out who is the owner of that passcode.
*
* PARAMETERS :
* uint16_t usrPassCode: user input pass code
*
* RETURNS : uint8_t
*/
uint8_t checkOwnerCode(uint16_t usrPassCode)
{
	for (int i = 0; i < NUM_PASSCODE; i++)
	{
		if (usrPassCode == passCode[i] || usrPassCode == uniquePassCode[i])
		{
			return i;
		}
	}
	return 10;
}

/* FUNCTION : setLed
*
* DESCRIPTION : This function change the RGB Led colour
*
* PARAMETERS :
* int ledColour: Led colour
*
* RETURNS : void
*/
void setLed(int ledColour)
{
	switch(ledColour)
	{
	case RED:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		break;
	case WHITE:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		break;
	}

}

/* FUNCTION : resetPassCode
*
* DESCRIPTION : This function used to reset the user pass code from the user UNIQUE
* PASSCODE
*
* PARAMETERS :
* char usrReset: User choice to reset or not
*
* RETURNS : void
*/
void resetPassCode(char usrReset)
{
	int usrUniqueCode, usrNewCode, ownerCodeName = 0;
	int numberAttemptLeft = 0;

	if (usrReset == 'y')
	{
		for (int i = NUM_ATTEMPT; i > 0; i--)
			{
				printf("Please type in your UNIQUE passcode: \r\n");
				scanf("%d", &usrUniqueCode);
				if (checkPassCode(usrUniqueCode) == USERCODE_MATCH)
				{
					printf("Access Granted\r\n");
					setLed(GREEN);
					playCorrectSound();
					numberAttemptLeft = 3;
					break;
				}
				else
				{
					printf("Access Denied\r\n");
					printf("You have %d attempt left \r\n", i - 1);
					setLed(RED);
					playWrongSound();
					numberAttemptLeft = i - 1;
					continue;
				}
			}

		// Zero attempt left for resetting pass code
		if (numberAttemptLeft == 0)
		{
			printf("Please contact the owner for further assist\r\n");
			printf("~~~~~~~~~~~~~~~\r\n");
			return;
		}

		// Resetting user passcode
		else
		{
			ownerCodeName = checkOwnerCode(usrUniqueCode); //Get Owner Name
			printf("Welcome Mrs %c\r\n", passCodeOwner[ownerCodeName]);
			printf("Please type in your new code\r\n");
			scanf("%d",&usrNewCode);
			passCode[ownerCodeName] = usrNewCode;
			printf("Your new passcode have been saved, please log in again\r\n");
			printf("~~~~~~~~~~~~~~~\r\n");
			return;
		}
	}
	else
	{
		printf("~~~~~~~~~~~~~~~\r\n");
		return;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	uint16_t userCode = 0;
	uint8_t numberAttemptLeft = 0;
	char usrReset;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  turnOffSpeaker();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Give user 3 attempts to type in their passcode
	  for (int i = NUM_ATTEMPT; i > 0; i--)
		{
			  printf("Please type in your passcode: \r\n");
			  scanf("%hu", &userCode);

			  // When user pass code match with the system
			  if (checkPassCode(userCode) == USERCODE_MATCH)
			  {
				  printf("Access Granted\r\n");
				  setLed(GREEN);
				  playCorrectSound();
				  numberAttemptLeft = 3;
				  break;
			  }

			  // When user pass code doesn't match with the system
			  else
			  {
				  printf("Access Denied\r\n");
				  printf("You have %d attempt left \r\n", i - 1);
				  setLed(RED);
				  playWrongSound();
				  numberAttemptLeft = i - 1;
				  continue;
			  }
		}

		// Reset Pass Code
		if (numberAttemptLeft == 0)
		{
			do
			{
				printf("Do you want to reset your passcode? (Type y or n): \r\n");
				scanf("%c",&usrReset);
				printf("%c\r\n",usrReset);
				if (usrReset != 'y' && usrReset != 'n')
				{
					printf("Please type again\r\n");
				}
			}
			while(usrReset != 'y' && usrReset != 'n');

			//Resetting user code
			resetPassCode(usrReset);
		}
		else
		{
			printf("~~~~~~~~~~~~~~~\r\n");
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 9;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 39;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 19;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED_Pin|COMMON_Pin|GREEN_Pin|BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED_Pin COMMON_Pin GREEN_Pin BLUE_Pin */
  GPIO_InitStruct.Pin = RED_Pin|COMMON_Pin|GREEN_Pin|BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
