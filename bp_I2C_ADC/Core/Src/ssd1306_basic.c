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
#include <ssd1306_basic.h>
#include "math.h"

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

void SSD1306_Putint(ssd1306_t* ssd1306, int data, uint8_t slot) {

	char menos_char = ' ';
	if (data < 0) {
		data = -data;
		menos_char = '-';
	}

	int size = size_int_oled(data);
	char data_char[size];		// String de chars=
	sprintf(data_char,"%d", data);	// Cada numero del int en un char

	uint16_t x_slot = 0;
	uint16_t y_slot = 0;
	uint8_t slot_mini = 0;
	switch (slot) {
	case 1:
		x_slot = 2;
		y_slot = 0;
		break;
	case 2:
		x_slot = 2;
		y_slot = 17;
		break;
	case 3:
		x_slot = 2;
		y_slot = 35;
		break;
	case 4:
		x_slot = 22;
		y_slot = 53;
		slot_mini = 1;
		break;
	case 5:
		x_slot = 70;
		y_slot = 53;
		slot_mini = 1;
		break;
	}
	if (slot_mini == 1){
		SSD1306_GotoXY (ssd1306, x_slot,y_slot);				// Select x and y from the selected slot
		SSD1306_Puts (ssd1306, "       ", &Font_7x10, 1);	// Reset the slot before writing (7 blank chars for small slots)
		SSD1306_GotoXY (ssd1306, x_slot,y_slot);				// Come back to the initial position
		SSD1306_Putc (ssd1306, menos_char, &Font_7x10, 1);	// Write minus char if necessary
		SSD1306_GotoXY (ssd1306, x_slot+7,y_slot);			// Shif a char correspondig to the minus char
		SSD1306_Puts (ssd1306, data_char, &Font_7x10, 1);	// Write the value
		return;
	}
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, "         ", &Font_11x18, 1);		// Reset the slot before writing (9 blank chars for small slots)
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Putc (ssd1306, menos_char, &Font_11x18, 1);
	SSD1306_GotoXY (ssd1306, x_slot+11,y_slot);
	SSD1306_Puts (ssd1306, data_char, &Font_11x18, 1);
}

char* double_str(double data, uint8_t decimales){
	char negativo = ' ';
	if (data < 0) {
		data = -data;
		negativo = '-';
	}

	int potencia_10 = pow(10, decimales);
	double p_entera_f = trunc(data);
	double p_decimal_f = data*potencia_10 - p_entera_f*potencia_10;
	int p_entera = p_entera_f;
	int p_decimal = p_decimal_f;

	int size = size_int_oled(p_entera);
	char p_entera_char[size];		// String de chars
	sprintf(p_entera_char,"%d", p_entera);	// Cada numero del int en un char

	char p_decimal_char[decimales];
	size = size_int_oled(p_decimal);
	if (decimales>size){
		p_decimal = p_decimal + pow(10,decimales-1);
	}
	sprintf(p_decimal_char,"%d", p_decimal);
	for(uint8_t i=0; i<(decimales-size);i++){
		p_decimal_char[i] = '0';
	}

	uint8_t total_size = sizeof(p_entera_char)+decimales+2;
	static char float_str[10];

	for(uint8_t i= 0; i<11; i++){
		if (i==0){
			float_str[i] = negativo;
		} else if((i>0) && (i<sizeof(p_entera_char)+1)){
			float_str[i] = p_entera_char[i-1];
		} else if (i == sizeof(p_entera_char)+1){
			float_str[i] = '.';
		} else if ((i> sizeof(p_entera_char)+1) && (i<total_size)){
			float_str[i] = p_decimal_char[i-(sizeof(p_entera_char)+2)];
		} else {float_str[i] = ' ';}
	}
	return float_str;
}

void SSD1306_Putdouble(ssd1306_t* ssd1306, float data, uint8_t decimales, uint8_t slot) {
	char* float_str = double_str(data, decimales);

	uint16_t x_slot = 0;
	uint16_t y_slot = 0;
	uint8_t slot_mini = 0;
	switch (slot) {
	case 1:
		x_slot = 2;
		y_slot = 0;
		break;
	case 2:
		x_slot = 2;
		y_slot = 17;
		break;
	case 3:
		x_slot = 2;
		y_slot = 35;
		break;
	case 4:
		x_slot = 2;
		y_slot = 53;
		slot_mini = 1;
		break;
	case 5:
		x_slot = 60;
		y_slot = 53;
		slot_mini = 1;
		break;
	}
	if (slot_mini == 1){
		SSD1306_GotoXY (ssd1306, x_slot,y_slot);				// Select x and y from the selected slot
		SSD1306_Puts (ssd1306, "       ", &Font_7x10, 1);	// Reset the slot before writing (7 blank chars for small slots)
		SSD1306_GotoXY (ssd1306, x_slot,y_slot);				// Come back to the initial position
		SSD1306_Puts (ssd1306, float_str, &Font_7x10, 1);	// Write the value
//		SSD1306_UpdateScreen();						// Update the screen
		return;
	}
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, "         ", &Font_11x18, 1);		// Reset the slot before writing (9 blank chars for small slots)
	SSD1306_GotoXY (ssd1306, x_slot,y_slot);
	SSD1306_Puts (ssd1306, float_str, &Font_11x18, 1);
//	SSD1306_UpdateScreen();
}

uint8_t size_int_oled(int data){
	uint8_t size;
	if (data==0){
		size = 1;
	}
	else {
		size = floor(log10(data))+1;
	}
	return size;
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
