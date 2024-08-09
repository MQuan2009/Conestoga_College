/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
* PROJECT : Temperature Sensor - Assignment #3
* PROGRAMMER : Minh Quan Tran
* FIRST VERSION : 2024-07-20
* DESCRIPTION : This file is the main code file to read an MCP9700A temperature sensor
* and display it to the OLed and RGB Led
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1331.h"
#include "logo.h"
#include "stm32l4xx_hal_tim.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGBColor;

typedef enum
{
	NONE,
	OK,
	CANCLE,
	CHEQUE,
	SAVING,
} pushButtonStateTypeDef;

typedef enum
{
	NOTHING    = 0,
	OK_PIN     = GPIO_PIN_1,
	CANCLE_PIN = GPIO_PIN_3,
	CHEQUE_PIN = GPIO_PIN_4,
	SAVING_PIN = GPIO_PIN_6,
} pushButtonPinTypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RED_LED    (RGBColor) {255,0,0}
#define YELLOW_LED (RGBColor) {255,255,0}
#define ORANGE_LED (RGBColor) {255,165,0}
#define BLUE_LED   (RGBColor) {0,0,255}
#define WHITE_LED  (RGBColor) {255,255,255}

#define PIN 1234

#define MATCH 1
#define NOT_MATCH 0

#define BANK_OK 1
#define BANK_NOT_OK 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile pushButtonPinTypeDef 	pinPressed = NONE;
volatile pushButtonStateTypeDef btnState   = NONE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

float checkIfAmountRec()
{
	float debitAmount = 0;

	int16_t result = 0;

	printf("waiting for debitAmount to be received on serial port\r\n"); //}
	result = scanf("%f", &debitAmount);
	if (result == 0)		//then somehow non-float chars were entered
	{						//and nothing was assigned to %f
		return 0;
	}
	return debitAmount;
}

void displayWelcome(void)
{
	char strBuffer[16] = { 0 };

	ssd1331_clear_screen(BLACK);
	snprintf(strBuffer, 16, "Welcome");
	ssd1331_display_string(0, 0, strBuffer, FONT_1206, WHITE);
}

void displayAmount(float amount)
{
	char strBuffer[16];

	ssd1331_clear_screen(BLACK);
	sprintf(strBuffer, "$%.2f", amount);
	ssd1331_display_string(0, 0, strBuffer, FONT_1206, WHITE);
}

void displayEnterPin(void)
{
	char strBuffer[16];
	ssd1331_clear_screen(BLACK);
	snprintf(strBuffer, 16, "Enter PIN");
	ssd1331_display_string(0, 0, strBuffer, FONT_1206, WHITE);
}

void displayIncorrectPin(void)
{
	char strBuffer[16];
	snprintf(strBuffer, 16, "Incorrect PIN");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);
}

void printReceipt(float amount)
{
	char strBuffer[16];
	char amountstr[9];
	ssd1331_clear_screen(BLACK);
	snprintf(strBuffer, 16, "Success");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);

	sprintf(amountstr,"%.1f",amount);
	snprintf(strBuffer, 7, "Total: ");
	strcat(strBuffer,amountstr);
	ssd1331_display_string(0, 20, strBuffer, FONT_1206, WHITE);
}

void displayWaitingBankReply(void)
{
	char strBuffer[16];

	snprintf(strBuffer, 16, "PIN is correct");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);

	snprintf(strBuffer, 16, "Waiting for Bank");
	ssd1331_display_string(0, 20, strBuffer, FONT_1206, WHITE);
}

void displayTransactionCancle(void)
{
	char strBuffer[16];

	snprintf(strBuffer, 16, "Transaction Err");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);

	snprintf(strBuffer, 16, "Cancle");
	ssd1331_display_string(0, 20, strBuffer, FONT_1206, WHITE);
}
/* FUNCTION     : displayOkOrCancel()
* DESCRIPTION   : displays "OK or Cancel?" on line 2 of OLED
* PARAMETERS    : none
* RETURNS       : nothing.
*/
void displayOkCancel(void) {
	char strBuffer[16];
	sprintf(strBuffer, "OK or Cancel?");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);
}

/* FUNCTION     : displayTransactionCancel()
* DESCRIPTION   : displays "Transaction Cancle" on line 2 of OLED
* PARAMETERS    : none
* RETURNS       : nothing.
*/
void displayTransactionCancel(void) {
	char strBuffer[16];
	ssd1331_clear_screen(BLACK);
	sprintf(strBuffer, "Transaction Cancle");
	ssd1331_display_string(0, 16, strBuffer, FONT_1206, WHITE);
}

/* FUNCTION     : displayCheuqingOrSaving()
* DESCRIPTION   : displays "Chequing or Saving?" on line 2 of OLED
* PARAMETERS    : none
* RETURNS       : nothing.
*/
void displayCheuqingOrSaving(void) {
	char strBuffer[16];
	ssd1331_clear_screen(BLACK);
	sprintf(strBuffer, "Chequing or Saving?");
	ssd1331_display_string(0, 10, strBuffer, FONT_1206, WHITE);
}

uint16_t getUserPin(void)
{
	uint16_t usrPin = 0;
	int16_t  result = 0;

	printf("waiting for PIN to be received on serial port\r\n"); //}
	scanf("%d", &usrPin);

	return usrPin;
}

bool checkPin(uint16_t usrPin)
{
	if (usrPin == PIN)
	{
		return MATCH;
	}
	else
	{
		return NOT_MATCH;
	}
}

bool checkBankApprove(void)
{
	char str[16];
	scanf("%s", &str);
	if (strcmp(str, "OK") == 0)
	{
		return BANK_OK;
	}
	else
	{
		return BANK_NOT_OK;
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
	pinPressed = GPIO_Pin;

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

        // Check the button state after debounce time
        if (pinPressed = OK_PIN && HAL_GPIO_ReadPin(GPIOA,OK_PIN) == GPIO_PIN_RESET)
        {
        	// Button is still pressed, call the callback function
        	btnState = OK;
        }
        else if (pinPressed = CANCLE_PIN && HAL_GPIO_ReadPin(GPIOA,CANCLE_PIN) == GPIO_PIN_RESET)
        {
        	btnState = CANCLE;
        }
        else if (pinPressed = CHEQUE_PIN && HAL_GPIO_ReadPin(GPIOA,CHEQUE_PIN) == GPIO_PIN_RESET)
        {
            btnState = CHEQUE;
        }
        else if (pinPressed = SAVING_PIN && HAL_GPIO_ReadPin(GPIOA,SAVING_PIN) == GPIO_PIN_RESET)
        {
        	btnState = SAVING;
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
	int transactionState = 1;
	float amount     = 0;
	uint16_t usrPin   = 0;
	uint8_t attempt   = 3;

	bool correctPin  = false;
	bool bankApprove = false;
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
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  //Start timer 1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);

  // Start OLed and Display a green rectangle
  ssd1331_init();
  ssd1331_clear_screen(BLACK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(transactionState)
	  {
		  case 1:  // Display Welcome and user Balance
			  displayWelcome();
			  // Receive balance from Putty/Tera Term
			  amount = checkIfAmountRec();

			  if (amount != 0)
			  {
				  displayAmount(amount);
				  displayOkCancel();
				  transactionState = 2;
			  }
			  break;
		  case 2: // Check OK or Cancle?
			  if (btnState != NOTHING)
			  {
				  if (btnState == CANCLE)
				  {
					  //then cancel was pressed.
					  printf("Cancel Pressed\r\n");
					  transactionState = 7;
					  break;
				  }
				  else if (btnState == OK)
				  {
					  //then ok pressed
					  printf("OK Pressed\r\n");
					  displayCheuqingOrSaving();
					  transactionState = 3;
					  break;
				  }
				  break;
			  }

		  case 3: //Check Chequings or Savings
			  if (btnState != NOTHING)
			  {
				  if (btnState == CANCLE)
				  {
					  //then cancel was pressed.
					  printf("Cancel Pressed\r\n");
					  transactionState = 7;
				  }
				  else if (btnState == CHEQUE)
				  {
					  //then ok pressed
					  printf("Cheque Pressed\r\n");
					  transactionState = 4;
				  }
				  else if (btnState == SAVING)
				  {
					  //then ok pressed
					  printf("Saving Pressed\r\n");
					  transactionState = 4;
				  }
			  }
			  break;
		  case 4: // User enter PIN

			  displayEnterPin();
			  usrPin = getUserPin();
			  displayOkCancel();
			  correctPin = checkPin(attempt);
			  if (correctPin)
			  {
				  transactionState = 5;
				  break;
			  }
			  else
			  {
				  usrPin = NULL;
				  displayIncorrectPin();
				  attempt--;
			  }

			  // No Attempt left
			  if (attempt == 0)
			  {
				  transactionState = 7;
			  }
			  break;
		  case 5:  //Check OK or Cancle
			  if (btnState != NOTHING)
			  {
				  if (btnState == CANCLE)
				  {
					  //then cancel was pressed.
					  printf("Cancel Pressed\r\n");
					  transactionState = 7;
				  }
				  else if (btnState == OK)
				  {
					  //then ok pressed
					  printf("OK Pressed\r\n");
					  displayWaitingBankReply();
					  transactionState = 6;
				  }
			  }
			  break;

		  case 6: // Send transaction detail to the bank
			  bankApprove = checkBankApprove();
			  if (bankApprove)
			  {
				  printReceipt(amount);
				  HAL_Delay(3000);
				  transactionState = 1;
			  }
			  else
			  {
				  transactionState = 6;
			  }
			  break;
		  case 7:
			  displayTransactionCancle();
			  HAL_Delay(2000); // Delay 2s
			  transactionState = 1;
			  break;
		  default:
			  break;
	  }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  return 0;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  htim1.Init.Prescaler = 12;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 255;
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
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
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
  htim2.Init.Prescaler = 31999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|SSD1331_CS_Pin|SSD1331_DC_Pin|SSD1331_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA3 PA4 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin SSD1331_CS_Pin SSD1331_DC_Pin SSD1331_RES_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|SSD1331_CS_Pin|SSD1331_DC_Pin|SSD1331_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

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
