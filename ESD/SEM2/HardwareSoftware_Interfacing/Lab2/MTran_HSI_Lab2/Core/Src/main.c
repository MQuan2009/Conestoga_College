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
* PROJECT : GPIO - Lab2
* PROGRAMMER : Minh Quan Tran
* FIRST VERSION : 2024 - 09 - 19
* DESCRIPTION : This file is the main code file to read/write GPIO
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	NONE,
	LEFT,
	RIGHT,
	TOGGLE,
} pushButtonState_t;

typedef enum
{
	NOTHING   = 0,
	LEFT_PIN  = GPIO_PIN_0,
	RIGHT_PIN = GPIO_PIN_1,
	TOGGLE_PIN  = GPIO_PIN_2,
} pushButtonPin_t;

typedef enum
{
	BTN,
	PROMPT,
} ledControlState_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_TIMEOUT 3000 //UART Time out after 3s
#define UART_TIMEOUT_ERROR 0xFF
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile pushButtonPin_t btnPressed = NOTHING;
volatile pushButtonState_t btnState = NONE;
volatile ledControlState_t controlState = BTN;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* FUNCTION : setLed
*
* DESCRIPTION : This function is used to set the 3 LED
*
* PARAMETERS :
*
* bool led1Status: status of LED1
* bool led2Status: status of LED2
* bool led3Status: status of LED3
*
*
*
* RETURNS :
*
*/
void setLed(bool led1Status,bool led2Status, bool led3Status)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,led1Status);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,led2Status);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,led3Status);

}

/* FUNCTION : shiftLeftLed
*
* DESCRIPTION : This function is used to shift LED to the LEFT using the setLed function
*
* PARAMETERS :
*
*
* RETURNS :
*
*/
void shiftLeftLed(void)
{
	setLed(0,0,1);
	HAL_Delay(100);
	setLed(0,1,0);
	HAL_Delay(100);
	setLed(1,0,0);
	HAL_Delay(100);
	setLed(0,0,0);
	HAL_Delay(100);
}

/* FUNCTION : shiftRightLed
*
* DESCRIPTION : This function is used to shift LED to the RIGHT using the setLed function
*
* PARAMETERS :
*
*
* RETURNS :
*
*/
void shiftRightLed(void)
{
	setLed(1,0,0);
	HAL_Delay(100);
	setLed(0,1,0);
	HAL_Delay(100);
	setLed(0,0,1);
	HAL_Delay(100);
	setLed(0,0,0);
	HAL_Delay(100);
}

/* FUNCTION : getCharFromUart2
*
* DESCRIPTION : This function is used to get char from UART
*
* PARAMETERS :
*
*
* RETURNS : uint8_t: return either the character or TIMEOUT code
*
*/
uint8_t getCharFromUart2(void)
{
    uint8_t rcvChar;
    HAL_StatusTypeDef status;

    // Try to receive a single character with a timeout
    status = HAL_UART_Receive(&huart2, &rcvChar, 1, UART_TIMEOUT);

    if (status == HAL_OK)
    {
        // Successfully received a character
        return rcvChar;
    }
    else
    {
        // Timeout occurred
        return UART_TIMEOUT_ERROR;
    }
}

/* FUNCTION : HAL_GPIO_EXTI_Callback
*
* DESCRIPTION : This function is used to call back whenever there an external interrupt
* happen. It used to start the timer to count for 20ms
*
* PARAMETERS :
*
* uint16_t GPIO_Pin: PIN number
*
* RETURNS : VOID
*
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	btnPressed = GPIO_Pin;

	// Start a timer to handle debounce
    HAL_TIM_Base_Start_IT(&htim2);
}
/* FUNCTION : HAL_TIM_PeriodElapsedCallback
*
* DESCRIPTION : This function is used to check the button state after
* 20ms to see if the button remained at a previous state (debounce)
*
* PARAMETERS :
*
* TIM_HandleTypeDef *htim: timer
*
* RETURNS : VOID
*
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        // Stop the timer
        HAL_TIM_Base_Stop_IT(&htim2);

        // Check the button state after 20ms
        if ((btnPressed == LEFT_PIN) && (HAL_GPIO_ReadPin(GPIOC,LEFT_PIN) == GPIO_PIN_RESET))
        {
        	btnState = LEFT;
        }
        else if ((btnPressed == RIGHT_PIN) && (HAL_GPIO_ReadPin(GPIOC,RIGHT_PIN) == GPIO_PIN_RESET))
        {
        	btnState = RIGHT;
        }
        else if ((btnPressed == TOGGLE_PIN) && (HAL_GPIO_ReadPin(GPIOC,TOGGLE_PIN) == GPIO_PIN_RESET))
        {
            btnState = TOGGLE;
        }
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
	char rcvChar = ' ';
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(controlState)
	  {
	  	  case BTN:
	  		  switch(btnState)
	  		  {
	  			  case LEFT:
	  				  shiftLeftLed();
	  				  break;
	  			  case RIGHT:
	  				  shiftRightLed();
	  				  break;
	  			  case TOGGLE:
	  				  controlState = PROMPT;
	  				  btnState = NONE;
	  				  break;
	  			  default:
	  				  break;

	  		  }
	  		  break;
	  	  case PROMPT:

	  		  // Toggle a button again to enter how the LED lit
	  		  if (btnState == TOGGLE)
	  		  {
				  printf("Insert L/R to control LED\r\n");
				  switch(getCharFromUart2())
				  {
					  case 'L':
						  rcvChar = 'L';
						  printf("Shift Left\r\n");
						  break;
					  case 'R':
						  rcvChar = 'R';
						  printf("Shift Right\r\n");
						  break;
					  case UART_TIMEOUT_ERROR:
						  printf("Prompt Timeout\r\n");
						  printf("Switching to BTN control state\r\n");
						  controlState = BTN;
						  rcvChar = ' ';
					  default:
						  break;

				  }
				  btnState = NONE;
	  		  }

	  		  switch(rcvChar)
	  		  {
			  	  case 'L':
			  		  shiftLeftLed();
			  		  break;
			  	  case 'R':
			  		  shiftRightLed();
			  		  break;
			  	  default:
			  		  break;
	  		  }

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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

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
