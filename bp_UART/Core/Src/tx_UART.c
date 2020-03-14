/**
  ******************************************************************************
  * @file    tx_UART.c
  * @author  Javier Morales
  * @brief   Source file for numbers representation through UART transmission.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

/* INCLUDES -------------------------------- */
#include "tx_UART.h"
#include "math.h"

/**
 * @brief  Creates new uart_t variable corresponding to a uart paeripehral
 * @param  *uart: uart peripheral from the mcu
 * @retval uart_t variable corresponding to uart paeripehral
 */
uart_t* tx_UART_new(UART_HandleTypeDef *huart){
	uart_t* uart = malloc(sizeof(*uart));
	tx_UART_init(uart, huart);
	return uart;
}

/**
 * @brief  Initialization of uart module
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  *huart: UART peripheral from the mcu
 * @retval None
 */
void tx_UART_init(uart_t *uart, UART_HandleTypeDef *huart){
	uart->huart = huart;
}

/**
 * @brief  Trasmits an integer through UART
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  integer: integer to be trasnmitted
 * @retval None
 */
void tx_UART_int(uart_t* uart, int integer){
	uint8_t negative = 0;
	if (integer < 0) {	// Si es negativo se pasa a positivo y activamos el flag
		integer = -integer;
		negative = 1;
	}

	uint8_t size = tx_UART_digits(integer);
	char data_char[size];		// String de chars
	sprintf(data_char,"%d", integer);	// Cada numero del int en un char

	size = size+negative+2;
	uint8_t int_str[size];

	int_str[size-1] = '\r';
	int_str[size-2] = '\n';

	for(int8_t i=(size-3); i>=0; i--){
		int_str[i] = (uint8_t) data_char[i-negative];
	}
	if (negative == 1) {
		int_str[0] = '-';
	}
	HAL_UART_Transmit(uart->huart, int_str, size, 1000);
}

/**
 * @brief  Trasmits a double through UART
 * @param  *uart: uart variable corresponding to the module targeted
 * @param  number: double to be trasnmitted
 * @param  decimal: number of decimals to be represented (round)
 * @retval None
 */
void tx_UART_double(uart_t* uart, double number, uint8_t decimals){
	uint8_t negative = 0;
		if (number < 0) {	// Si es negativo se pasa a positivo y activamos el flag
			number = -number;
			negative = 1;
		}

	number = round(number*pow(10, decimals));

	uint8_t size = tx_UART_digits(number);
	char data_char[size];		// String de chars
	sprintf(data_char,"%d", (int)number);	// Cada numero del int en un char

	size = size+negative+3;
	uint8_t int_str[size];

	for(int8_t i=(size-3); i>=0; i--){
		if (i>(size-decimals-3)){
			int_str[i] = (uint8_t) data_char[i-1-negative];
		} else if (i<size-decimals-3){
			int_str[i] = data_char[i-negative];
		}
	}
	int_str[size-1] = '\r';
	int_str[size-2] = '\n';
	int_str[size-decimals-3] = '.';
	if (negative == 1) {
		int_str[0] = '-';
	}
	HAL_UART_Transmit(uart->huart, int_str, size, 1000);
}

/**
 * @brief  Returns the number of digits of an integer
 * @param  int: number to be inspected
 * @retval number of digits needed to represent the integer
 */
uint8_t tx_UART_digits(int integer){
	uint8_t digits;
	if (integer==0){
		digits = 1;
	}
	else {
		digits = floor(log10(integer))+1;
	}
	return digits;
}
