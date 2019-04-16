/*
 * print_UART.h
 *
 *  Created on: Apr 6, 2019
 *      Author: Javi
 */

#ifndef PRINT_UART_H_
#define PRINT_UART_H_

#include "stm32f1xx_hal.h"
//#include "stm32f3xx_hal.h"

/**
 * @brief  Trasmits int through UART
 * @param  *huart: UART peripheral
 * @param  data: int to be trasnmitted
 * @param  Timeout: Timeout UART
 * @retval None
 */
void tx_UART_int(UART_HandleTypeDef *huart, int data, uint32_t Timeout);

/**
 * @brief  Trasmits double through UART
 * @param  *huart: UART peripheral
 * @param  data: double to be trasnmitted
 * @param  decimal_p: number of decimal places to be printed
 * @param  Timeout: Timeout UART
 * @retval None
 */
void tx_UART_double(UART_HandleTypeDef *huart, double data, uint8_t decimal_p, uint32_t Timeout);

/**
 * @brief  Trasmits uint8_t (byte) through UART
 * @param  *huart: UART peripheral
 * @param  data: uint8_t to be trasnmitted
 * @param  Timeout: Timeout UART
 * @retval None
 */
void tx_UART_byte(UART_HandleTypeDef *huart, uint8_t data, uint32_t Timeout);

/**
 * @brief  Calculates the number of digits of an int
 * @param  data: int te be processed
 * @retval Number of digits of the int
 */
uint8_t size_int(int data);

#endif /* PRINT_UART_H_ */
