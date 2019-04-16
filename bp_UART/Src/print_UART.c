#include "print_UART.h"
#include "math.h"

void tx_UART_int(UART_HandleTypeDef *huart, int data, uint32_t Timeout) {
	uint8_t negativo = 0;
	if (data < 0) {	// Si es negativo se pasa a positivo y activamos el flag
		data = -data;
		negativo = 1;
	}

	uint8_t size = size_int(data);
	char data_char[size];		// String de chars
	sprintf(data_char,"%d", data);	// Cada numero del int en un char

	size = size+negativo+2;
	uint8_t int_str[size];

	int_str[size-1] = '\r';
	int_str[size-2] = '\n';

	for(int8_t i=(size-3); i>=0; i--){
		int_str[i] = (uint8_t) data_char[i-negativo];
	}
	if (negativo == 1) {
		int_str[0] = '-';
	}
	HAL_UART_Transmit(huart,int_str,sizeof(int_str), 10);	// TX por UART del array de uint8_t
}

void tx_UART_double(UART_HandleTypeDef *huart, double data, uint8_t decimal_p, uint32_t Timeout) {
	uint8_t negativo = 0;
	if (data < 0) {
		data = -data;
		negativo = 1;
	}

	int potencia_10 = pow(10, decimal_p);
	double p_entera_f = trunc(data);
	double p_decimal_f = data*potencia_10 - p_entera_f*potencia_10;
	int p_entera = p_entera_f;
	int p_decimal = p_decimal_f;

	int size = size_int(p_entera);
	char p_entera_char[size];		// String de chars
	sprintf(p_entera_char,"%d", p_entera);	// Cada numero del int en un char

	char p_decimal_char[decimal_p];
	size = size_int(p_decimal);
	if (decimal_p>size){
		p_decimal = p_decimal + pow(10,decimal_p-1);
	}
	sprintf(p_decimal_char,"%d", p_decimal);
	for(uint8_t i=0; i<(decimal_p-size);i++){
		p_decimal_char[i] = '0';
	}

	uint8_t total_size = sizeof(p_entera_char)+decimal_p+3+negativo;
	uint8_t double_str[total_size];

	double_str[total_size-1] = '\r';
	double_str[total_size-2] = '\n';
	double_str[negativo+sizeof(p_entera_char)] = '.';

	for(int8_t i= (total_size-3); i>=0; i--){
		if (i > (negativo+sizeof(p_entera_char))) {
			double_str[i] = (uint8_t) p_decimal_char[i-(negativo+sizeof(p_entera_char)+1)];
		} else if (i < negativo+sizeof(p_entera_char)) {
			double_str[i] = (uint8_t) p_entera_char[i-negativo];
		}
	}

	if (negativo == 1) {
		double_str[0] = '-';
	}
	HAL_UART_Transmit(huart, double_str, sizeof(double_str), 10);
}

void tx_UART_byte(UART_HandleTypeDef *huart, uint8_t data, uint32_t Timeout){
	uint8_t byte_str[10];
	for(uint8_t i=0;i<8;i++){
		if((data>>(7-i) & 0x01)==1){
			byte_str[i]=0x31;
		}
		if((data>>(7-i) & 0x01)==0){
			byte_str[i]=0x30;
		}
	}
	byte_str[8] = '\r';
	byte_str[9] = '\n';
	HAL_UART_Transmit(huart, byte_str, sizeof(byte_str), 10);
}

uint8_t size_int(int data){
	uint8_t size;
	if (data==0){
		size = 1;
	}
	else {
		size = floor(log10(data))+1;
	}
	return size;
}
