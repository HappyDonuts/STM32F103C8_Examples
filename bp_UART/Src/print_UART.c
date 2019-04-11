#include "print_UART.h"

void tx_UART_int_nospace(UART_HandleTypeDef *huart, int data, uint32_t Timeout)
{

	int size = 1;
	uint8_t negativo = 0;

	if (data < 0) {	// Si los pulsos
		data = -data;
		negativo = 1;
	}

	int numero = data;

	while(numero > 9) {
	  numero =  numero/10;
	  size++;
	}

	char data_char[size];		// String de chars
	uint8_t data_tx[size];	// String de uint8_t

	sprintf(data_char,"%d", data);	// Cada numero del int en un char

	for(uint8_t i=0; i<size; i++ ) {			// Casting de char a uint8_t
		data_tx[i] = (uint8_t) data_char[i];
	}

	if (negativo) {		// Si el numero es negativo, transmite un "-" antes
		uint8_t menos[] = "-";
		HAL_UART_Transmit(huart, menos, 1, 10);
	}
	HAL_UART_Transmit(huart,data_tx,sizeof(data_tx), 10);	// TX por UART del array de uint8_t


}

void tx_UART_int(UART_HandleTypeDef *huart, int data, uint32_t Timeout) {
	tx_UART_int_nospace(huart, data, 10);
	uint8_t salto[] = "\r\n";
	HAL_UART_Transmit(huart, salto, 2, 10);
}

void tx_UART_float(UART_HandleTypeDef *huart, float data, uint8_t decimales, uint32_t Timeout) {
	int potencia_10 = 1;
	for (uint8_t i=0; i<decimales; i++){
		potencia_10 = potencia_10*10;
	}

	int p_entera = data/1;
	int p_decimal = ((data - p_entera)*potencia_10)/1;

	tx_UART_int_nospace(huart, p_entera, 10);

	uint8_t punto[] = ".";
	HAL_UART_Transmit(huart, punto, 1, 10);

	if (p_decimal < 10) {
		uint8_t cero[] = "0";
		HAL_UART_Transmit(huart, cero, 1, 10);
	}

	tx_UART_int_nospace(huart, p_decimal, 10);

	uint8_t salto[] = "\r\n";
	HAL_UART_Transmit(huart, salto, 2, 10);
}

void tx_UART_byte(UART_HandleTypeDef *huart, uint8_t data, uint32_t Timeout){

	uint8_t byte[8];
	for(uint8_t i=0;i<8;i++){
		if((data>>(7-i) & 0x01)==1){
			byte[i]=0x31;
		}
		if((data>>(7-i) & 0x01)==0){
			byte[i]=0x30;
		}
	}
	HAL_UART_Transmit(huart, byte, sizeof(byte), 10);

	uint8_t salto[] = "\r\n";
	HAL_UART_Transmit(huart, salto, 2, 10);

//	uint8_t salto[] = "\r\n";
//	HAL_UART_Transmit(huart, salto, 2, 10);
}
