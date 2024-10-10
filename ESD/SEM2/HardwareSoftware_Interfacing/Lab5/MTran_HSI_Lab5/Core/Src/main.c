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
* PROJECT : Stepper Motor - Lab5
* PROGRAMMER : Minh Quan Tran
* FIRST VERSION : 2024 - 10 - 10
* DESCRIPTION : This file is the main code file to use timer to control the stepper motor
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	IN_PROCESS,
	DONE
} motorStatus_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar (void)

#define RAMP_STEP 450

#define MAX_DELAY 5000
#define MIN_DELAY 50

#define USE_INTERRUPT
#define ADVANCE

// Define the min macro
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
motorStatus_t runFlag = IN_PROCESS;
int32_t numberOfSteps = 1600;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM11_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* FUNCTION : microDelay
*
* DESCRIPTION : This function is create to delay in micro second by counting the
* 				number of ticks has pass using the CNT register
*
* PARAMETERS : uint32_t usDelay  - duration of delay
* RETURNS 	 : NONE
*
*/
void microDelay(uint32_t usDelay)
{
    // Get the current timer counter value
    uint32_t startTime = __HAL_TIM_GET_COUNTER(&htim11);
    uint32_t ticks = usDelay - 1; // 1 ticks = 1 us

    // Poll the CNT register until the specified number of ticks has passed
    while ((__HAL_TIM_GET_COUNTER(&htim11) - startTime) < ticks)
    {
        // Wait until the timer reaches the required ticks
    }
}

/* FUNCTION : stepInit
*
* DESCRIPTION : This function is to initalize the stepper motor
*
* PARAMETERS : NONE
* RETURNS 	 : NONE
*
*/
void stepInit(void)
{
	HAL_GPIO_WritePin(GPIOC,Reset_Pin,GPIO_PIN_SET); // High - Normal operation, Low - Reset
	HAL_GPIO_WritePin(GPIOC,PS_Pin,GPIO_PIN_SET);    // High - Operation mode, Low - Standby mode
	HAL_GPIO_WritePin(GPIOA,VRef_Pin,GPIO_PIN_SET);  // High - 100% current, Low - 0 current
	printf("Inital Motor \r\n");
}

/* FUNCTION : stepEnable
*
* DESCRIPTION : This function is to enable and disable the stepper motor
*
* PARAMETERS : bool enable: flag to enable or disable motor
* RETURNS 	 : NONE
*
*/
void stepEnable(bool enable)
{
	if(!enable)
	{
		HAL_GPIO_WritePin(GPIOC,OE_Pin,GPIO_PIN_SET);
		printf("Disable Motor\r\n");
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,OE_Pin,GPIO_PIN_RESET);
		printf("Enable Motor\r\n");
	}
	return;
}

/* FUNCTION : stepSteps
*
* DESCRIPTION : This function is to control the number of step and how long a step
* 				will run in the stepper motor
*
* PARAMETERS :
* int32_t numberOfSteps - number of step motor will run
* uint32_t usDelay      - how much delay (in microsecond) for 1 step
*
* RETURNS 	 : NONE
*
*/
void stepSteps(int32_t numberOfSteps, uint32_t usDelay)
{
#ifdef USE_INTERRUPT
	runFlag = IN_PROCESS;

	htim11.Init.Period = usDelay / 2 - 1;
	if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_TIM_Base_Start_IT(&htim11);

	//Wait until finish step
	while(runFlag == IN_PROCESS);

#else // Basic (No interrupt needed)

	// Tell which direction the motor is running
	if (numberOfSteps > 0)
	{
		HAL_GPIO_WritePin(GPIOC,FR_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,FR_Pin,GPIO_PIN_RESET);
	}

	printf("Stepping Motor\r\n");
	for (int i = 0; i < abs(numberOfSteps); i++)
	{
		HAL_GPIO_WritePin(GPIOB,Step_Pin,GPIO_PIN_SET);
		microDelay(usDelay / 2);
		HAL_GPIO_WritePin(GPIOB,Step_Pin,GPIO_PIN_RESET);
		microDelay(usDelay / 2);
	}
	printf("Finish Stepping\r\n");
#endif


}

#ifdef ADVANCE
/* FUNCTION : runTrapezoidalMotion
*
* DESCRIPTION : This function is to run the motor using the trapezoidal motion where it accelerate
* 				and decelerate for the number of ramp step
*
* PARAMETERS :
* int32_t numberOfSteps - number of step motor will run
* uint32_t usDelay      - how much delay (in microsecond) for 1 step
*
* RETURNS 	 : NONE
*
*/
void runTrapezoidalMotion(int32_t numberOfSteps, uint32_t usDelay)
{
	// Tell which direction the motor is running
	if (numberOfSteps > 0)
	{
		HAL_GPIO_WritePin(GPIOC,FR_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,FR_Pin,GPIO_PIN_RESET);
	}

	numberOfSteps = abs(numberOfSteps);

    // Ensure number of rampSteps is not greater than half of the total number of steps
    int32_t rampSteps = MIN(RAMP_STEP, numberOfSteps / 2);

    // Middle steps at max speed
    int32_t middleSteps = numberOfSteps - 2 * rampSteps;

    printf("Stepping Motor\r\n");
    // Acceleration Phase
    for (int32_t i = 0; i < rampSteps; i++)
    {
        // Linearly interpolate delay between max speed and min speed
        uint32_t currentDelay = MAX_DELAY - (MAX_DELAY - usDelay) * i / rampSteps;
        stepSteps(1, currentDelay);
    }

    // Constant Speed Phase
    for (int32_t i = 0; i < middleSteps; i++)
    {
        stepSteps(1, usDelay);  //Intended speed
    }

    // Deceleration Phase
    for (int32_t i = 0; i < rampSteps; i++)
    {
        // Linearly interpolate delay between minUsDelay and maxUsDelay
        uint32_t currentDelay = usDelay + (MAX_DELAY - usDelay) * i / rampSteps;
        stepSteps(1, currentDelay);
    }

    printf("Finish Stepping\r\n");
}
#endif

/* FUNCTION : HAL_TIM_PeriodElapsedCallback
*
* DESCRIPTION : This function is used to toggle LED whenever the timer overflow
*
* PARAMETERS : NONE
* RETURNS 	 : NONE
*
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM11)
    {
        HAL_GPIO_TogglePin(GPIOB, Step_Pin);
        static uint32_t stepCount = 0;
        stepCount++;
#ifdef ADVANCE
        if (stepCount > 2)
        {
        	HAL_TIM_Base_Stop_IT(&htim11);
        	stepCount = 0;
        	runFlag = DONE;
        }
#else
        if (stepCount > abs(numberOfSteps) * 2)
        {
        	HAL_TIM_Base_Stop_IT(&htim11);
        	stepCount = 0;
        	runFlag = DONE;
        }
#endif
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
	bool enableFlag = 0;
	uint16_t delay = 500;

	int doPrompt = 1;
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
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  stepInit();

#ifndef USE_INTERRUPT
  HAL_TIM_Base_Start(&htim11);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(doPrompt != 0)
	  {
		  doPrompt = 0;
		  printf(PROMPT);
	  }


	  switch(GetCharFromUART2())
	  {
	  case '0':
		  enableFlag = !enableFlag;
		  stepEnable(enableFlag);
		  break;
	  case '1':
		  numberOfSteps += 400; // Rotate an addition 90o
		  printf("Set steps = %ld, Set delay = %u \r\n",numberOfSteps, delay );
		  break;
	  case '2':
		  numberOfSteps -= 400; // Reduce 90 degree
		  printf("Set steps = %ld, Set delay = %u \r\n",numberOfSteps, delay );
		  break;
	  case '3':
		  if (delay < MAX_DELAY)
		  {
			  delay += 50;
			  printf("Set steps = %ld, Set delay = %u \r\n",numberOfSteps, delay );
		  }
		  else
		  {
			  printf("Delay can't be higher than %duS\r\n", MAX_DELAY);
		  }
		  break;
	  case '4':
		  if (delay > MIN_DELAY)
		  {
			  delay -= 50;
			  printf("Set steps = %ld, Set delay = %u \r\n",numberOfSteps, delay );
		  }
		  else
		  {
			  printf("Delay can't be lower than %duS\r\n", MIN_DELAY);
		  }
		  break;
	  case '5':
#ifdef ADVANCE
		  runTrapezoidalMotion(numberOfSteps,delay);
#else
		  stepSteps(numberOfSteps,delay);
		  printf("RUNNING\r\n");
#endif
		  break;
	  case TIMEOUT_ERROR:
		  printf("Prompt timeout\r\n");
		  break;
	  default:
		  break;
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 99;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 499;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Reset_Pin|OE_Pin|FR_Pin|PS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, VRef_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Step_GPIO_Port, Step_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Reset_Pin OE_Pin FR_Pin PS_Pin */
  GPIO_InitStruct.Pin = Reset_Pin|OE_Pin|FR_Pin|PS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : VRef_Pin LD2_Pin */
  GPIO_InitStruct.Pin = VRef_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Step_Pin */
  GPIO_InitStruct.Pin = Step_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Step_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{/* Place your implementation of fputc here */
/* e.g. write a character to the USART1 and Loop until the end of transmission */
HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
return ch;
}
GETCHAR_PROTOTYPE
{uint8_t ch = 0;
// Clear the Overrun flag just before receiving the first character
__HAL_UART_CLEAR_OREFLAG(&huart2);
HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, 5);
return ch;
}
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
