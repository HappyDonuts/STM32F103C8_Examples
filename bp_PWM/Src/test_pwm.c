/*
 * test_pwm.c
 *
 *  Created on: Mar 18, 2020
 *      Author: Javi
 */

/* INCLUDES -------------------------------- */
#include "test_pwm.h"
/* PERIPHERALS ----------------------------- */
TIM_HandleTypeDef htim1;

TIM_HandleTypeDef* tim_pwm = &htim1; // A8
/* DEFINES  -------------------------------- */

/* TYPEDEF --------------------------------- */

/* VARIABLES ------------------------------- */

/* FUNCTION PROTOTYPES --------------------- */


/**
  * @brief  The application entry point.
  * @retval none
  */
void main_s(void){
	/* MAIN CODE */

	/* Set period */
	uint16_t period = 1599;
	__HAL_TIM_SET_AUTORELOAD(tim_pwm, period-1);
	/* Set prescaler */
	uint16_t ps = 2;
	__HAL_TIM_SET_PRESCALER(tim_pwm, ps-1);
	/* Set duty cycle */
	float duy_cycle = 0.2;
	__HAL_TIM_SET_COMPARE(tim_pwm, TIM_CHANNEL_1, period*duy_cycle);
	/* Start PWM */
	HAL_TIM_PWM_Start(tim_pwm, TIM_CHANNEL_1);
	/* Stop PWM */
//	HAL_TIM_PWM_Stop(tim_pwm, TIM_CHANNEL_1);
	
	while(1){
		/* WHILE CODE */
		
	}
}

/**
  * @brief  Period elapsed callback.
  * @retval none
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	/* TIM CALLBACK CODE */
	
}

/**
  * @brief  EXTI line detection callbacks.
  * @retval none
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* GPIO CALLBACK CODE */
	
}
