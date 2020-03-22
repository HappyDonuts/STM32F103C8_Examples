/*
 * LED_Blink.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_blink.h"
/* PERIPHERALS ----------------------------- */

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
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(500);
	}
}

/**
  * @brief  EXTI line detection callbacks.
  * @retval none
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* GPIO CALLBACK CODE */
	
}
