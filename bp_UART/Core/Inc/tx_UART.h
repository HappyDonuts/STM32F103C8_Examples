/**
  ******************************************************************************
  * @file    tx_UART.h
  * @author  Javier Morales
  * @brief   Header file for numbers representation through UART transmissionn.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

#ifndef INC_TX_UART_H_
#define INC_TX_UART_H_

#include "main.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct uart_t {
	UART_HandleTypeDef *huart;
} uart_t;

/**
 * @brief  Creates new uart_t variable corresponding to a uart paeripehral
 * @param  *uart: uart peripheral from the mcu
 * @retval uart_t variable corresponding to uart paeripehral
 */
uart_t* tx_UART_new(UART_HandleTypeDef *huart);

/**
 * @brief  Initialization of uart module
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  *huart: UART peripheral from the mcu
 * @retval None
 */
void tx_UART_init(uart_t *uart, UART_HandleTypeDef *huart);

/**
 * @brief  Trasmits an integer through UART
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  integer: integer to be trasnmitted
 * @retval None
 */
void tx_UART_int(uart_t* uart, int integer);

/**
 * @brief  Trasmits a double through UART
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  number: double to be trasnmitted
 * @param  decimal: number of decimals to be represented (round)
 * @retval None
 */
void tx_UART_double(uart_t* uart, double integer, uint8_t decimals);

/**
 * @brief  Returns the number of digits of an integer
 * @param  int: number to be inspected
 * @retval number of digits needed to represent the integer
 */
uint8_t tx_UART_digits(int integer);

#endif /* INC_TX_UART_H_ */
