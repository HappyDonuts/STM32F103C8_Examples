/**
  ******************************************************************************
  * @file    ads155.c
  * @author  Javier Morales
  * @brief   C file of ads155 ADC module functions.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

#include "ads115.h"

/**
 * @brief  Creates new ads_t variable corresponding to a ads module
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module:
 * 			ADDR -> GND: 0x48
 * 			addr -> VDD: 0x49
 * 			addr -> SDA: 0x4A
 * 			addr -> SCL: 0x4B
 * @retval ads_t variable corresponding to the ads module
 */
ads_t* ads_new(I2C_HandleTypeDef *hi2c, uint8_t addr){
	ads_t* ads = malloc(sizeof(*ads));
	ads_init(ads, hi2c, addr);
	return ads;
}

/**
 * @brief  Initialization of ads115 module
 * @param  *ads: ads variable corresponding to the module targeted
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module:
 * 			ADDR -> GND: 0x48
 * 			addr -> VDD: 0x49
 * 			addr -> SDA: 0x4A
 * 			addr -> SCL: 0x4B
 * @retval None
 */
void ads_init(ads_t *ads, I2C_HandleTypeDef *hi2c, uint8_t addr){
	ads->hi2c = hi2c;
	ads->addr = addr;
}

/**
 * @brief  Configures the ads module register before reading (mux and gain)
 * 		   sending 3 bytes using I2C
 * @param  *ads: variable corresponding to the module targeted
 * @retval None
 */
void ads_write(ads_t *ads){
	uint8_t adsWrite[3];
	adsWrite[0] = 0x01;
	adsWrite[1] = 0x80 + (ads->mux<<4) + (ads->gain<<1);
	adsWrite[2] = 0x83;
	HAL_I2C_Master_Transmit(ads->hi2c, ads->addr<<1, adsWrite, 3, 100);
}

/**
 * @brief  Auxiliary function. Converts a raw ADC reading to a voltage reading in mV
 * @param  reading: raw ADC reading
 * @param  gain: gain of the internal amplifier chosen
 * @retval Voltage reading in mV
 */
uint16_t ads_voltageConv(uint16_t reading, uint8_t gain){
	float factor;
	switch (gain){
		case 0: factor = 6144; break;
		case 1: factor = 4096; break;
		case 2: factor = 2048; break;
		case 3: factor = 1024; break;
		case 4: factor = 512; break;
		default: factor = 256; break;
	}
	factor = factor*reading/32768;
	return (uint16_t) factor;
}

/**
 * @brief  Reads an analog value using the ADC
 * @note  First reading:   250 us (72 MHz)
 * 		  Later readings:  150 us (72 MHz)
 * @note  Wait at least 15 ms after changing mux
 * @note  Do not change gain once sent
 * @param  *ads: variable corresponding to the module targeted
 * @param  mux: channel of the ADC:
 * 			0: 0 and 1		1: 0 and 3
 * 			2: 1 and 3		3: 2 and 3
 * 			4: 0 and GND	5: 1 and GND
 * 			6: 2 and GND	7: 3 and GND
 * @param  gain: gain of the internal amplifier:
 * 			0: +-6144 mV	1: +-4096 mV
 * 			2: +-2048 mV	3: +-1024 mV
 * 			4: +-512 mV		5, 6, 7: +-256 mV
 * @retval Voltage read by the module (mV)
 */
uint16_t ads_read(ads_t *ads, uint8_t mux, uint8_t gain){
	if ((ads->mux != mux) | (ads->gain != gain)){
		ads->mux = mux;
		ads->gain = gain;
		ads_write(ads);
	}

	uint8_t adsRead[2];
	HAL_I2C_Master_Transmit(ads->hi2c, ads->addr<<1, 0x00, 1, 100);
	HAL_I2C_Master_Receive(ads->hi2c, ads->addr<<1, adsRead, 2, 100);
	int16_t reading = (adsRead[0] << 8 | adsRead[1]);
	if (reading < 0){
	  reading = 0;
	}
	return ads_voltageConv(reading, gain);
}
