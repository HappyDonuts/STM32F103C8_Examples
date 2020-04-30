/*
 * oled_test.c
 *
 *  Created on: Mar 14, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "oled_test.h"
#include "ssd1306_basic.h"
/* PERIPHERALS ----------------------------- */
extern I2C_HandleTypeDef hi2c1;

I2C_HandleTypeDef* i2c = &hi2c1;
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */
ssd1306_t* ssd1306_1;
/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	ssd1306_1 = ssd1306_new(i2c, 0x78); // 0x79
	
//	double data = -30.12345678;
	double data = -0.041;
	SSD1306_Putdouble(ssd1306_1, data, 2, NO, 1);
	SSD1306_Putdouble(ssd1306_1, data, 4, MV, 2);
	SSD1306_Putdouble(ssd1306_1, data, 6, MV, 3);
	SSD1306_Putdouble(ssd1306_1, data, 1, NO, 4);
	SSD1306_Putdouble(ssd1306_1, data, 4, NO, 5);
	SSD1306_UpdateScreen(ssd1306_1);

//	int data_i = -0;
//	SSD1306_Putint(ssd1306_1, data_i,  NO, 1);
//	SSD1306_Putint(ssd1306_1, data_i,  MA, 2);
//	SSD1306_Putint(ssd1306_1, 12345678,  MV, 3);
//	SSD1306_Putint(ssd1306_1, 1234567, NO, 4);
//	SSD1306_Putint(ssd1306_1, -1234567, NO, 5);
//	SSD1306_UpdateScreen(ssd1306_1);

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
