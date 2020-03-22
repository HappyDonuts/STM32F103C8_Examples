/**
  ******************************************************************************
  * @file    ads155.h
  * @author  Javier Morales
  * @brief   Header file of ads155 ADC module.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

//#include "stm32f3xx_hal.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>

typedef struct ads_t {
	uint8_t addr;
	I2C_HandleTypeDef *hi2c;
	uint8_t gain;
	uint8_t mux;
} ads_t;

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
ads_t* ads_new(I2C_HandleTypeDef *hi2c, uint8_t addr);

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
void ads_init(ads_t *ads, I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief  Configures the ads module register before reading (mux and gain)
 * 		   sending 3 bytes using I2C
 * @param  *ads: variable corresponding to the module targeted
 * @retval None
 */
void ads_write(ads_t *ads);

/**
 * @brief  Auxiliary function. Converts a raw ADC reading to a voltage reading in mV
 * @param  reading: raw ADC reading
 * @param  gain: gain of the internal amplifier chosen
 * @retval Voltage reading in mV
 */
uint16_t ads_voltageConv(uint16_t reading, uint8_t gain);


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
uint16_t ads_read(ads_t *ads, uint8_t mux, uint8_t gain);
