/*
 * sensor_process.h
 *
 *  Created on: Oct 28, 2022
 *      Author: minht
 */

#ifndef SERIALPORTCOM_SENSOR_PROCESS_H_
#define SERIALPORTCOM_SENSOR_PROCESS_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

char* Get_SerialData(void);
void vProcessSerialChannel(uint8_t u8Channel);
void Serial_Setup_Timer(void);
void SerialSensor_Task(void const * argument);

osTimerId periodicSerialTimer[6];

#endif /* SERIALPORTCOM_SENSOR_PROCESS_H_ */
