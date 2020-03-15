/**
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>
 * optimization: Javier Morales

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#ifndef SSD1306_H
#define SSD1306_H 100

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include "fonts.h"
#include <math.h>


/* SSD1306 width in pixels */
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH            132 // Menos para el display mini
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT           64
#endif

#define NO 	"  "
#define MV 		"mV"
#define V 		" V"
#define MA 		"mA"
#define A	    " A"

/**
 * @brief  SSD1306 color enumeration
 */
typedef enum SSD1306_COLOR_t{
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;

/**
 * @brief  SSD1306 struct
 */
typedef struct ssd1306_t{
	uint8_t addr;
	I2C_HandleTypeDef *hi2c;
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
	uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
} ssd1306_t;

/**
 * @brief  Creates ssd1306_t variable corresponding to the display
 * @param  *hi2c: I2C peripheral used by the mcu
 * @param  addr: I2C address corresponding to the targeted display
 * @retval ssd1306_t variable targeting the desired display
 */
ssd1306_t* ssd1306_new(I2C_HandleTypeDef *hi2c, uint8_t addr);

/**
 * @brief  Initializes SSD1306 LCD
 * @param  *hi2c: I2C peripheral used by the mcu
 * @param  addr: I2C address corresponding to the targeted display
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
uint8_t SSD1306_Init(ssd1306_t* ssd1306, I2C_HandleTypeDef *hi2c, uint8_t addr);

/** 
 * @brief  Updates buffer from internal RAM to LCD
 * @note   This function must be called each time you do some changes to LCD, to update buffer from RAM to LCD
 * @param  None
 * @retval None
 */
void SSD1306_UpdateScreen(ssd1306_t* ssd1306);

/**
 * @brief  Toggles pixels invertion inside internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  None
 * @retval None
 */
void SSD1306_ToggleInvert(ssd1306_t* ssd1306);

/** 
 * @brief  Fills entire LCD with desired color
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  Color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_Fill(ssd1306_t* ssd1306, SSD1306_COLOR_t color);

/**
 * @brief  Draws pixel at desired location
 * @note   @ref SSD1306_UpdateScreen() must called after that in order to see updated LCD screen
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @param  color: Color to be used for screen fill. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval None
 */
void SSD1306_DrawPixel(ssd1306_t* ssd1306, uint16_t x, uint16_t y, SSD1306_COLOR_t color);

/**
 * @brief  Sets cursor pointer to desired location for strings
 * @param  x: X location. This parameter can be a value between 0 and SSD1306_WIDTH - 1
 * @param  y: Y location. This parameter can be a value between 0 and SSD1306_HEIGHT - 1
 * @retval None
 */
void SSD1306_GotoXY(ssd1306_t* ssd1306, uint16_t x, uint16_t y);

/**
 * @brief  Puts character to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  ch: Character to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Character written
 */
char SSD1306_Putc(ssd1306_t* ssd1306, uint8_t ch, FontDef_t* Font, SSD1306_COLOR_t color);

/**
 * @brief  Puts string to internal RAM
 * @note   @ref SSD1306_UpdateScreen() must be called after that in order to see updated LCD screen
 * @param  *str: String to be written
 * @param  *Font: Pointer to @ref FontDef_t structure with used font
 * @param  color: Color used for drawing. This parameter can be a value of @ref SSD1306_COLOR_t enumeration
 * @retval Zero on success or character value when function failed
 */
char SSD1306_Puts(ssd1306_t* ssd1306, char* str, FontDef_t* Font, SSD1306_COLOR_t color);

/**
 * @brief  Represents an integer number with a char array
 * @param  integer: int to be represented
 * @param  unit: unit of the value, 2 char array ("mV", " A", etc.)
 * @retval Char array containing a char for each digit
 */
char* int_str(int integer,  char* unit);

/**
 * @brief  Puts integer to internal RAM
 * @note   @ref If the number has mora than 6 digits, displays OVERLOAD
 * @param  data: integer to be written
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @param  slot: slot to be used. 1, 2, and 3 are large slots. 4 and 5 are small slots
 */
void SSD1306_Putint(ssd1306_t* ssd1306, int data, char* unit, uint8_t slot);

/**
 * @brief  Represents a double number with a char array
 * @param  number: double to be represented
 * @param  decimals: number of decimals to be represented
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @retval Char array containing a char for each digit
 */
char* double_str(double number, uint8_t decimals, char* unit);

/**
 * @brief  Puts double to internal RAM
 * @note   @ref If the number has mora than 6 digits, displays OVERLOAD
 * @param  data: double to be written
 * @param  decimals: number of decimals to be represented
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @param  slot: slot to be used. 1, 2, and 3 are large slots. 4 and 5 are small slots
 */
void SSD1306_Putdouble(ssd1306_t* ssd1306, double data, uint8_t decimals, char* unit, uint8_t slot);

/**
 * @brief  Calculates the number of digits needed to represent an integer
 * @param  int: integer to be processed
 * @retval Number of digits in an integer
 */
uint8_t SSD1306_digits(int integer);

#ifndef ssd1306_I2C_TIMEOUT
#define ssd1306_I2C_TIMEOUT					20000
#endif

/**
 * @brief  Initializes SSD1306 LCD
 * @param  None
 * @retval Initialization status:
 *           - 0: LCD was not detected on I2C port
 *           - > 0: LCD initialized OK and ready to use
 */
void ssd1306_I2C_Init();

/**
 * @brief  Writes single byte to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  data: data to be written
 * @retval None
 */
void ssd1306_I2C_Write(ssd1306_t* ssd1306, uint8_t reg, uint8_t data);

/**
 * @brief  Writes multi bytes to slave
 * @param  *I2Cx: I2C used
 * @param  address: 7 bit slave address, left aligned, bits 7:1 are used, LSB bit is not used
 * @param  reg: register to write to
 * @param  *data: pointer to data array to write it to slave
 * @param  count: how many bytes will be written
 * @retval None
 */
void ssd1306_I2C_WriteMulti(ssd1306_t* ssd1306, uint8_t reg, uint8_t* data, uint16_t count);



/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
