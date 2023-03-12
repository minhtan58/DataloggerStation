/*
 * i2c.h
 *
 *  Created on: Aug 2, 2021
 *  Author: Admin
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32h7xx_hal.h"
#include "main.h"

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c4;

void MX_I2C1_Init(void);
void MX_I2C4_Init(void);

#endif /* I2C_H_ */
