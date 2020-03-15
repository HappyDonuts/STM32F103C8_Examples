/*
 * uart.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "uart_test.h"
#include "tx_UART.h"
/* PERIPHERALS ----------------------------- */
extern UART_HandleTypeDef huart1;
UART_HandleTypeDef* uart = &huart1; // A9 TX
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */
uart_t* uart_1;
/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	uart_1 = tx_UART_new(uart);

	double number = -99.000912;
	//tx_UART_int(uart_1, number);
	tx_UART_double(uart_1, number, 4);
	
	while(1){
		/* WHILE CODE */
		
	}
}

