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
#include "ssd1306_basic.h"

/* SSD1306 data buffer */
//static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

ssd1306_t* ssd1306_new(I2C_HandleTypeDef *hi2c, uint8_t addr){
	ssd1306_t* ssd1306 = malloc(sizeof(*ssd1306));
	SSD1306_Init(ssd1306, hi2c, addr);
	return ssd1306;
}

uint8_t SSD1306_Init(ssd1306_t* ssd1306, I2C_HandleTypeDef *hi2c, uint8_t addr) {

	ssd1306->hi2c = hi2c;
	ssd1306->addr = addr;
	
	/* Check if LCD connected to I2C */
	if (HAL_I2C_IsDeviceReady(ssd1306->hi2c, ssd1306->addr, 1, 20000) != HAL_OK) {
		/* Return false */
		return 0;
	}
	
	/* A little delay */
//	uint32_t p = 2500;
//	while(p>0)
//		p--;
	
	/* Init LCD */
	ssd1306_I2C_Write(ssd1306, 0x00, 0xAE);
	ssd1306_I2C_Write(ssd1306, 0x00, 0xAE); //display off
	ssd1306_I2C_Write(ssd1306, 0x00, 0x20); //Set Memory Addressing Mode
	ssd1306_I2C_Write(ssd1306, 0x00, 0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	ssd1306_I2C_Write(ssd1306, 0x00, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	ssd1306_I2C_Write(ssd1306, 0x00, 0xC8); //Set COM Output Scan Direction
	ssd1306_I2C_Write(ssd1306, 0x00, 0x00); //---set low column address
	ssd1306_I2C_Write(ssd1306, 0x00, 0x10); //---set high column address
	ssd1306_I2C_Write(ssd1306, 0x00, 0x40); //--set start line address
	ssd1306_I2C_Write(ssd1306, 0x00, 0x81); //--set contrast control register
	ssd1306_I2C_Write(ssd1306, 0x00, 0xFF);
	ssd1306_I2C_Write(ssd1306, 0x00, 0xA1); //--set segment re-map 0 to 127
	ssd1306_I2C_Write(ssd1306, 0x00, 0xA6); //--set normal display
	ssd1306_I2C_Write(ssd1306, 0x00, 0xA8); //--set multiplex ratio(1 to 64)
	ssd1306_I2C_Write(ssd1306, 0x00, 0x3F); //
	ssd1306_I2C_Write(ssd1306, 0x00, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	ssd1306_I2C_Write(ssd1306, 0x00, 0xD3); //-set display offset
	ssd1306_I2C_Write(ssd1306, 0x00, 0x00); //-not offset
	ssd1306_I2C_Write(ssd1306, 0x00, 0xD5); //--set display clock divide ratio/oscillator frequency
	ssd1306_I2C_Write(ssd1306, 0x00, 0xF0); //--set divide ratio
	ssd1306_I2C_Write(ssd1306, 0x00, 0xD9); //--set pre-charge period
	ssd1306_I2C_Write(ssd1306, 0x00, 0x22); //
	ssd1306_I2C_Write(ssd1306, 0x00, 0xDA); //--set com pins hardware configuration
	ssd1306_I2C_Write(ssd1306, 0x00, 0x12);
	ssd1306_I2C_Write(ssd1306, 0x00, 0xDB); //--set vcomh
	ssd1306_I2C_Write(ssd1306, 0x00, 0x20); //0x20,0.77xVcc
	ssd1306_I2C_Write(ssd1306, 0x00, 0x8D); //--set DC-DC enable
	ssd1306_I2C_Write(ssd1306, 0x00, 0x14); //
	ssd1306_I2C_Write(ssd1306, 0x00, 0xAF); //--turn on SSD1306 panel
	
	/* Clear screen */
	SSD1306_Fill(ssd1306, SSD1306_COLOR_BLACK);
	
	/* Update screen */
	SSD1306_UpdateScreen(ssd1306);
	
	/* Set default values */
	ssd1306->CurrentX = 0;
	ssd1306->CurrentY = 0;
	
	/* Initialized OK */
	ssd1306->Initialized = 1;
	ssd1306->Inverted= 0;
	
	/* Return OK */
	return 1;
}

void SSD1306_UpdateScreen(ssd1306_t* ssd1306) {
	uint8_t m;

	for (m = 0; m < 8; m++) {
		ssd1306_I2C_Write(ssd1306, 0x00, 0xB0 + m);
		ssd1306_I2C_Write(ssd1306, 0x00, 0x00);
		ssd1306_I2C_Write(ssd1306, 0x00, 0x10);

		/* Write multi data */
		ssd1306_I2C_WriteMulti(ssd1306, 0x40, &ssd1306->SSD1306_Buffer[SSD1306_WIDTH * m], SSD1306_WIDTH);
	}
}

void SSD1306_Fill(ssd1306_t* ssd1306, SSD1306_COLOR_t color) {
	/* Set memory */
	memset(ssd1306->SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(ssd1306->SSD1306_Buffer));
}

void SSD1306_DrawPixel(ssd1306_t* ssd1306, uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}
	
	/* Check if pixels are inverted */
	if (ssd1306->Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}
	
	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		ssd1306->SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		ssd1306->SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1306_GotoXY(ssd1306_t* ssd1306, uint16_t x, uint16_t y) {
	/* Set write pointers */
	ssd1306->CurrentX = x;
	ssd1306->CurrentY = y;
}

char SSD1306_Putc(ssd1306_t* ssd1306, uint8_t ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;
	
	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (ssd1306->CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (ssd1306->CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}

	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				SSD1306_DrawPixel(ssd1306, ssd1306->CurrentX + j, (ssd1306->CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				SSD1306_DrawPixel(ssd1306, ssd1306->CurrentX + j, (ssd1306->CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}
	
	/* Increase pointer */
	ssd1306->CurrentX += Font->FontWidth;
	
	/* Return character written */
	return ch;
}

/**
 * @brief  Represents an integer number with a char array
 * @param  integer: int to be represented
 * @param  unit: unit of the value, 2 char array ("mV", " A", etc.)
 * @retval Char array containing a char for each digit
 */
char* int_str(int integer,  char* unit){
	char negative = ' ';
	if (integer < 0) {
		integer = -integer;
		negative = '-';
	}

	int size = SSD1306_digits(integer);
	if (size > 7){
			return "OVERLOAD";
	}
	char data_char[size];
	sprintf(data_char,"%d", integer);

	static char int_str[11];
	for(int i=1; i<11; i++){
		if (i<size+1){
			int_str[i] = data_char[i-1];
		} else if (i < 9){
			int_str[i] = ' ';
		} else {
			int_str[i] = unit[i-9];
		}
	}
	int_str[0] = negative;
	int_str[8] = ' ';
	return int_str;
}

/**
 * @brief  Puts integer to internal RAM
 * @note   @ref If the number has mora than 6 digits, displays OVERLOAD
 * @param  data: integer to be written
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @param  slot: slot to be used. 1, 2, and 3 are large slots. 4 and 5 are small slots
 */
void SSD1306_Putint(ssd1306_t* ssd1306, int data, char* unit, uint8_t slot) {

	char* integer_str = int_str(data, unit);
	uint16_t x_slot = 1;
	uint16_t y_slot = 0;
	FontDef_t font = Font_11x18;
	switch (slot) {
	case 1:
		y_slot = 0;
		break;
	case 2:
		y_slot = 17;
		break;
	case 3:
		y_slot = 34;
		break;
	case 4:
		y_slot = 53;
		font = Font_7x10;
		break;
	case 5:
		x_slot = 65;
		y_slot = 53;
		font = Font_7x10;
		break;
	}
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, "         ", &font, 1);
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, integer_str, &font, 1);
}

/**
 * @brief  Represents a double number with a char array
 * @param  number: double to be represented
 * @param  decimals: number of decimals to be represented
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @retval Char array containing a char for each digit
 */
char* double_str(double number, uint8_t decimals, char* unit){

	uint8_t negative = ' ';
	if (number < 0) {
		number = -number;
		negative = '-';
	}

	number = round(number*pow(10, decimals));
	uint8_t size;
	if (number == 0){
		size = decimals+1;
	} else {
		size = SSD1306_digits(number);
	}
	if (size > 6) {
		return "OVERLOAD";
	}

	char data_char[size];
	if (number == 0){
		for(uint8_t i=0;i<size;i++){
			data_char[i]='0';
		}
	} else {
		sprintf(data_char,"%d", (int)number);
	}

	static char double_str[11];
	for(int8_t i=1; i<11; i++){
		if (i < (size-decimals+1)){
			double_str[i] = data_char[i-1];
		} else if (i < size+2){
			double_str[i] = data_char[i-2];
		} else if (i < 11-2){
			double_str[i] = ' ';
		} else {
			double_str[i] = unit[i-9];
		}
	}
	double_str[0] = negative;
	double_str[size-decimals+1] = '.';
	double_str[8] = ' ';
	return double_str;
}

/**
 * @brief  Puts double to internal RAM
 * @note   @ref If the number has mora than 6 digits, displays OVERLOAD
 * @param  data: double to be written
 * @param  decimals: number of decimals to be represented
 * @param  unit: unit of the value, 2 char array (NO, MV, V, MA, A, "uV", "ms", etc.)
 * @param  slot: slot to be used. 1, 2, and 3 are large slots. 4 and 5 are small slots
 */
void SSD1306_Putdouble(ssd1306_t* ssd1306, double data, uint8_t decimals, char* unit, uint8_t slot) {
	char* float_str = double_str(data, decimals, unit);

	uint16_t x_slot = 1;
	uint16_t y_slot = 0;
	FontDef_t font = Font_11x18;
	switch (slot) {
	case 1:
		y_slot = 0;
		break;
	case 2:
		y_slot = 17;
		break;
	case 3:
		y_slot = 34;
		break;
	case 4:
		y_slot = 53;
		font = Font_7x10;
		break;
	case 5:
		x_slot = 65;
		y_slot = 53;
		font = Font_7x10;
		break;
	}

	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, "         ", &font, 1);
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, float_str, &font, 1);
}

/**
 * @brief  Calculates the number of digits needed to represent an integer
 * @param  int: integer to be processed
 * @retval Number of digits in an integer
 */
uint8_t SSD1306_digits(int integer){
	uint8_t digits;
	if (integer==0){
		digits = 1;
	}
	else {
		digits = floor(log10(integer))+1;
	}
	return digits;
}

char SSD1306_Puts(ssd1306_t* ssd1306, char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (SSD1306_Putc(ssd1306, *str, Font, color) != *str) {
			/* Return error */
			return *str;
		}
		
		/* Increase string pointer */
		str++;
	}
	
	/* Everything OK, zero should be returned */
	return *str;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  _____ ___   _____ 
// |_   _|__ \ / ____|
//   | |    ) | |     
//   | |   / /| |     
//  _| |_ / /_| |____ 
// |_____|____|\_____|
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ssd1306_I2C_WriteMulti(ssd1306_t* ssd1306, uint8_t reg, uint8_t* data, uint16_t count) {
	uint8_t dt[count + 1];
	dt[0] = reg;
	uint8_t i;
	for(i = 1; i <= count; i++)
		dt[i] = data[i-1];
	HAL_I2C_Master_Transmit(ssd1306->hi2c, ssd1306->addr, dt, count, 10);
}

void ssd1306_I2C_Write(ssd1306_t* ssd1306, uint8_t reg, uint8_t data) {
	uint8_t dt[2];
	dt[0] = reg;
	dt[1] = data;
	HAL_I2C_Master_Transmit(ssd1306->hi2c, ssd1306->addr, dt, 2, 10);
}
