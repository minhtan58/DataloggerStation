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

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
int Is_First_Captured = 0;
float frequency = 0;
/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
osMessageQId digitalQueueHandle;

/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

void DigitalSensor_Task(void const * argument)
{
	osEvent event;

	osMessageQDef(myQueue01, 16, uint16_t);
	digitalQueueHandle = osMessageCreate(osMessageQ(myQueue01), NULL);

	while(1)
	{
		event = osMessageGet(digitalQueueHandle, 10);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case APP_E_DIGITAL_CHANNEL_1:
					//Lấy giá trị frequency CH1
					break;
				case APP_E_DIGITAL_CHANNEL_2:
					//Lấy giá trị frequency CH1
					break;
				case APP_E_DIGITAL_CHANNEL_3:
					//Lấy giá trị frequency CH1
					break;
				case APP_E_DIGITAL_CHANNEL_4:
					//Lấy giá trị frequency CH1
					break;
				case APP_E_DIGITAL_CHANNEL_5:
					// Dọc giá trị đêm dc của ngăt PI4
					break;
				case APP_E_DIGITAL_CHANNEL_6:
					// Dọc giá trị đêm dc của ngăt PI5
					break;
				case APP_E_DIGITAL_READ_FREQUENCY:
					// Dọc giá trị sersor Frequency
					HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
					HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
					HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
					HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
					break;
				default:
					break;
			}
		}
		osDelay(1);
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
//	printf("Call back\n\r");
	HAL_TIM_Base_Start_IT(&htim5);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if (Is_First_Captured==0) // if the first rising edge is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
		}

		else   // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffffffff - IC_Val1) + IC_Val2;
			}

			float refClock = TIMCLOCK/(PRESCALAR);

			frequency = refClock/Difference;
			osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_1, 0);

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false
		}
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if (Is_First_Captured==0) // if the first rising edge is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
		}

		else   // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffffffff - IC_Val1) + IC_Val2;
			}

			float refClock = TIMCLOCK/(PRESCALAR);

			frequency = refClock/Difference;
			osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_2, 0);

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false
		}
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if (Is_First_Captured==0) // if the first rising edge is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
		}

		else   // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffffffff - IC_Val1) + IC_Val2;
			}

			float refClock = TIMCLOCK/(PRESCALAR);

			frequency = refClock/Difference;
			osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_3, 0);

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false
		}
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if (Is_First_Captured==0) // if the first rising edge is not captured
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
		}

		else   // If the first rising edge is captured, now we will capture the second edge
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value

			if (IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2-IC_Val1;
			}

			else if (IC_Val1 > IC_Val2)
			{
				Difference = (0xffffffff - IC_Val1) + IC_Val2;
			}

			float refClock = TIMCLOCK/(PRESCALAR);

			frequency = refClock/Difference;
			osMessagePut(digitalQueueHandle, APP_E_DIGITAL_CHANNEL_4, 0);

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false
		}
	}
}
