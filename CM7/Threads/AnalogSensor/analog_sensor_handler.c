/*
 * analog_sensor_handler.c
 *
 *  Created on: Nov 17, 2022
 *      Author: minht
 */
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analog_sensor_handler.h"
#include "ADS124S08.h"
#include "cmsis_os.h"
#include "enums.h"
#include "structs.h"
#include "command_process.h"

/****************************************************************************/
/***    Local Function Prototypes                     ***/
/****************************************************************************/
void PTCallback1(void const * argument);
void PTCallback2(void const * argument);
void PTCallback3(void const * argument);
void PTCallback4(void const * argument);
void PTCallback5(void const * argument);
void PTCallback6(void const * argument);
void PTCallback7(void const * argument);
void PTCallback8(void const * argument);
void PTCallback9(void const * argument);
void PTCallback10(void const * argument);
void PTCallback11(void const * argument);
void PTCallback12(void const * argument);

/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
osMessageQId analogQueueHandle;
osTimerId periodicTimerHandle[12];
extern tsAnalogSensor analogSettingList[12];
extern volatile float fAnalogDataList[12];

char dataAnalogUsed[50];
/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

void Analog_setup()
{
	for(int i = 0; i < 12; i++)
	{
		if(analogSettingList[i].status[0] == 'E')
			osTimerStart(periodicTimerHandle[i], atoi(analogSettingList[i].interval)*1000);
		else
			osTimerStop(periodicTimerHandle[i]);
	}
}

char* Get_AnalogData(void)
{
	uint8_t length = 0;
	memset(dataAnalogUsed, 0, 50);

	for(int i = 0; i < 12; i++)
	{
		if(strcmp("Enabled", analogSettingList[i].status) == 0)
		{
			length += sprintf(dataAnalogUsed+length,";%.2f", fAnalogDataList[i]);
		}
	}
	return dataAnalogUsed;
}

void AnalogSensor_Task(void const * argument)
{
	float fdata = 0;
	osEvent event;
	InitDevice_ADC();

	osMessageQDef(myQueue03, 16, uint16_t);
	analogQueueHandle = osMessageCreate(osMessageQ(myQueue03), NULL);

	osTimerDef(periodicTimer1, PTCallback1);
	periodicTimerHandle[0] = osTimerCreate(osTimer(periodicTimer1), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer2, PTCallback2);
	periodicTimerHandle[1] = osTimerCreate(osTimer(periodicTimer2), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer3, PTCallback3);
	periodicTimerHandle[2] = osTimerCreate(osTimer(periodicTimer3), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer4, PTCallback4);
	periodicTimerHandle[3] = osTimerCreate(osTimer(periodicTimer4), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer5, PTCallback5);
	periodicTimerHandle[4] = osTimerCreate(osTimer(periodicTimer5), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer6, PTCallback6);
	periodicTimerHandle[5] = osTimerCreate(osTimer(periodicTimer6), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer7, PTCallback7);
	periodicTimerHandle[6] = osTimerCreate(osTimer(periodicTimer7), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer8, PTCallback8);
	periodicTimerHandle[7] = osTimerCreate(osTimer(periodicTimer8), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer9, PTCallback9);
	periodicTimerHandle[8] = osTimerCreate(osTimer(periodicTimer9), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer10, PTCallback10);
	periodicTimerHandle[9] = osTimerCreate(osTimer(periodicTimer10), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer11, PTCallback11);
	periodicTimerHandle[10] = osTimerCreate(osTimer(periodicTimer11), osTimerPeriodic, NULL);

	osTimerDef(periodicTimer12, PTCallback12);
	periodicTimerHandle[11] = osTimerCreate(osTimer(periodicTimer12), osTimerPeriodic, NULL);

	Analog_setup();

	while(1)
	{
		event = osMessageGet(analogQueueHandle, 10);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case APP_E_ANALOG_CHANNEL_1:
					//printf("APP_E_ANALOG_CHANNEL_1\n\r");
					fdata = Getdata_ADC(CHANNEL_0_EN);
					/* Formula: Result = Vin*Slope + Offset */
					fAnalogDataList[0] = (fdata*atoi(analogSettingList[0].slope)) + atoi(analogSettingList[0].offset);

					break;
				case APP_E_ANALOG_CHANNEL_2:
					//printf("APP_E_ANALOG_CHANNEL_2\n\r");
					fdata = Getdata_ADC(CHANNEL_1_EN);
					fAnalogDataList[1] = (fdata*atoi(analogSettingList[1].slope))+ atoi(analogSettingList[1].offset);

					break;
				case APP_E_ANALOG_CHANNEL_3:
					//printf("APP_E_ANALOG_CHANNEL_3\n\r");
					fdata = Getdata_ADC(CHANNEL_2_EN);
					fAnalogDataList[2] = (fdata*atoi(analogSettingList[2].slope))+ atoi(analogSettingList[2].offset);
					break;
				case APP_E_ANALOG_CHANNEL_4:
					//printf("APP_E_ANALOG_CHANNEL_4\n\r");
					fdata = Getdata_ADC(CHANNEL_3_EN);
					fAnalogDataList[3] = (fdata*atoi(analogSettingList[3].slope))+ atoi(analogSettingList[3].offset);
					break;
				case APP_E_ANALOG_CHANNEL_5:
					//printf("APP_E_ANALOG_CHANNEL_5\n\r");
					fdata = Getdata_ADC(CHANNEL_4_EN);
					fAnalogDataList[4] = (fdata*atoi(analogSettingList[4].slope))+ atoi(analogSettingList[4].offset);
					break;
				case APP_E_ANALOG_CHANNEL_6:
					//printf("APP_E_ANALOG_CHANNEL_6\n\r");
					fdata = Getdata_ADC(CHANNEL_5_EN);
					fAnalogDataList[5] = (fdata*atoi(analogSettingList[5].slope))+ atoi(analogSettingList[5].offset);
					break;
				case APP_E_ANALOG_CHANNEL_7:
					//printf("APP_E_ANALOG_CHANNEL_7\n\r");
					fdata = Getdata_ADC(CHANNEL_6_EN);
					fAnalogDataList[6] = (fdata*atoi(analogSettingList[6].slope))+ atoi(analogSettingList[6].offset);
					break;
				case APP_E_ANALOG_CHANNEL_8:
					//printf("APP_E_ANALOG_CHANNEL_8\n\r");
					fdata = Getdata_ADC(CHANNEL_7_EN);
					fAnalogDataList[7] = (fdata*atoi(analogSettingList[7].slope))+ atoi(analogSettingList[7].offset);
					break;
				case APP_E_ANALOG_CHANNEL_9:
					//printf("APP_E_ANALOG_CHANNEL_9\n\r");
					fdata = Getdata_ADC(CHANNEL_8_EN);
					fAnalogDataList[8] = (fdata*atoi(analogSettingList[8].slope))+ atoi(analogSettingList[8].offset);
					break;
				case APP_E_ANALOG_CHANNEL_10:
					//printf("APP_E_ANALOG_CHANNEL_10\n\r");
					fdata = Getdata_ADC(CHANNEL_9_EN);
					fAnalogDataList[9] = (fdata*atoi(analogSettingList[9].slope))+ atoi(analogSettingList[9].offset);
					break;
				case APP_E_ANALOG_CHANNEL_11:
					//printf("APP_E_ANALOG_CHANNEL_11\n\r");
					fdata = Getdata_ADC(CHANNEL_10_EN);
					fAnalogDataList[10] = (fdata*atoi(analogSettingList[10].slope))+ atoi(analogSettingList[10].offset);
					break;
				case APP_E_ANALOG_CHANNEL_12:
					//printf("APP_E_ANALOG_CHANNEL_12\n\r");
					fdata = Getdata_ADC(CHANNEL_11_EN);
					fAnalogDataList[11] = (fdata*atoi(analogSettingList[11].slope))+ atoi(analogSettingList[11].offset);
					//printf("data = %lu\n\r", (unsigned long)data);
					break;
				default:
					break;
			}
		}
		osDelay(1);
	}
}

/* PTCallback function */
void PTCallback1(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_1, 0);
}

void PTCallback2(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_2, 0);
}

void PTCallback3(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_3, 0);
}

void PTCallback4(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_4, 0);
}

void PTCallback5(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_5, 0);
}

void PTCallback6(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_6, 0);
}

void PTCallback7(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_7, 0);
}

void PTCallback8(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_8, 0);
}

void PTCallback9(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_9, 0);
}

void PTCallback10(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_10, 0);
}

void PTCallback11(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_11, 0);
}

void PTCallback12(void const * argument)
{
	osMessagePut(analogQueueHandle, APP_E_ANALOG_CHANNEL_12, 0);
}
