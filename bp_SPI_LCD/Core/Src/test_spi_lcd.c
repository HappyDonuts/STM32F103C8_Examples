/*
 * test_spi_lcd.c
 *
 *  Created on: May 5, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_spi_lcd.h"
/* PERIPHERALS ----------------------------- */
extern SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef* spi = &hspi1;
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */

/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	
	
	while(1){
		/* WHILE CODE */
		
	}
}

/**
  * @brief  EXTI line detection callbacks.
  * @retval none
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* GPIO CALLBACK CODE */
	
}
