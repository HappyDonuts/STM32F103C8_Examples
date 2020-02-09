/**
  ******************************************************************************
  * @file    mcp4725.h
  * @author  Javier Morales
  * @brief   Header file of mcp4725 I2C DAC module.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal.h"
#include <stdlib.h>

typedef struct mcp_t {
	uint8_t addr;
	I2C_HandleTypeDef *hi2c;
	uint8_t buffer[3];
} mcp_t;

/**
 * @brief  Creates new mcp_t variable corresponding to a mcp module
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module
 * @retval mcp_t variable corresponding to the mcp module
 */
mcp_t* mcp_new(I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief  Initialization of mcp4725 module
 * @param  *mcp: mcp variable corresponding to the module targeted
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module
 * @retval None
 */
void mcp_init(mcp_t *mcp, I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief  Writes a value on the mcp4725 DAC module
 * @param  value: From 0 to 4095 (12 bits) value to be written on the DAC
 * @param  eeprom: If 1, saves the value on eeprom (persistence after reset)
 * @retval None
 */
void mcp_write(mcp_t *mcp, uint16_t value, uint8_t eeprom);
