/**
  ******************************************************************************
  * @file    ads155.h
  * @author  Javier Morales
  * @brief   Header file of timing library. It's purpose is to
  * 		 measure execution time in us, ms and seconds
  *
  * https://github.com/HappyDonuts
  ******************************************************************************
  */

//#include "stm32f3xx_hal.h"
#include "stm32f1xx_hal.h"
//#include "stm32f3xx_hal.h"
#include <stdlib.h>
#include "math.h"

typedef struct timing_t {
	TIM_HandleTypeDef *htim;
	uint32_t f_clk;
	uint16_t PS_us;
	uint16_t PS_ms;
	uint16_t PS_s;
} timing_t;

timing_t* timing_new(TIM_HandleTypeDef *htim);

void timing_init(timing_t *timing, TIM_HandleTypeDef *htim);

volatile void timing_start_us(timing_t *timing);

volatile double timing_get_us(timing_t *timing);

volatile double timing_self_timer(timing_t *timing);

volatile void timing_start_ms(timing_t *timing);

volatile double timing_get_ms(timing_t *timing);

volatile void timing_start_s(timing_t *timing);

volatile double timing_get_s(timing_t *timing);
