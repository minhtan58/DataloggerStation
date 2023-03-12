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

extern tsConfig_SerialSensor serialSettingList[6];
extern tsCommand sCommand[8];

char data_com[10][50];
char dataSerialUsed[128];

osMessageQId serialQueueHandle;

void serialPTCallback1(void const * argument);
void serialPTCallback2(void const * argument);
void serialPTCallback3(void const * argument);
void serialPTCallback4(void const * argument);
void serialPTCallback5(void const * argument);
void serialPTCallback6(void const * argument);

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

void vProcessSerialChannel(uint8_t u8Channel)
{
    char delimiters[] = ";";
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
    {
        if (serialSettingList[i].status[0] == 'E')
            osTimerStart(periodicSerialTimer[i], atoi(serialSettingList[i].interval)*1000);
        else
            osTimerStop(periodicSerialTimer[i]);
    }
}

void SerialSensor_Task(void const * argument)
{
    osEvent event;

    osMessageQDef(serialQueue, 16, uint16_t);
    serialQueueHandle = osMessageCreate(osMessageQ(serialQueue), NULL);

    osTimerDef(periodicSerialTimer1, serialPTCallback1);
    periodicSerialTimer[0] = osTimerCreate(osTimer(periodicSerialTimer1), osTimerPeriodic, NULL);

    osTimerDef(periodicSerialTimer2, serialPTCallback2);
    periodicSerialTimer[1] = osTimerCreate(osTimer(periodicSerialTimer2), osTimerPeriodic, NULL);

    osTimerDef(periodicSerialTimer3, serialPTCallback3);
    periodicSerialTimer[2] = osTimerCreate(osTimer(periodicSerialTimer3), osTimerPeriodic, NULL);

    osTimerDef(periodicSerialTimer4, serialPTCallback4);
    periodicSerialTimer[3] = osTimerCreate(osTimer(periodicSerialTimer4), osTimerPeriodic, NULL);

    osTimerDef(periodicSerialTimer5, serialPTCallback5);
    periodicSerialTimer[4] = osTimerCreate(osTimer(periodicSerialTimer5), osTimerPeriodic, NULL);

    osTimerDef(periodicSerialTimer6, serialPTCallback6);
    periodicSerialTimer[5] = osTimerCreate(osTimer(periodicSerialTimer6), osTimerPeriodic, NULL);

    while(1)
    {
        event = osMessageGet(serialQueueHandle, 10);
        if (event.status == osEventMessage)
        {
            switch(event.value.v)
            {
                case E_SERIAL_CHANNEL_COM_1:
                    // send measurement command to sensor
                    HAL_UART_Transmit(&huart4, (uint8_t*)serialSettingList[0].getDataCmd, strlen((char*)serialSettingList[0].getDataCmd), 10);
                    break;
                case E_SERIAL_CHANNEL_COM_2:
                    HAL_UART_Transmit(&huart6, (uint8_t*)serialSettingList[1].getDataCmd, strlen((char*)serialSettingList[1].getDataCmd), 10);
                    break;
                case E_SERIAL_CHANNEL_COM_3:
                    HAL_UART_Transmit(&huart5, (uint8_t*)serialSettingList[2].getDataCmd, strlen((char*)serialSettingList[2].getDataCmd), 10);
                    break;
                case E_SERIAL_CHANNEL_COM_4:
                    HAL_UART_Transmit(&huart1, (uint8_t*)serialSettingList[3].getDataCmd, strlen((char*)serialSettingList[3].getDataCmd), 10);
                    break;
                case E_SERIAL_CHANNEL_RS485:
                    HAL_UART_Transmit(&huart2, (uint8_t*)serialSettingList[4].getDataCmd, strlen((char*)serialSettingList[4].getDataCmd), 10);
                    break;
                case E_SERIAL_CHANNEL_SDI12:
                    HAL_UART_Transmit(&huart7, (uint8_t*)serialSettingList[5].getDataCmd, strlen((char*)serialSettingList[5].getDataCmd), 10);
                    break;
                default:
                    break;
            }
        }
        osDelay(1);
    }
}

void serialPTCallback1(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_COM_1, 0);
}

void serialPTCallback2(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_COM_2, 0);
}

void serialPTCallback3(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_COM_3, 0);
}

void serialPTCallback4(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_COM_4, 0);
}

void serialPTCallback5(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_RS485, 0);
}

void serialPTCallback6(void const * argument)
{
    osMessagePut(serialQueueHandle, E_SERIAL_CHANNEL_SDI12, 0);
}

