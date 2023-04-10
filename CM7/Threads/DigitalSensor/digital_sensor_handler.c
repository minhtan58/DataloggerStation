/*
 * digital_sensor_handler.c
 *
 *  Created on: Jan 16, 2023
 *      Author: minht
 */

#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "cmsis_os.h"
#include "string.h"
#include "enums.h"
#include "structs.h"

/****************************************************************************/
/***    Local Function Prototypes                     ***/
/****************************************************************************/
#define TIMCLOCK   200000000
#define PRESCALAR  200

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;
extern tsDigitalSensor digitalSettingList[6];

volatile uint32_t ch1_val1, ch1_val2, ch2_val1, ch2_val2, ch3_val1, ch3_val2, ch4_val1, ch4_val2;
volatile uint32_t difference_ch1, difference_ch2, difference_ch3, difference_ch4;
volatile int flag_ch1, flag_ch2, flag_ch3, flag_ch4;
volatile float frequency_ch1, frequency_ch2, frequency_ch3, frequency_ch4;
volatile uint32_t counter_ch1, counter_ch2, counter_ch3, counter_ch4;
/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
//osMessageQId digitalQueueHandle;
osTimerId periodicDigitalTimer[4];
char dataDigitalUsed[128];
float dataDigital[4];

/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

void Digital_setup()
{
	for(int i = 0; i < 4; i++)
	{
		if(digitalSettingList[i].status[0] == 'E')
		{
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2 * i);
			osTimerStart(periodicDigitalTimer[i], atoi(digitalSettingList[i].interval)*1000);
		}
		else
		{
			HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_2 * i);
			osTimerStop(periodicDigitalTimer[i]);
		}
	}
}

char* Get_DigitalData(void)
{
    uint8_t length = 0;
    memset(dataDigitalUsed, 0, 128);

    for(int i = 0; i < 6; i++)
    {
        if(digitalSettingList[i].status[0] == 'E')
        {
            length += sprintf(dataDigitalUsed + length, "%s", dataDigital[i]);
        }
    }
    return dataDigitalUsed;
}

void Digital_Callback1(void const * argument)
{
	dataDigital[0] = counter_ch1;
	if(digitalSettingList[0].mode[0] == 'F'){
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
	}
	//osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_1, 0);
}

void Digital_Callback2(void const * argument)
{
	dataDigital[1] = counter_ch2;
	if(digitalSettingList[1].mode[0] == 'F'){
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	}
	//osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_2, 0);
}

void Digital_Callback3(void const * argument)
{
	dataDigital[2] = counter_ch3;
	if(digitalSettingList[2].mode[0] == 'F'){
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
	}
	//osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_3, 0);
}

void Digital_Callback4(void const * argument)
{
	dataDigital[3] = counter_ch4;
	if(digitalSettingList[3].mode[0] == 'F'){
		HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
	}
	//osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_4, 0);
}

void DigitalSensor_Task(void const * argument)
{
	//osEvent event;

	//osMessageQDef(myQueue01, 16, uint16_t);
	//digitalQueueHandle = osMessageCreate(osMessageQ(myQueue01), NULL);

	osTimerDef(periodicTimer1, Digital_Callback1);
	periodicDigitalTimer[0] = osTimerCreate(osTimer(periodicTimer1), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer2, Digital_Callback2);
	periodicDigitalTimer[1] = osTimerCreate(osTimer(periodicTimer2), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer3, Digital_Callback3);
	periodicDigitalTimer[2] = osTimerCreate(osTimer(periodicTimer3), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer4, Digital_Callback4);
	periodicDigitalTimer[3] = osTimerCreate(osTimer(periodicTimer4), osTimerPeriodic, NULL);

	while(1)
	{
		/*
		event = osMessageGet(digitalQueueHandle, 10);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case APP_E_DIGITAL_CHANNEL_1:
					dataDigital[0] = counter_ch1;
					break;
				case APP_E_DIGITAL_CHANNEL_2:
					dataDigital[1] = counter_ch2;
					break;
				case APP_E_DIGITAL_CHANNEL_3:
					dataDigital[2] = counter_ch3;
					break;
				case APP_E_DIGITAL_CHANNEL_4:
					dataDigital[3] = counter_ch4;
					break;
				case APP_E_DIGITAL_CHANNEL_5:
					break;
				case APP_E_DIGITAL_CHANNEL_6:
					break;
				default:
					break;
			}
		}
		*/
		osDelay(1);
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(digitalSettingList[0].mode[0] == 'F')
		{

			if (flag_ch1 == 0)
			{
				ch1_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
				flag_ch1 = 1;
			}
			else
			{
				ch1_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

				if (ch1_val2 > ch1_val1)
				{
					difference_ch1 = ch1_val2 - ch1_val1;
				}
				else if (ch1_val1 > ch1_val2)
				{
					difference_ch1 = (0xffffffff - ch1_val1) + ch1_val2;
				}

				frequency_ch1 = TIMCLOCK/(PRESCALAR)/difference_ch1;
				dataDigital[0] = frequency_ch1;
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
				flag_ch1 = 0;
			}
		}
		else
		{
			counter_ch1++;
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(digitalSettingList[1].mode[0] == 'F')
		{
			if (flag_ch2 == 0)
			{
				ch2_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				flag_ch2 = 1;
			}
			else
			{
				ch2_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

				if (ch2_val2 > ch2_val1)
				{
					difference_ch2 = ch2_val2 - ch2_val1;
				}
				else if (ch2_val1 > ch2_val2)
				{
					difference_ch2 = (0xffffffff - ch2_val1) + ch2_val2;
				}

				frequency_ch2 = TIMCLOCK/(PRESCALAR)/difference_ch2;
				dataDigital[1] = frequency_ch2;
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_2);
				flag_ch2 = 0;
			}
		}
		else
		{
			counter_ch2++;
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(digitalSettingList[2].mode[0] == 'F')
		{
			if (flag_ch3 == 0)
			{
				ch3_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
				flag_ch3 = 1;
			}
			else
			{
				ch3_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);

				if (ch3_val2 > ch3_val1)
				{
					difference_ch3 = ch3_val2 - ch3_val1;
				}
				else if (ch3_val1 > ch3_val2)
				{
					difference_ch3 = (0xffffffff - ch3_val1) + ch3_val2;
				}

				frequency_ch3 = TIMCLOCK/(PRESCALAR)/difference_ch3;
				dataDigital[2] = frequency_ch3;
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_3);
				flag_ch3 = 0;
			}
		}
		else
		{
			counter_ch3++;
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(digitalSettingList[2].mode[3] == 'F')
		{
			if (flag_ch4 == 0)
			{
				ch4_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
				flag_ch4 = 1;
			}
			else
			{
				ch4_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

				if (ch4_val2 > ch4_val1)
				{
					difference_ch4 = ch4_val2 - ch4_val1;
				}
				else if (ch4_val1 > ch4_val2)
				{
					difference_ch4 = (0xffffffff - ch4_val1) + ch4_val2;
				}

				frequency_ch4 = TIMCLOCK/(PRESCALAR)/difference_ch4;
				dataDigital[3] = frequency_ch4;
				HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_4);
				flag_ch4 = 0;
			}
		}
		else
		{
			counter_ch4++;
		}
	}
}
