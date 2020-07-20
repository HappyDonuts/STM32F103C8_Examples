/*
 * test_spi_lcd.c
 *
 *  Created on: May 5, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_spi_lcd.h"

//#include "MY_ILI9341.h"
#include "MY_ILI9341.h"
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
	ILI9341_Init(spi, GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_3, GPIOB, GPIO_PIN_0);
	ILI9341_setRotation(2);
	ILI9341_Fill(COLOR_WHITE);


	while(1){
		/* WHILE CODE */
		for (uint16_t i=0;i<200;i++){
			for (uint8_t j=0;j<200;j++){
				ILI9341_DrawPixel(i, j, COLOR_BLACK);
			}
		}
	}
}

/**
  * @brief  EXTI line detection callbacks.
  * @retval none
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* GPIO CALLBACK CODE */
	
}
