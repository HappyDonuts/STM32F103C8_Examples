/*
 * test_dac_i2c.c
 *
 *  Created on: Mar 19, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_dac_i2c.h"

#include "mcp4725.h"
/* PERIPHERALS ----------------------------- */
extern I2C_HandleTypeDef hi2c1;

I2C_HandleTypeDef* i2c = &hi2c1;
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */
mcp_t* mcp_1; //DAC module
/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */
	mcp_1 = mcp_new(i2c, 0xC0); //0xC4 - other address
	HAL_Delay(200);
	
	uint16_t voltage = 1500; // mV
	uint16_t data_dac = voltage*4095/3300;
	mcp_write(mcp_1, data_dac, 1);

	while(1){
		/* WHILE CODE */
		
	}
}

