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
#include "sensor_process.h"
#include "sdcard.h"
/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/
RTC_TimeTypeDef sTimeSys;
RTC_DateTypeDef sDateSys;

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim2;
extern uint8_t data_channel_1[50];
extern tsSetting setting;
extern tsOverview overview;
extern tsTransmission transmission;

osMessageQId NetworkQueueHandle;
char data_send[254];
volatile time_t sys_time;
extern u8_t recev_config[255];
void PT_NetworkCallback(void const * argument);
osTimerId periodicNetworkTimer;

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
	tcp_echoserver_init();
	Config_Ftp();
	HAL_TIM_Base_Start_IT(&htim2);

    header_count = 0;
    header_interval = 0;
    uint8_t u8lenght = 0;

	osTimerDef(periodicTimer, PT_NetworkCallback);
	periodicNetworkTimer = osTimerCreate(osTimer(periodicTimer), osTimerPeriodic, NULL);
    //osTimerStart(periodicNetworkTimer, atoi(transmission.interval)*1000);
	osTimerStart(periodicNetworkTimer,11000);

	while(1)
	{
		event = osMessageGet(NetworkQueueHandle, 10);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case APP_E_NETWORK_SEND_DATA:
				{
					HAL_RTC_GetTime(&hrtc,&sTimeSys,RTC_FORMAT_BIN);
					HAL_RTC_GetDate(&hrtc,&sDateSys,RTC_FORMAT_BIN);
					if (header_interval != 0)
					{
						if (header_count == header_interval)
						{
							u8lenght = sprintf(data_send,"%s;Date;Time;%s%s%s\r\n", (char*)overview.modelName, (char*)setting.stationName,\
							                                     (char*)Get_AnalogName(), (char*)Get_SerialName());
							//tcp_client_send_data(data_send);
							tcp_echoclient_connect(192,168,2,105,9);
							App_Ftp(data_send);
							header_count = 0;
							// todo: write to memory
							HME_WriteFile(fileData, &data_send, u8lenght);
						}
						header_count += 1;
					}


					u8lenght = sprintf(data_send,"%s;%d/%d/%d;%02d:%02d:%02d;%s%s%s\r\n", (char*)overview.modelName, sDateSys.Date, sDateSys.Month, sDateSys.Year,\
							                                                   sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds, (char*)setting.stationName,\
																			   (char*)Get_AnalogData(), (char*)Get_SerialData());
					//printf("%s\n\r", data_send);
					sprintf(data,"%s",data_send);
					get_time = 0;
					tcp_echoclient_connect(192,168,2,105,9);
					App_Ftp(data_send);
					// todo: write to memory
					HME_WriteFile(fileData, &data_send, u8lenght);
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

				case APP_E_NETWORK_CLIENT_REV_DATA:
				{
					printf("Data : %s \n\r",recev_buf);
					int i=0;
					char *control[15];
					char *token = strtok(recev_buf, " ");
					while(token != NULL) {
						control[i] = token;
						i++;
						token = strtok(NULL, " ");
					}
					//60030 23-03-27 19:26:18 50 0 0 854.1 UTC(NIST) *
					if(control[0][1] == '6')
					{
						struct tm tm;
						sscanf(control[1], "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
						sscanf(control[2], "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

						tm.tm_hour = tm.tm_hour + 7;
						if(tm.tm_hour >= 24){
							tm.tm_hour = tm.tm_hour - 24;
							tm.tm_mday = tm.tm_mday + 1;
						}
						sTimeSys.Hours = tm.tm_hour;
						sTimeSys.Minutes = tm.tm_min;
						sTimeSys.Seconds = tm.tm_sec;
						HAL_RTC_SetTime(&hrtc, &sTimeSys, RTC_FORMAT_BIN);

						sDateSys.Month = tm.tm_mon;
						sDateSys.Date = tm.tm_mday;
						sDateSys.Year = tm.tm_year;
						HAL_RTC_SetDate(&hrtc, &sDateSys, RTC_FORMAT_BIN);
					}
				}

				case APP_E_NETWORK_SERVER_REV_DATA:
				{
					printf("Datan\r"/*recev_config*/);
				}
				break;
				default:
					break;
			}
		}
		osDelay(1);
	}
}

void PT_NetworkCallback(void const * argument)
{
	osMessagePut(NetworkQueueHandle, APP_E_NETWORK_SEND_DATA, 0);
}
