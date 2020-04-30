/*
 * test_timing.c
 *
 *  Created on: Mar 22, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_timing.h"

#include "ssd1306_basic.h"
#include "timing.h"
/* PERIPHERALS ----------------------------- */
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;

I2C_HandleTypeDef* i2c = &hi2c1;
TIM_HandleTypeDef* tim = &htim2;
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */
ssd1306_t* ssd1306_1;
timing_t* timing_1;
/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	ssd1306_1 = ssd1306_new(i2c, 0x79);
	timing_1 = timing_new(tim);

//	timing_start_ms(timing_1);
	volatile double ex_time;
	timing_start_us(timing_1);
//	ex_time = timing_get_ms(timing_1);
	ex_time = timing_get_us(timing_1);
	
	SSD1306_Putdouble(ssd1306_1, ex_time, 1, "ms", 1);
	SSD1306_UpdateScreen(ssd1306_1);
	
	while(1){
		/* WHILE CODE */
		
	}
}

/**
  * @brief  Period elapsed callback.
  * @retval none
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	/* TIM CALLBACK CODE */
	
}

