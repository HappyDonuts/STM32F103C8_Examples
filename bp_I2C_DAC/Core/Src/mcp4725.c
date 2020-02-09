/**
  ******************************************************************************
  * @file    mcp4725.c
  * @author  Javier Morales
  * @brief   C file of mcp4725 I2C DAC module functions.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

#include "mcp4725.h"

/**
 * @brief  Creates new mcp_t variable corresponding to a mcp module
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module
 * @retval mcp_t variable corresponding to the mcp module
 */
mcp_t* mcp_new(I2C_HandleTypeDef *hi2c, uint8_t addr){
	mcp_t* mcp = malloc(sizeof(*mcp));
	mcp_init(mcp, hi2c, addr);
	return mcp;
}

/**
 * @brief  Initialization of mcp4725 module
 * @param  *mcp: mcp variable corresponding to the module targeted
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module
 * @retval None
 */
void mcp_init(mcp_t *mcp, I2C_HandleTypeDef *hi2c, uint8_t addr){
	mcp->hi2c = hi2c;
	mcp->addr = addr;
}

/**
 * @brief  Writes a value on the mcp4725 DAC module
 * @param  value: From 0 to 4095 (12 bits) value to be written on the DAC
 * @param  eeprom: If 1, saves the value on eeprom (persistence after reset)
 * @retval None
 */
void mcp_write(mcp_t *mcp, uint16_t value, uint8_t eeprom){
	if (eeprom == 1) {
		mcp->buffer[0] = 0x60;	} // Persists after reset
	else {
		mcp->buffer[0] = 0x40;
	}
	mcp->buffer[1] = (value / 16);       // Upper data bits     (D11.D10.D9.D8.D7.D6.D5.D4)
	mcp->buffer[2] = (value % 16) << 4;  // Lower data bits     (D3.D2.D1.D0.x.x.x.x)
	HAL_I2C_Master_Transmit(mcp->hi2c, mcp->addr, mcp->buffer, sizeof(mcp->buffer), 1000);
}
