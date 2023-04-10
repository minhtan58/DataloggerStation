/*
 * virtual_sensor_handle.h
 *
 *  Created on: Mar 15, 2023
 *      Author: HuynhLD
 */

#ifndef VIRTUALSENSOR_VIRTUAL_SENSOR_HANDLE_H_
#define VIRTUALSENSOR_VIRTUAL_SENSOR_HANDLE_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

typedef enum {
	E_VIRTUAL_CHANNEL_1 = 0,
	E_VIRTUAL_CHANNEL_2,
	E_VIRTUAL_CHANNEL_3,
	E_VIRTUAL_CHANNEL_4,
	E_VIRTUAL_CHANNEL_5,
	E_VIRTUAL_CHANNEL_6,
	E_VIRTUAL_CHANNEL_7,
	E_VIRTUAL_CHANNEL_8,
	E_VIRTUAL_CHANNEL_9,
	E_VIRTUAL_CHANNEL_10,
	E_VIRTUAL_CHANNEL_11,
	E_VIRTUAL_CHANNEL_12,
	E_VIRTUAL_CHANNEL_13,
	E_VIRTUAL_CHANNEL_14,
	E_VIRTUAL_CHANNEL_15,
} teVirtualChannelID;

void Virtual_setup(void);
void VirtualSensor_Task(void const * argument);

osTimerId periodicVirtualTimer[15]; /* for sensor Interval */
osTimerId oneshootVirtualTimer[15]; /* for sensor Start time */

#endif /* VIRTUALSENSOR_VIRTUAL_SENSOR_HANDLE_H_ */
