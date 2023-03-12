/*
 * uart_handler.c
 *
 *  Created on: Oct 12, 2022
 *      Author: minht
 */
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "cmsis_os.h"
#include "string.h"
#include "ftplib.h"
#include "uart_handler.h"
#include "structs.h"
#include "command_process.h"
#include "sensor_process.h"
#include "sdcard.h"

#define END_DATA_CONF       33 // !

extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

osMessageQId SerialQueueHandle;
tsCommand sCommand[8]; // Appconfig + 6UART + Sim4G

volatile uint8_t UART1_RxBuffer;
volatile uint8_t UART2_RxBuffer;
volatile uint8_t UART3_RxBuffer;
volatile uint8_t UART4_RxBuffer;
volatile uint8_t UART5_RxBuffer;
volatile uint8_t UART6_RxBuffer;
volatile uint8_t UART7_RxBuffer;
volatile uint8_t UART8_RxBuffer;

extern FILINFO fno;
extern FIL myfile;
extern FATFS FatFs;

void Setting_channel(uint8_t channel)
{
	switch(channel) {
		case 0: // COM1 == UART4
		{
			if(strcmp("Enabled", serialSettingList[0].status) == 0)
			{
				MX_UART4_Init();
				HAL_UART_Receive_IT(&huart4, (uint8_t*)&UART4_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(UART4_IRQn);
			}
		}
		break;
		case 1: // COM2 == USART6
		{
			if(strcmp("Enabled", serialSettingList[1].status) == 0)
			{
				MX_USART6_UART_Init();
				HAL_UART_Receive_IT(&huart6, (uint8_t*)&UART6_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(USART6_IRQn);
			}
		}
		break;
		case 2: // COM3 == UART5
		{
			if(strcmp("Enabled", serialSettingList[2].status) == 0)
			{
				MX_UART5_Init();
				HAL_UART_Receive_IT(&huart5, (uint8_t*)&UART5_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(UART5_IRQn);
			}
		}
		break;
		case 3: // COM4 == USART1
		{
			if(strcmp("Enabled", serialSettingList[3].status) == 0)
			{
				MX_USART1_UART_Init();
				HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(USART1_IRQn);
			}
		}
		break;
		case 4: // RS485 == USART2
		{
			if(strcmp("Enabled", serialSettingList[4].status) == 0)
			{
				MX_USART2_UART_Init();
				HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART6_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(USART2_IRQn);
			}
		}
		break;
		case 5: // SDI-12 == UART7
		{
			if(strcmp("Enabled", serialSettingList[5].status) == 0)
			{
				MX_UART7_Init();
				HAL_UART_Receive_IT(&huart7, (uint8_t*)&UART7_RxBuffer, 1);
			}else{
				HAL_NVIC_DisableIRQ(UART7_IRQn);
			}
		}
		break;
		default:
			break;
	}
}

void UART_Init(void)
{
	if(strcmp("Enabled", serialSettingList[0].status) == 0)
	{
		MX_UART4_Init();
		HAL_UART_Receive_IT(&huart4, (uint8_t*)&UART4_RxBuffer, 1);
	}
	if(strcmp("Enabled", serialSettingList[1].status) == 0)
	{
		MX_USART6_UART_Init();
		HAL_UART_Receive_IT(&huart6, (uint8_t*)&UART6_RxBuffer, 1);
	}
	if(strcmp("Enabled", serialSettingList[2].status) == 0)
	{
		MX_UART5_Init();
		HAL_UART_Receive_IT(&huart5, (uint8_t*)&UART5_RxBuffer, 1);
	}
	if(strcmp("Enabled", serialSettingList[3].status) == 0)
	{
		MX_USART1_UART_Init();
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	}
	if(strcmp("Enabled", serialSettingList[4].status) == 0)
	{
		MX_USART2_UART_Init();
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1);
	}
	if(strcmp("Enabled", serialSettingList[5].status) == 0)
	{
		MX_UART7_Init();
		HAL_UART_Receive_IT(&huart7, (uint8_t*)&UART7_RxBuffer, 1);
	}
}

void SerialPortCom_Task(void const * argument)
{
	osEvent event;

	osMessageQDef(myQueue01, 16, uint16_t);
	SerialQueueHandle = osMessageCreate(osMessageQ(myQueue01), NULL);

//	uint8_t rc;
//	f_mount(&FatFs,"",1);
//	rc = f_stat(fileConfig, &fno);
//	f_mount(&FatFs,"",0);
//	if (rc == FR_OK) {
//		// read data from mmc
//		printf("Ton tai\r\n");
//		HME_ReadMemory(fileConfig, &overview, sizeof(tsOverview), BASE_ADD_OVERVIEW);
//		HME_ReadMemory(fileConfig, &setting, sizeof(tsSetting), BASE_ADD_SYSTEM);
//
//		for (rc = 0; rc < 12; rc++)
//			HME_ReadMemory(fileConfig, &analogSettingList[rc], sizeof(tsAnalogSensor), BASE_ADD_ANALOG + (rc*SIZE_ANALOG_T));
//		for (rc = 0; rc < 6; rc++) {
//			HME_ReadMemory(fileConfig, &digitalSettingList[rc], sizeof(tsDigitalSensor), BASE_ADD_DIGITAL + (rc*SIZE_DIGITAL_T));
//			HME_ReadMemory(fileConfig, &serialSettingList[rc], sizeof(tsConfig_SerialSensor), BASE_ADD_SERIAL + (rc*SIZE_SERIAL_T));
//		}
//		for (rc = 0; rc < 15; rc++)
//			HME_ReadMemory(fileConfig, &virtualSensorList[rc], sizeof(tsVirtualSensor), BASE_ADD_VIRTUAL + (rc*SIZE_VIRTUAL_T));
//
//		HME_ReadMemory(fileConfig, &transmission, sizeof(tsTransmission), BASE_ADD_TRANSMISSION);
//		HME_ReadMemory(fileConfig, &cellular, sizeof(tsCellular), BASE_ADD_CELLULAR);
//		HME_ReadMemory(fileConfig, &u8SorttingChannel, sizeof(char)*50, BASE_ADD_CHANNEL_SORT);
//	}
//	else
//	{
//		appStartRountine();
//	}

	appStartRountine();
	UART_Init();
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&UART3_RxBuffer, 1);

	while(1)
	{
		event = osMessageGet(SerialQueueHandle, 16);
		if( event.status == osEventMessage )
		{
			switch(event.value.v)
			{
				case E_SERIAL_CHANNEL_CONFIG:
					appProcessCommand(sCommand[0].u8Buffer);
					break;
				case E_SERIAL_CHANNEL_COM_1:
//					HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_13);
					vProcessSerialChannel(E_SERIAL_CHANNEL_COM_1);
					break;
				case E_SERIAL_CHANNEL_COM_2:
					vProcessSerialChannel(E_SERIAL_CHANNEL_COM_2);
					break;
				case E_SERIAL_CHANNEL_COM_3:
					vProcessSerialChannel(E_SERIAL_CHANNEL_COM_3);
					break;
				case E_SERIAL_CHANNEL_COM_4:
					vProcessSerialChannel(E_SERIAL_CHANNEL_COM_4);
					break;
				case E_SERIAL_CHANNEL_RS485:
					vProcessSerialChannel(E_SERIAL_CHANNEL_RS485);
					break;
				case E_SERIAL_CHANNEL_SDI12:
					vProcessSerialChannel(E_SERIAL_CHANNEL_SDI12);
					break;
				case E_SERIAL_CHANNEL_SIM4G:
					HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_15);
					break;
				default:
					break;
			}
			memset(sCommand[event.value.v].u8Buffer, 0, COMMAND_BUF_SIZE);
			sCommand[event.value.v].u8Pos = 0;
		}
		osDelay(1);
	}
}

void vProcessRxChar(uint8_t u8Char, uint8_t port)
{
    if ((sCommand[port].u8Pos < COMMAND_BUF_SIZE)  && (u8Char != END_DATA_CONF) && (u8Char != 0x0d))
    {
        sCommand[port].u8Buffer[sCommand[port].u8Pos++] = u8Char;
    }
    else if ((sCommand[port].u8Pos >= COMMAND_BUF_SIZE) || (u8Char == END_DATA_CONF) || (u8Char == 0x0d))
    {
    	osMessagePut(SerialQueueHandle, port, 0);
//    	printf("port %d : %s\r\n",port,sCommand[port].u8Buffer);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart6.Instance)
	{
		vProcessRxChar(UART6_RxBuffer, E_SERIAL_CHANNEL_COM_2);
		HAL_UART_Receive_IT(&huart6, (uint8_t*)&UART6_RxBuffer, 1);
	}
	else if(huart->Instance == huart4.Instance)
	{
		vProcessRxChar(UART4_RxBuffer, E_SERIAL_CHANNEL_COM_1);
		HAL_UART_Receive_IT(&huart4, (uint8_t*)&UART4_RxBuffer, 1);
	}
	else if(huart->Instance == huart3.Instance)
	{
		vProcessRxChar(UART3_RxBuffer, E_SERIAL_CHANNEL_CONFIG);
		HAL_UART_Receive_IT(&huart3, (uint8_t*)&UART3_RxBuffer, 1);
	}
	else if(huart->Instance == huart5.Instance)
	{
		vProcessRxChar(UART5_RxBuffer, E_SERIAL_CHANNEL_COM_3);
		HAL_UART_Receive_IT(&huart5, (uint8_t*)&UART5_RxBuffer, 1);
	}
	else if(huart->Instance == huart1.Instance)
	{
		vProcessRxChar(UART1_RxBuffer, E_SERIAL_CHANNEL_COM_4);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&UART1_RxBuffer, 1);
	}
	else if(huart->Instance == huart2.Instance)
	{
		vProcessRxChar(UART2_RxBuffer, E_SERIAL_CHANNEL_RS485);
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&UART2_RxBuffer, 1);
	}
	else if(huart->Instance == huart7.Instance)
	{
		vProcessRxChar(UART7_RxBuffer, E_SERIAL_CHANNEL_SDI12);
		HAL_UART_Receive_IT(&huart7, (uint8_t*)&UART7_RxBuffer, 1);
	}
	else if(huart->Instance == huart8.Instance)
	{
		vProcessRxChar(UART8_RxBuffer, E_SERIAL_CHANNEL_SIM4G);
		HAL_UART_Receive_IT(&huart8, (uint8_t*)&UART8_RxBuffer, 1);
	} else
	{
		//Do nothing
	}
}

void GetIPConverted(const char *src, uint8_t *dst)
{
    int dwIP=0,dwIP_Part=0;
    while(1)
    {
        if(src[0] == 0)
        {
            dwIP = (dwIP << 8) | dwIP_Part;
            break;
        }
        if(src[0]=='.')
        {
            dwIP = (dwIP << 8) | dwIP_Part;
            dwIP_Part = 0;
           src++;
        }
        dwIP_Part = (dwIP_Part*10)+(src[0]-'0');
        src++;
    }

    *dst = (dwIP >> 24) & 0xFF;
    *(dst+1) = (dwIP >> 16) & 0xFF;
    *(dst+2) = (dwIP >> 8) & 0xFF;
    *(dst+3) = dwIP & 0xFF;
}
