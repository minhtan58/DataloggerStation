/*
 * timer.h
 *
 *  Created on: Oct 19, 2022
 *      Author: minht
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM5_Init(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* TIMER_H_ */
