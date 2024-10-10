/**
  ******************************************************************************
  * @file           : userInput.c

  * @brief          : functions used with user input system in menus
  * @author         : Allan Smith (Conestoga College)
  * @date           : 4-06-2024

  ******************************************************************************
  */

#include <stdio.h>

#include "userInput.h"

extern UART_HandleTypeDef huart2;

// FUNCTION      : ClearUART2RxBuffer
// DESCRIPTION   :
//   Clear any extra characters in UART buffer
//
// PARAMETERS    :
//   none
//
// RETURNS       :
//  none

void ClearUART2RxBuffer( void )
{
	char	RxBuffer[LENGTH_OF_INPUT_ARRAY] = {0};

	HAL_UART_Receive(&huart2, (uint8_t *)RxBuffer, LENGTH_OF_INPUT_ARRAY, 1);
}

// FUNCTION      : GetCharFromUART2
// DESCRIPTION   :
//   Get a single character of input from UART2 and return standard HAL GPIO
//   state information
// PARAMETERS    :
//   none
// RETURNS       :
//  character received

char GetCharFromUART2( void )
{
  char	RxBuffer[LENGTH_OF_INPUT_ARRAY] = {0};

  HAL_StatusTypeDef UartStatus = HAL_OK;

  UartStatus = HAL_UART_Receive(&huart2, (uint8_t *)RxBuffer, 1, READ_TIMEOUT_LONG);


  // check if Receive got a character
  if( !((UartStatus == HAL_TIMEOUT) || (UartStatus == HAL_OK)) )
  {
	  printf("ERROR in GetCharFromUART2\n\r");  // nothing received or error so signal that via a print
	  return TIMEOUT_ERROR;
  }
  if(UartStatus == HAL_TIMEOUT)
  {
	  return TIMEOUT_ERROR;
  }
  return *RxBuffer;
}

