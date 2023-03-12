/*
 * network_handler.c
 *
 *  Created on: Oct 20, 2022
 *      Author: minht
 */
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "network_handler.h"
#include "cmsis_os.h"
#include "enums.h"
#include "stm32h7xx_hal.h"
#include "lwip.h"
#include "tcp_echoserver.h"
#include "ftplib.h"
#include "sntp.h"
#include "structs.h"
#include "analog_sensor_handler.h"
#include "tcp_echoclient.h"
/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
RTC_TimeTypeDef sTimeSys;
RTC_DateTypeDef sDateSys;

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim2;
extern uint8_t data_channel_1[50];
extern tsSetting setting;

osMessageQId NetworkQueueHandle;
char data_send[254];
volatile time_t sys_time;

/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

void Network_Task(void const *argument)
{
	osEvent event;
	osMessageQDef(myQueue02, 16, uint16_t);
	NetworkQueueHandle = osMessageCreate(osMessageQ(myQueue02), NULL);

	/* init code for LWIP */
	MX_LWIP_Init();
	osDelay(1000);
	tcp_echoserver_init();
	tcp_echoclient_connect();
	HAL_TIM_Base_Start_IT(&htim2);

    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

	while(1)
	{
		event = osMessageGet(NetworkQueueHandle, 10);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case APP_E_NETWORK_SEND_DATA:
				{
					char dateTime[32];

					HAL_RTC_GetTime(&hrtc,&sTimeSys,RTC_FORMAT_BIN);
					HAL_RTC_GetDate(&hrtc,&sDateSys,RTC_FORMAT_BIN);
					sprintf(dateTime,"%02d:%02d:%02d-%d/%d/%d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds, sDateSys.Date, sDateSys.Month, sDateSys.Year);
					sprintf(data_send,"%s;%s%s%s\r\n", (char*)dateTime, (char*)setting.stationName, (char*)Get_AnalogData(), (char*)Get_SerialData());
//					printf("%s\n\r", data_send);
					tcp_client_send_data("Data to TCP server");
					App_Ftp(data_send);
				}
				break;
				case APP_E_NETWORK_GET_TIME:
				{
			        struct tm *curDate;
			        sys_time += (7 * 60 * 60);
			        curDate = localtime(&sys_time);

					sTimeSys.Hours = curDate->tm_hour;
					sTimeSys.Minutes = curDate->tm_min;
					sTimeSys.Seconds = curDate->tm_sec;
					sTimeSys.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
					sTimeSys.StoreOperation = RTC_STOREOPERATION_RESET;
					HAL_RTC_SetTime(&hrtc, &sTimeSys, RTC_FORMAT_BIN);

					sDateSys.WeekDay = curDate->tm_wday;
					sDateSys.Month = (curDate->tm_mon) + 1;
					sDateSys.Date = curDate->tm_mday;
					sDateSys.Year = (curDate->tm_year) + 1900 - 2000;
					HAL_RTC_SetDate(&hrtc, &sDateSys, RTC_FORMAT_BIN);
				}
				break;
				default:
					break;
			}
		}
		osDelay(1);
	}
}

void lwip_sntp_recv_callback(uint32_t time)
{
    if (time == 0)
    {
//    	printf("NTP: Error, server not responding or bad response\r\n");
    }
    else
    {
    	sys_time = time;
    	osMessagePut(NetworkQueueHandle, APP_E_NETWORK_GET_TIME, 0);
    }
}
