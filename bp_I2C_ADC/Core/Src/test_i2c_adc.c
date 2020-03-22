/*
 * test_i2c_adc.c
 *
 *  Created on: Mar 22, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_i2c_adc.h"

#include "ads115.h"
#include "ssd1306_basic.h"
/* PERIPHERALS ----------------------------- */
I2C_HandleTypeDef hi2c1;

I2C_HandleTypeDef* i2c = &hi2c1;
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */
ssd1306_t* ssd1306_1;
ads_t* ads_1;
/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	 ads_1 = ads_new(i2c, 0x48);
	 ssd1306_1 = ssd1306_new(i2c, 0x79);

	 int16_t v_1;
	
	while(1){
		/* WHILE CODE */
		HAL_Delay(100);
		v_1 = ads_read(ads_1, 5, 0);
		
		SSD1306_Putint(ssd1306_1, v_1, MV, 1);
		SSD1306_UpdateScreen(ssd1306_1);
	}
}
