/*
 * sensor_process.c
 *
 *  Created on: Oct 28, 2022
 *      Author: minht
 */

#include <string.h>
#include <stdio.h>
#include "sensor_process.h"
#include "uart.h"
#include "structs.h"
#include "uart_handler.h"
#include <stdlib.h>
#include "sdi12.h"

extern tsConfig_SerialSensor serialSettingList[6];
extern tsCommand sCommand[8];

char data_com[10][50];
char dataSerialUsed[128];

osMessageQId serialQueueHandle;
uint8_t serial_arg[6] = {1,2,3,4,5,6};

extern volatile uint8_t UART7_RxBuffer;

void serialPTCallback(void const * argument);

char* Get_SerialData(void)
{
    uint8_t length = 0;
    memset(dataSerialUsed, 0, 128);

    for(int i = 0; i < 6; i++)
    {
        if(strcmp("Enabled", serialSettingList[i].status) == 0)
        {
            length += sprintf(dataSerialUsed+length, "%s", data_com[i]);
        }
    }
//    printf("Data: %s\r\n", dataSerialUsed);
    return dataSerialUsed;
}

char* Get_SerialName(void)
{
	uint8_t i, j;
    memset(dataSerialUsed, 0, 128);

    for(i = 0; i < 6; i++)
    {
        if(strcmp("Enabled", serialSettingList[i].status) == 0)
        {
        	for (j = 0; j < serialSettingList[i].numDataType; j++)
        	{
        		strcat(dataSerialUsed,";");
        		strcat(dataSerialUsed, serialSettingList[i].dataType[j].name);
        	}
        }
    }

    return dataSerialUsed;
}

void vProcessSerialChannel(uint8_t u8Channel)
{
    char delimiters[] = ";,"; /* Add multiple delemiter */
    int i=0,j,k;
    char *control[20];
    memset(data_com[u8Channel-1], 0, 50);
    strcpy((char*)u8RxSerialSensor[u8Channel-1], (char*)sCommand[u8Channel].u8Buffer);
    char *token = strtok((char*)sCommand[u8Channel].u8Buffer, delimiters);

    while(token != NULL)
    {
        control[i] = token;
        i++;
        token = strtok(NULL, delimiters);
    }

    for(j = 0; j < i; j++)
    {
        for(k = 0; k < serialSettingList[u8Channel-1].numDataType; k++ )
        {
            if(j == (atoi(serialSettingList[u8Channel-1].dataType[k].index)-1))
            {
                strcat(data_com[u8Channel-1], ";");
                strcat(data_com[u8Channel-1], control[j]);
            }
        }
    }
//    printf("data_com = %s\n\r",data_com[u8Channel-1]);
}

void Serial_Setup_Timer(void)
{
    for (uint8_t i = 0; i < 6; i++)
        if (serialSettingList[i].status[0] == 'E')
            osTimerStart(periodicSerialTimer[i], atoi(serialSettingList[i].interval)*1000);
}

char addr = '1';
//char data[100];
extern SDI12_TypeDef sdi12;
SDI12_Measure_TypeDef measurement_info;

void SerialSensor_Task(void const * argument)
{
    osEvent event;
    uint8_t u8CR = 0x0d;  /* Carriage Return */

    osMessageQDef(serialQueue, 16, uint16_t);
    serialQueueHandle = osMessageCreate(osMessageQ(serialQueue), NULL);

    osTimerDef(periodicSerialTimer1, serialPTCallback);
    periodicSerialTimer[0] = osTimerCreate(osTimer(periodicSerialTimer1), osTimerPeriodic, (void*)&serial_arg[0]);

    osTimerDef(periodicSerialTimer2, serialPTCallback);
    periodicSerialTimer[1] = osTimerCreate(osTimer(periodicSerialTimer2), osTimerPeriodic, (void*)&serial_arg[1]);

    osTimerDef(periodicSerialTimer3, serialPTCallback);
    periodicSerialTimer[2] = osTimerCreate(osTimer(periodicSerialTimer3), osTimerPeriodic, (void*)&serial_arg[2]);

    osTimerDef(periodicSerialTimer4, serialPTCallback);
    periodicSerialTimer[3] = osTimerCreate(osTimer(periodicSerialTimer4), osTimerPeriodic, (void*)&serial_arg[3]);

    osTimerDef(periodicSerialTimer5, serialPTCallback);
    periodicSerialTimer[4] = osTimerCreate(osTimer(periodicSerialTimer5), osTimerPeriodic, (void*)&serial_arg[4]);

    osTimerDef(periodicSerialTimer6, serialPTCallback);
    periodicSerialTimer[5] = osTimerCreate(osTimer(periodicSerialTimer6), osTimerPeriodic, (void*)&serial_arg[5]);

    SDI12_Init(&huart7);
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin = sdi12.Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(sdi12.Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(sdi12.Port, (uint16_t) sdi12.Pin, GPIO_PIN_RESET);

    while(1)
    {
        event = osMessageGet(serialQueueHandle, 10);

        if (event.status == osEventMessage)
        {
            switch(event.value.v)
            {
                case E_SERIAL_CHANNEL_COM_1:
                    // send measurement command to sensor
                    HAL_UART_Transmit(&huart4, (uint8_t*)serialSettingList[0].getDataCmd, strlen((char*)serialSettingList[0].getDataCmd), HAL_MAX_DELAY);
                    break;
                case E_SERIAL_CHANNEL_COM_2:
                    HAL_UART_Transmit(&huart6, (uint8_t*)serialSettingList[1].getDataCmd, strlen((char*)serialSettingList[1].getDataCmd), HAL_MAX_DELAY);
                    break;
                case E_SERIAL_CHANNEL_COM_3:
                    HAL_UART_Transmit(&huart5, (uint8_t*)serialSettingList[2].getDataCmd, strlen((char*)serialSettingList[2].getDataCmd), HAL_MAX_DELAY);
                    break;
                case E_SERIAL_CHANNEL_COM_4:
                    HAL_UART_Transmit(&huart1, (uint8_t*)serialSettingList[3].getDataCmd, strlen((char*)serialSettingList[3].getDataCmd), HAL_MAX_DELAY);
                    break;
                case E_SERIAL_CHANNEL_RS485:
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET); /* Set DE pin to transmit */
                    HAL_UART_Transmit(&huart2, (uint8_t*)serialSettingList[4].getDataCmd, strlen((char*)serialSettingList[4].getDataCmd), HAL_MAX_DELAY);
                    HAL_UART_Transmit(&huart2, (uint8_t*)&u8CR, 1, HAL_MAX_DELAY);
                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
                    break;
                case E_SERIAL_CHANNEL_SDI12:
                {
//                    HAL_UART_Transmit(&huart7, (uint8_t*)serialSettingList[5].getDataCmd, strlen((char*)serialSettingList[5].getDataCmd), HAL_MAX_DELAY);


//            		SDI12_StartMeasurement(addr, &measurement_info);
//            		HAL_Delay(measurement_info.Time * 1000);
//            		SDI12_SendData(addr, &measurement_info, );

            		char cmd[] = { addr, 'D', 0, '!'};
            		SDI12_QueryDevice(cmd, 4, sCommand[E_SERIAL_CHANNEL_SDI12].u8Buffer, COMMAND_BUF_SIZE/2);
            		vProcessSerialChannel(E_SERIAL_CHANNEL_SDI12);
        			memset(sCommand[E_SERIAL_CHANNEL_SDI12].u8Buffer, 0, COMMAND_BUF_SIZE);
        			sCommand[E_SERIAL_CHANNEL_SDI12].u8Pos = 0;
                    break;
                }
                default:
                    break;
            }
        }
        osDelay(1);
    }
}

void serialPTCallback(void const * argument)
{
	uint8_t *data = (uint8_t*)pvTimerGetTimerID((TimerHandle_t)argument);
	osMessagePut(serialQueueHandle, *data, 0);
}
