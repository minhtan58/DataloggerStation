/*
 * virtual_sensor_handle.c
 *
 *  Created on: Mar 15, 2023
 *      Author: HuynhLD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enums.h"
#include "structs.h"
#include "command_process.h"
#include "virtual_sensor_handle.h"

osMessageQId virtualQueueHandle;
extern tsVirtualSensor virtualSensorList[15];

void virtualPTCallback(void const * argument); /* prototypes for periodic timer call-back */
void virtualOSCallback(void const * argument); /* prototypes for one-shoot timer call-back */

uint8_t timer_arg[15] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14}; /* list argument for the timer call-back function */

void Virtual_setup(void)
{
    uint8_t u8index;
    for (u8index = 0; u8index < 15; u8index++)
    {
    	if (virtualSensorList[u8index].status[0] == 'E')
    		osTimerStart(oneshootVirtualTimer[u8index], atoi(virtualSensorList[u8index].interval)*1000);
    }
}

void VirtualSensor_Task(void const * argument)
{
    osEvent event;

    osMessageQDef(virtualQueue, 16, uint16_t);
    virtualQueueHandle = osMessageCreate(osMessageQ(virtualQueue), NULL);

    osTimerDef(osVirtualTimer1, virtualOSCallback);
    osTimerDef(osVirtualTimer2, virtualOSCallback);
    osTimerDef(osVirtualTimer3, virtualOSCallback);
    osTimerDef(osVirtualTimer4, virtualOSCallback);
    osTimerDef(osVirtualTimer5, virtualOSCallback);
    osTimerDef(osVirtualTimer6, virtualOSCallback);
    osTimerDef(osVirtualTimer7, virtualOSCallback);
    osTimerDef(osVirtualTimer8, virtualOSCallback);
    osTimerDef(osVirtualTimer9, virtualOSCallback);
    osTimerDef(osVirtualTimer10, virtualOSCallback);
    osTimerDef(osVirtualTimer11, virtualOSCallback);
    osTimerDef(osVirtualTimer12, virtualOSCallback);
    osTimerDef(osVirtualTimer13, virtualOSCallback);
    osTimerDef(osVirtualTimer14, virtualOSCallback);
    osTimerDef(osVirtualTimer15, virtualOSCallback);

    osTimerDef(ptVirtualTimer1, virtualPTCallback);
    osTimerDef(ptVirtualTimer2, virtualPTCallback);
    osTimerDef(ptVirtualTimer3, virtualPTCallback);
    osTimerDef(ptVirtualTimer4, virtualPTCallback);
    osTimerDef(ptVirtualTimer5, virtualPTCallback);
    osTimerDef(ptVirtualTimer6, virtualPTCallback);
    osTimerDef(ptVirtualTimer7, virtualPTCallback);
    osTimerDef(ptVirtualTimer8, virtualPTCallback);
    osTimerDef(ptVirtualTimer9, virtualPTCallback);
    osTimerDef(ptVirtualTimer10, virtualPTCallback);
    osTimerDef(ptVirtualTimer11, virtualPTCallback);
    osTimerDef(ptVirtualTimer12, virtualPTCallback);
    osTimerDef(ptVirtualTimer13, virtualPTCallback);
    osTimerDef(ptVirtualTimer14, virtualPTCallback);
    osTimerDef(ptVirtualTimer15, virtualPTCallback);

    oneshootVirtualTimer[0] = osTimerCreate(osTimer(osVirtualTimer1), osTimerOnce, &timer_arg[0]);
    oneshootVirtualTimer[1] = osTimerCreate(osTimer(osVirtualTimer2), osTimerOnce, &timer_arg[1]);
    oneshootVirtualTimer[2] = osTimerCreate(osTimer(osVirtualTimer3), osTimerOnce, &timer_arg[2]);
    oneshootVirtualTimer[3] = osTimerCreate(osTimer(osVirtualTimer4), osTimerOnce, &timer_arg[3]);
    oneshootVirtualTimer[4] = osTimerCreate(osTimer(osVirtualTimer5), osTimerOnce, &timer_arg[4]);
    oneshootVirtualTimer[5] = osTimerCreate(osTimer(osVirtualTimer6), osTimerOnce, &timer_arg[5]);
    oneshootVirtualTimer[6] = osTimerCreate(osTimer(osVirtualTimer7), osTimerOnce, &timer_arg[6]);
    oneshootVirtualTimer[7] = osTimerCreate(osTimer(osVirtualTimer8), osTimerOnce, &timer_arg[7]);
    oneshootVirtualTimer[8] = osTimerCreate(osTimer(osVirtualTimer9), osTimerOnce, &timer_arg[8]);
    oneshootVirtualTimer[9] = osTimerCreate(osTimer(osVirtualTimer10), osTimerOnce, &timer_arg[9]);
    oneshootVirtualTimer[10] = osTimerCreate(osTimer(osVirtualTimer11), osTimerOnce, &timer_arg[10]);
    oneshootVirtualTimer[11] = osTimerCreate(osTimer(osVirtualTimer12), osTimerOnce, &timer_arg[11]);
    oneshootVirtualTimer[12] = osTimerCreate(osTimer(osVirtualTimer13), osTimerOnce, &timer_arg[12]);
    oneshootVirtualTimer[13] = osTimerCreate(osTimer(osVirtualTimer14), osTimerOnce, &timer_arg[13]);
    oneshootVirtualTimer[14] = osTimerCreate(osTimer(osVirtualTimer15), osTimerOnce, &timer_arg[14]);

    periodicVirtualTimer[0] = osTimerCreate(osTimer(ptVirtualTimer1), osTimerPeriodic, &timer_arg[0]);
    periodicVirtualTimer[1] = osTimerCreate(osTimer(ptVirtualTimer2), osTimerPeriodic, &timer_arg[1]);
    periodicVirtualTimer[2] = osTimerCreate(osTimer(ptVirtualTimer3), osTimerPeriodic, &timer_arg[2]);
    periodicVirtualTimer[3] = osTimerCreate(osTimer(ptVirtualTimer4), osTimerPeriodic, &timer_arg[3]);
    periodicVirtualTimer[4] = osTimerCreate(osTimer(ptVirtualTimer5), osTimerPeriodic, &timer_arg[4]);
    periodicVirtualTimer[5] = osTimerCreate(osTimer(ptVirtualTimer6), osTimerPeriodic, &timer_arg[5]);
    periodicVirtualTimer[6] = osTimerCreate(osTimer(ptVirtualTimer7), osTimerPeriodic, &timer_arg[6]);
    periodicVirtualTimer[7] = osTimerCreate(osTimer(ptVirtualTimer8), osTimerPeriodic, &timer_arg[7]);
    periodicVirtualTimer[8] = osTimerCreate(osTimer(ptVirtualTimer9), osTimerPeriodic, &timer_arg[8]);
    periodicVirtualTimer[9] = osTimerCreate(osTimer(ptVirtualTimer10), osTimerPeriodic, &timer_arg[9]);
    periodicVirtualTimer[10] = osTimerCreate(osTimer(ptVirtualTimer11), osTimerPeriodic, &timer_arg[10]);
    periodicVirtualTimer[11] = osTimerCreate(osTimer(ptVirtualTimer12), osTimerPeriodic, &timer_arg[11]);
    periodicVirtualTimer[12] = osTimerCreate(osTimer(ptVirtualTimer13), osTimerPeriodic, &timer_arg[12]);
    periodicVirtualTimer[13] = osTimerCreate(osTimer(ptVirtualTimer14), osTimerPeriodic, &timer_arg[13]);
    periodicVirtualTimer[14] = osTimerCreate(osTimer(ptVirtualTimer15), osTimerPeriodic, &timer_arg[14]);

    while(1)
    {
    	event = osMessageGet(virtualQueueHandle, 10);
    	if (event.status == osEventMessage)
    	{
    		switch (event.value.v) {
				case E_VIRTUAL_CHANNEL_1:
					// 1. determine type of virtual sensor
					// 2. determine physic sensor that used by as input data for virtual sensor
					// 3. get value then save
					break;
				case E_VIRTUAL_CHANNEL_2:
					break;
				case E_VIRTUAL_CHANNEL_3:
					break;
				case E_VIRTUAL_CHANNEL_4:
					break;
				case E_VIRTUAL_CHANNEL_5:
					break;
				case E_VIRTUAL_CHANNEL_6:
					break;
				case E_VIRTUAL_CHANNEL_7:
					break;
				case E_VIRTUAL_CHANNEL_8:
					break;
				case E_VIRTUAL_CHANNEL_9:
					break;
				case E_VIRTUAL_CHANNEL_10:
					break;
				case E_VIRTUAL_CHANNEL_11:
					break;
				case E_VIRTUAL_CHANNEL_12:
					break;
				case E_VIRTUAL_CHANNEL_13:
					break;
				case E_VIRTUAL_CHANNEL_14:
					break;

				default:
					break;
			}
    	}
    	osDelay(1);
    }
}


void virtualPTCallback(void const * argument)
{
	uint8_t *data = (uint8_t*)pvTimerGetTimerID((TimerHandle_t)argument);

	// 1. send event to update data
	osMessagePut(virtualQueueHandle, *data, 10);
}

void virtualOSCallback(void const * argument)
{
	// 1. stop oneshoot timer
	// 2. start periodic timer
	uint8_t *data = (uint8_t*)pvTimerGetTimerID((TimerHandle_t)argument);

	osTimerStop(oneshootVirtualTimer[*data]);
	osTimerStart(periodicVirtualTimer[*data], atoi(virtualSensorList[*data].interval)*1000);
}
