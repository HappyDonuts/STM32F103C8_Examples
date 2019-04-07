/*
 * print_UART.h
 *
 *  Created on: Apr 6, 2019
 *      Author: Javi
 */

#ifndef PRINT_UART_H_
#define PRINT_UART_H_

#include "stm32f1xx_hal.h"

void tx_UART_int_nospace(UART_HandleTypeDef *huart, int data, uint32_t Timeout);
void tx_UART_int(UART_HandleTypeDef *huart, int data, uint32_t Timeout);
void tx_UART_float(UART_HandleTypeDef *huart, float data, uint8_t decimales, uint32_t Timeout);
void tx_UART_byte(UART_HandleTypeDef *huart, uint8_t data, uint32_t Timeout);

#endif /* PRINT_UART_H_ */
