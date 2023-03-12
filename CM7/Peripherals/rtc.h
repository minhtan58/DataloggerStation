/*
 * rtc.h
 *
 *  Created on: Oct 16, 2022
 *      Author: minht
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32h7xx_hal.h"

void MX_RTC_Init(void);
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc);
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc);

#endif /* RTC_H_ */
