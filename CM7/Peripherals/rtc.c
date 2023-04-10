/*
 * rtc.c
 *
 *  Created on: Oct 16, 2022
 *      Author: minht
 */
#include <stdio.h>
#include "app_common.h"
#include "rtc.h"
#include "ftplib.h"

RTC_HandleTypeDef hrtc;
extern iupd_t ftp_set;
extern tsSetting setting;
char ftpFilename[50];
extern RTC_DateTypeDef sDateSys;

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	//printf("Callback for set file name\r\n");
	//strcpy(&filePathConfig[0], "/public_html/DATA/");
	snprintf(ftpFilename, 50,"%s%d%d%d.txt", setting.stationName, sDateSys.Year, sDateSys.Month, sDateSys.Date);
	//snprintf(filePathConfig, 50,"/%s/%d%/%d", sDateSys.Year, sDateSys.Month, sDateSys.Date);
	CreateNewDir(sDateSys.Year, sDateSys.Month, sDateSys.Date);
	strcpy((char*)ftp_set.file_name, ftpFilename);
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	//printf("Bat dau gui du lieu\r\n");
	//osTimerStart(periodicNetworkTimer, atoi(transmission.interval)*1000);
}

void MX_RTC_Init(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	RTC_AlarmTypeDef sAlarm = {0};

	/** Initialize RTC Only
	*/
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 124;
	hrtc.Init.SynchPrediv = 7999;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initialize RTC and set the Time and Date
	*/
	  sTime.Hours = 10;
	  sTime.Minutes = 0;
	  sTime.Seconds = 0;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	  sDate.Month = RTC_MONTH_MARCH;
	  sDate.Date = 0x21;
	  sDate.Year = 0x23;

	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	/** Enable the Alarm A
	*/
	sAlarm.AlarmTime.Hours = 10;
	sAlarm.AlarmTime.Minutes = 5;
	sAlarm.AlarmTime.Seconds = 0;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 20;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	/** Enable the Alarm B
	*/
	sAlarm.AlarmTime.Hours = 0x1;
	sAlarm.AlarmTime.Minutes = 0x2;
	sAlarm.AlarmTime.Seconds = 0x3;
	sAlarm.Alarm = RTC_ALARM_B;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(hrtc->Instance==RTC)
	{
		/** Initializes the peripherals clock
		*/
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV25;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
		  Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_RTC_ENABLE();

		/* RTC interrupt Init */
		HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	}
}

/**
* @brief RTC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
	if(hrtc->Instance==RTC)
	{
		/* Peripheral clock disable */
		__HAL_RCC_RTC_DISABLE();
		HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
	}
}

