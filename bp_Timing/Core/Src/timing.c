/**
  ******************************************************************************
  * @file    ads155.c
  * @author  Javier Morales
  * @brief   C file of timing library functions.
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

#include "timing.h"

/**
 * @brief  Creates new timing_t variable corresponding to a timing module
 * @param  *hi2c: I2C peripheral from the mcu
 * @param  addr: I2C address used by the module
 * @retval ads_t variable corresponding to the ads module
 */
timing_t* timing_new(TIM_HandleTypeDef *htim){
	timing_t* timing = malloc(sizeof(*timing));
	timing_init(timing, htim);
	return timing;
}

void timing_init(timing_t *timing, TIM_HandleTypeDef *htim){
	__HAL_TIM_SET_AUTORELOAD(htim, 65535);
	timing->htim = htim;
	timing->f_clk = HAL_RCC_GetSysClockFreq();
	timing->PS_us = round((double)timing->f_clk/(65535*1000));
	if (timing->PS_us < 1) {
		timing->PS_us = 0;
	}
	timing->PS_ms = round((double)timing->f_clk/(65535));
	if (timing->PS_ms < 0) {
		timing->PS_ms = 0;
	}
	timing->PS_s = 0xFFFF;
}

volatile void timing_start_us(timing_t *timing){
	HAL_TIM_Base_Init(timing->htim);
	__HAL_TIM_SET_PRESCALER(timing->htim, timing->PS_us-1);
	__HAL_TIM_SET_COUNTER(timing->htim, 0);
	while (timing->htim->State != HAL_TIM_STATE_READY){
	}

	HAL_TIM_Base_Start(timing->htim);
}

volatile double timing_get_us(timing_t *timing){
	HAL_TIM_Base_Stop(timing->htim);

	double count = __HAL_TIM_GET_COUNTER(timing->htim);
	count = count*1000000*timing->PS_us/(double)timing->f_clk;
	HAL_TIM_Base_DeInit(timing->htim);

	double timer_d = timing_self_timer(timing);
	return count-timer_d;
}

volatile double timing_self_timer(timing_t *timing){
	HAL_TIM_Base_Init(timing->htim);
	__HAL_TIM_SET_PRESCALER(timing->htim, 0);
	__HAL_TIM_SET_COUNTER(timing->htim, 0);
	while (timing->htim->State != HAL_TIM_STATE_READY){
	}

	HAL_TIM_Base_Start(timing->htim);
	HAL_TIM_Base_Stop(timing->htim);
	double count = __HAL_TIM_GET_COUNTER(timing->htim);
	count = count*1000000/(double)timing->f_clk;
	HAL_TIM_Base_DeInit(timing->htim);
	return count;
}

volatile void timing_start_ms(timing_t *timing){
	__HAL_TIM_SET_PRESCALER(timing->htim, timing->PS_ms-1);
	__HAL_TIM_SET_COUNTER(timing->htim, 0);
	while (timing->htim->State != HAL_TIM_STATE_READY){
	}

	HAL_TIM_Base_Start(timing->htim);
}

volatile double timing_get_ms(timing_t *timing){
	HAL_TIM_Base_Stop(timing->htim);

	double count = __HAL_TIM_GET_COUNTER(timing->htim);
	count = count*1000*timing->PS_ms/(double)timing->f_clk;
	return count;
}

volatile void timing_start_s(timing_t *timing){
	__HAL_TIM_SET_PRESCALER(timing->htim, timing->PS_s-1);
	__HAL_TIM_SET_COUNTER(timing->htim, 0);
	while (timing->htim->State != HAL_TIM_STATE_READY){
	}

	HAL_TIM_Base_Start(timing->htim);
 }

volatile double timing_get_s(timing_t *timing){
	 HAL_TIM_Base_Stop(timing->htim);

	double count = __HAL_TIM_GET_COUNTER(timing->htim);
	count = count*timing->PS_s/(double)timing->f_clk;
	return count;
 }
