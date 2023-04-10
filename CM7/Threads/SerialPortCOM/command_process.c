/*
 * command_process.c
 *
 *  Created on: Oct 28, 2022
 *      Author: minht
 */
/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_process.h"
#include "uart_handler.h"
#include "uart.h"
#include "analog_sensor_handler.h"
#include "sdcard.h"
#include "sensor_process.h"
#include "lwip.h"
#include "ftplib.h"
#include "tcp_echoclient.h"
#include "stm32h7xx_hal.h"
/****************************************************************************/
/***    Local Function Prototypes                     ***/
/****************************************************************************/
static void appLogin(void);
static void appOverView(void);
static void appSetting(void);
static void appAnalogSetting(void);
static void appDigitalSetting(void);
static void appSerialSetting(void);
static void appTransmissionSetting(void);
static void appCellularSetting(void);
static void appSortSetting(void);
static void appVirtualSetting(void);
static void appProcessVirtualSensor(uint8_t u8Channel);
static void appAnalogStatus(uint8_t u8Channel);
static void appDigitalStatus(uint8_t u8Channel);
static void appSerialStatus(uint8_t u8Channel);
static void appVirtualStatus(uint8_t u8Channel);
static void appTransmissionStatus(void);
static void appSensorStatus(void);
static void appGetDataSequence(void);
static void appCellularStatus(void);
static void appSortStatus(void);
static void appLogExport(void);
static void setTimeFromString(char *s, RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime);

/****************************************************************************/
/***    Local Variables                           ***/
/****************************************************************************/

static uint8_t u8CommandConfig[][8] = {"LN", "S7", "S8", "S91", "S92", "S9R", "RSS", "RSSDATA",\
                                       "RS11", "S11", "S12", "S13", "S14", "RS14", "S15", "RS15"};
/* structure to store configure data */
tsOverview overview;
tsSetting setting;
tsAnalogSensor analogSettingList[12];
tsDigitalSensor digitalSettingList[6];
tsConfig_SerialSensor serialSettingList[6];
tsTransmission transmission;
tsCellular cellular;
tsVirtualSensor virtualSensorList[15];
//uint8_t u8SorttingChannel[50];

extern FIL myfile;
extern FATFS FatFs;
extern UINT br, bw;

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTimeSys;
extern RTC_DateTypeDef sDateSys;
extern osTimerId periodicTimerHandle[12];
extern osTimerId periodicSerialTimer[6];

extern struct netif gnetif;
extern ip4_addr_t ipaddr;
extern ip4_addr_t netmask;
extern ip4_addr_t gw;
extern uint8_t IP_ADDRESS[4];
extern uint8_t NETMASK_ADDRESS[4];
extern uint8_t GATEWAY_ADDRESS[4];

extern TIM_HandleTypeDef htim2;

char *context=NULL;
uint8_t sortting[COMMAND_BUF_SIZE];
/****************************************************************************/
/***    Implementation                          */
/****************************************************************************/

void appStartRountine(void)
{
    // create new file
    f_mount(&FatFs,"",1);
    if (f_open(&myfile, fileConfig, FA_CREATE_ALWAYS|FA_READ|FA_WRITE) == FR_OK) {
        printf("Create new file ok\r\n");
    } else {
        printf("Create new file error\r\n");
    }
//    f_close(&myfile);
//    f_mount(&FatFs,"",0);


    /* Init overview info */
    strcpy((char*)overview.modelName, "DSL-21");
    strcpy((char*)overview.serialNumber, "SR6868");
    strcpy((char*)overview.firmwareVer, "REV 1.1");
    strcpy((char*)overview.macAddress, "AA:BB:CC:DD:EE:FF");
    strcpy((char*)overview.batteryVolt, "12V");
	f_lseek(&myfile, BASE_ADD_OVERVIEW);
	f_write(&myfile, &overview, sizeof(tsOverview), &bw);
//    HME_WriteMemory(fileConfig, &overview, sizeof(tsOverview), BASE_ADD_OVERVIEW);
    HAL_Delay(10);

    /* Init setting info */
    strcpy((char*)setting.stationName, "HaNoi");
    strcpy((char*)setting.stationID, "2930");
    strcpy((char*)setting.dateTime, "2022/01/01 00:00:00");
    strcpy((char*)setting.ipAddress, "192.168.2.123");
    strcpy((char*)setting.subnetMask, "255.255.255.0");
    strcpy((char*)setting.gateway, "192.168.2.204");
    strcpy((char*)setting.history, "2022/01/01 00:00:00");

//    GetIPConverted(&setting.ipAddress[0], &IP_ADDRESS[0]);
//    GetIPConverted(&setting.subnetMask[0], &NETMASK_ADDRESS[0]);
//    GetIPConverted(&setting.gateway[0], &GATEWAY_ADDRESS[0]);
//    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
//    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
//    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
//    netif_set_ipaddr(&gnetif, &ipaddr);
//    netif_set_netmask(&gnetif, &netmask);
//    netif_set_gw(&gnetif, &gw);

	f_lseek(&myfile, BASE_ADD_SYSTEM);
	f_write(&myfile, &setting, sizeof(tsSetting), &bw);
//    HME_WriteMemory(fileConfig, &setting, sizeof(tsSetting), BASE_ADD_SYSTEM);
    HAL_Delay(10);

    /* Init analog info */
    uint8_t u8Index, u8Channel[6];

    for (u8Index = 0; u8Index < 12; u8Index++)
    {
        snprintf((char*)&analogSettingList[u8Index].channel[0], 6, "AI%d", u8Index+1);
        strcpy((char*)analogSettingList[u8Index].name, "Analog");
        strcpy((char*)analogSettingList[u8Index].status, "Disabled");
        strcpy((char*)analogSettingList[u8Index].inputRange, "0 - 1 VDC");
        strcpy((char*)analogSettingList[u8Index].interval, "0");
        strcpy((char*)analogSettingList[u8Index].slope, "0");
        strcpy((char*)analogSettingList[u8Index].offset, "0");
        strcpy((char*)analogSettingList[u8Index].rightDigit, "0");
        strcpy((char*)analogSettingList[u8Index].unit, "m");
        strcpy((char*)analogSettingList[u8Index].history, "2022/01/01 00:00:00");
        analogSettingList[u8Index].value = 1.2345;

    	f_lseek(&myfile, BASE_ADD_ANALOG + (u8Index*SIZE_ANALOG_T));
    	f_write(&myfile, &analogSettingList[u8Index], sizeof(tsAnalogSensor), &bw);
//        HME_WriteMemory(fileConfig, &analogSettingList[u8Index], sizeof(tsAnalogSensor), BASE_ADD_ANALOG + (u8Index*SIZE_ANALOG_T));
        HAL_Delay(10);
    }

    /* Init digital info */
    for (u8Index = 0; u8Index < 6; u8Index++)
    {
        snprintf((char*)&digitalSettingList[u8Index].channel[0], 6, "DI%d", u8Index+1);
        strcpy((char*)digitalSettingList[u8Index].name, "Digital");
        strcpy((char*)digitalSettingList[u8Index].status, "Disabled");
        strcpy((char*)digitalSettingList[u8Index].mode, "Counter");
        strcpy((char*)digitalSettingList[u8Index].interval, "0");
        strcpy((char*)digitalSettingList[u8Index].slope, "0");
        strcpy((char*)digitalSettingList[u8Index].offset, "0");
        strcpy((char*)digitalSettingList[u8Index].rightDigit, "0");
        strcpy((char*)digitalSettingList[u8Index].unit, "m");
        strcpy((char*)digitalSettingList[u8Index].history, "2022/01/01 00:00:00");
        digitalSettingList[u8Index].value = 0.0;

    	f_lseek(&myfile, BASE_ADD_DIGITAL + (u8Index*SIZE_DIGITAL_T));
    	f_write(&myfile, &digitalSettingList[u8Index], sizeof(tsDigitalSensor), &bw);
//        HME_WriteMemory(fileConfig, &digitalSettingList[u8Index], sizeof(tsDigitalSensor), BASE_ADD_DIGITAL + (u8Index*SIZE_DIGITAL_T));
        HAL_Delay(10);
    }

    /* Init serital info */
    for (u8Index = 0; u8Index < 6; u8Index++)
    {
        if (u8Index < 4) {
            snprintf((char*)u8Channel, 8, "COM%d", u8Index+1);
        } else if (u8Index == 4) {
            snprintf((char*)u8Channel, 8, "RS485");
        } else if (u8Index == 5) {
            snprintf((char*)u8Channel, 8, "SDI-12");
        }

        strcpy((char*)serialSettingList[u8Index].channel, (char*)u8Channel);
        strcpy((char*)serialSettingList[u8Index].name, "Serial");
        strcpy((char*)serialSettingList[u8Index].status, "Disabled");
        strcpy((char*)serialSettingList[u8Index].baudrate, "9600");
        strcpy((char*)serialSettingList[u8Index].dataBit, "8");
        strcpy((char*)serialSettingList[u8Index].parity, "None");
        strcpy((char*)serialSettingList[u8Index].stopBit, "0");
        strcpy((char*)serialSettingList[u8Index].flowControl, "None");
        strcpy((char*)serialSettingList[u8Index].leadChar, "None");
        strcpy((char*)serialSettingList[u8Index].endOfLine, "None");
        strcpy((char*)serialSettingList[u8Index].interval, "0");
        strcpy((char*)serialSettingList[u8Index].getDataCmd, "CMD");
        strcpy((char*)serialSettingList[u8Index].unit, "mm");
        strcpy((char*)serialSettingList[u8Index].dataSequence, "N/A=1=m");
        strcpy((char*)serialSettingList[u8Index].history, "2022/01/01 00:00:00");

    	f_lseek(&myfile, BASE_ADD_SERIAL + (u8Index*SIZE_SERIAL_T));
    	f_write(&myfile, &serialSettingList[u8Index], sizeof(tsConfig_SerialSensor), &bw);
//        HME_WriteMemory(fileConfig, &serialSettingList[u8Index], sizeof(tsConfig_SerialSensor), BASE_ADD_SERIAL + (u8Index*SIZE_SERIAL_T));
        HAL_Delay(10);
    }

    /* Init transmission info */
    strcpy((char*)transmission.startTime, "10:00:00");
    strcpy((char*)transmission.interval, "1");
    strcpy((char*)transmission.URL, "192.168.2.76");
    strcpy((char*)transmission.port, "21");
    strcpy((char*)transmission.useName, "huynh");
    strcpy((char*)transmission.passWord, "1234");
    strcpy((char*)transmission.fileName, "okok.txt");
    strcpy((char*)transmission.headerInterval, "2");
    strcpy((char*)transmission.tcp_ip, "80");
    strcpy((char*)transmission.history, "2022/01/01 00:00:00");
    Config_Ftp();

	f_lseek(&myfile, BASE_ADD_TRANSMISSION);
	f_write(&myfile, &transmission, sizeof(tsTransmission), &bw);
//    HME_WriteMemory(fileConfig, &transmission, sizeof(tsTransmission), BASE_ADD_TRANSMISSION);
    HAL_Delay(10);

    /* Init cellular info */
    strcpy((char*)cellular.cellularMode, "Disabled");
    strcpy((char*)cellular.modemStatus, "Ready");
    strcpy((char*)cellular.operator, "Viettel");
    strcpy((char*)cellular.modemAccess, "HSPA");
    strcpy((char*)cellular.Band, "Band 3");
    strcpy((char*)cellular.phoneNumber, "0912345678");
    strcpy((char*)cellular.ipV4, "27.72.56.21");
    strcpy((char*)cellular.signalLevel, "Strong");
    strcpy((char*)cellular.history, "2022/01/01 00:00:00");

	f_lseek(&myfile, BASE_ADD_CELLULAR);
	f_write(&myfile, &cellular, sizeof(tsCellular), &bw);
//    HME_WriteMemory(fileConfig, &cellular, sizeof(tsCellular), BASE_ADD_CELLULAR);
    HAL_Delay(10);

    /* Init sort channel info */
//    strcpy((char*)u8SorttingChannel, "DI2&0&1;AI1&0&2;COM1&2&3");
	f_lseek(&myfile, BASE_ADD_CHANNEL_SORT);
	f_write(&myfile, (char*)"DI2&0&1;AI1&0&2;COM1&2&3", COMMAND_BUF_SIZE, &bw);
//    HME_WriteMemory(fileConfig, &u8SorttingChannel, sizeof(char)*50, BASE_ADD_CHANNEL_SORT);
    HAL_Delay(10);

    /* Init virtual sensor info */
    for (u8Index = 0; u8Index < 15; u8Index++)
    {
        snprintf((char*)&virtualSensorList[u8Index].channel[0], 6, "VS%d", u8Index+1);
        strcpy((char*)virtualSensorList[u8Index].name, "NA");
        strcpy((char*)virtualSensorList[u8Index].status, "Disabled");
        strcpy((char*)virtualSensorList[u8Index].vsType, "1");
        strcpy((char*)virtualSensorList[u8Index].inputSensor1, "AI1");
        strcpy((char*)virtualSensorList[u8Index].index1, "0");
        strcpy((char*)virtualSensorList[u8Index].inputSensor2, "AI2");
        strcpy((char*)virtualSensorList[u8Index].index2, "0");
        strcpy((char*)virtualSensorList[u8Index].startTime, "00:00:00");
        strcpy((char*)virtualSensorList[u8Index].interval, "0");
        strcpy((char*)virtualSensorList[u8Index].sensorHeight, "0");
        strcpy((char*)virtualSensorList[u8Index].dataSequence, "NA=NA=NA");

    	f_lseek(&myfile, BASE_ADD_VIRTUAL + (u8Index*SIZE_VIRTUAL_T));
    	f_write(&myfile, &virtualSensorList[u8Index], sizeof(tsVirtualSensor), &bw);
//        HME_WriteMemory(fileConfig, &virtualSensorList[u8Index], sizeof(tsVirtualSensor), BASE_ADD_VIRTUAL + (u8Index*SIZE_VIRTUAL_T));
        HAL_Delay(10);
    }

    f_close(&myfile);
    f_mount(&FatFs,"",0);
}

void appProcessCommand(uint8_t *pInputData)
{
    char *command;
    char delimiter[] = ";";
    command = strtok_r((char*)pInputData, delimiter, &context);
    uint8_t i;
    uint8_t u8Index = 0;
    for (i = 0; i < 16; i++) {
        if (0 != strcmp (command, (char*)u8CommandConfig[i])) {
            continue;
        }
        /* get command function */
        u8Index = i;
        break;
    }
    HAL_RTC_GetTime(&hrtc,&sTimeSys,RTC_FORMAT_BIN);

    switch(u8Index)
    {
        case 0:
            /* Command = "LN" -> Screen 5 */
            appLogin();
            break;
        case 1:
            /* Command = "S7" -> Screen 7 */
            appOverView();
            break;
        case 2:
            /* Command = "S8" -> Screen 8 */
            appSetting();
            break;
        case 3:
            /* Command = "S91" -> Screen 9.1 */
            appAnalogSetting();
            break;
        case 4:
            /* Command = "S92" -> Screen 9.2 */
            appDigitalSetting();
            break;
        case 5:
            /* Command = "S9R" -> Screen 9.3 */
            appSerialSetting();
            break;
        case 6:
            /* Command = "RSS;0x" -> Screen 10 */
            appSensorStatus();
            break;
        case 7:
            /* Command = "RSSDATA;xx" */
            /* To do: send data from serial sensor to app */
            appGetDataSequence();
            break;
        case 8:
            /* Command = "RS11" */
            /* To do: send parameter transmission setting to app */
            appTransmissionStatus();
            break;
        case 9:
            /* Command = "S11" -> Screen 11 */
            /* To do: save parameter transmission setting */
            appTransmissionSetting();
            break;
        case 10:
            /* Command = "S12" -> Show Log Message */
            appLogExport();
            break;
        case 11:
            /* S13 -> Setting Virtual Sensor */
            appVirtualSetting();
            break;
        case 12:
            /* Command = "S14" -> save sorting channel */
            appSortSetting();
            break;
        case 13:
            /* Command = "RS14" -> show sorting channel */
            appSortStatus();
            break;
        case 14:
            /* Command = "S15" -> save cellular setting */
            appCellularSetting();
            break;
        case 15:
            /* Command = "RS15" -> show cellular setting */
            appCellularStatus();
            break;
        default:
            break;
    }
}

static void appLogin(void)
{
    HAL_UART_Transmit(&huart3, (uint8_t*)"OK!", 3, HAL_MAX_DELAY);
}

static void appOverView(void)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

	HAL_RTC_GetTime(&hrtc,&sTimeSys,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&sDateSys,RTC_FORMAT_BIN);
    sprintf((char*)respone, "%d/%d/%d %02d:%02d:%02d", sDateSys.Year, sDateSys.Month, sDateSys.Date,\
    		                                           sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);
    strcpy(setting.dateTime, (char*)respone);

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s!", &overview.modelName[0], &setting.stationName[0],\
            &setting.stationID[0], &overview.serialNumber[0], &overview.firmwareVer[0], &overview.macAddress[0],\
            &setting.ipAddress[0], &setting.subnetMask[0], &setting.gateway[0], &overview.batteryVolt[0], &setting.dateTime[0]);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appSetting(void)
{
    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.stationName[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.stationID[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.dateTime[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.ipAddress[0], token);
    GetIPConverted(token, &IP_ADDRESS[0]);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.subnetMask[0], token);
    GetIPConverted(token, &NETMASK_ADDRESS[0]);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&setting.gateway[0], token);
    GetIPConverted(token, &GATEWAY_ADDRESS[0]);
    snprintf(&setting.history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);
    if (FR_OK != HME_WriteMemory(fileConfig, &setting, sizeof(tsSetting), BASE_ADD_SYSTEM))
    	Error_Handler();

    // copy and set datetime follow setting parameter
    setTimeFromString(&setting.dateTime[0], &sDateSys, &sTimeSys);

    GetIPConverted(&setting.ipAddress[0], &IP_ADDRESS[0]);
    GetIPConverted(&setting.subnetMask[0], &NETMASK_ADDRESS[0]);
    GetIPConverted(&setting.gateway[0], &GATEWAY_ADDRESS[0]);
    IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
    IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
    IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);
    netif_set_ipaddr(&gnetif, &ipaddr);
    netif_set_netmask(&gnetif, &netmask);
    netif_set_gw(&gnetif, &gw);

    HAL_UART_Transmit(&huart3, (uint8_t*)"8_OK!", 5, HAL_MAX_DELAY);
}

static void appAnalogSetting(void)
{
    uint8_t u8Index;
    char cAnalogChannel[][5] = {"AI1", "AI2", "AI3", "AI4", "AI5", "AI6",\
                                "AI7", "AI8", "AI9", "AI10", "AI11", "AI12"};

    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    /* Find analog channel corresponding */

    for (u8Index = 0; u8Index < 12; u8Index++)
    {
        if (0 == strcmp(token, cAnalogChannel[u8Index]))
        {
            strcpy((char*)analogSettingList[u8Index].channel, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].name, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].status, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].inputRange, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].interval, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].slope, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].offset, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].rightDigit, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)analogSettingList[u8Index].unit, token);
            snprintf((char*)&analogSettingList[u8Index].history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);

            if (analogSettingList[u8Index].status[0] == 'E')
                osTimerStart(periodicTimerHandle[u8Index], atoi(analogSettingList[u8Index].interval)*1000);
            else
                osTimerStop(periodicTimerHandle[u8Index]);

            if (FR_OK != HME_WriteMemory(fileConfig, &analogSettingList[u8Index], sizeof(tsAnalogSensor), BASE_ADD_ANALOG + (u8Index*SIZE_ANALOG_T)))
            	Error_Handler();
            HAL_Delay(1);
            HAL_UART_Transmit(&huart3, (uint8_t*)"91_OK!", 6, HAL_MAX_DELAY);
            break;
        }
    }
}

static void appDigitalSetting(void)
{
    uint8_t u8Index;
    char cDigitalChannel[][5] = {"DI1", "DI2", "DI3", "DI4", "DI5", "DI6"};

    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    /* Find analog channel corresponding */
    for (u8Index = 0; u8Index < 6; u8Index++)
    {
        if (0 == strcmp(token, cDigitalChannel[u8Index]))
        {
            strcpy((char*)digitalSettingList[u8Index].channel, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].name, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].status, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].mode, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].interval, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].slope, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].offset, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].rightDigit, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)digitalSettingList[u8Index].unit, token);
            snprintf((char*)&digitalSettingList[u8Index].history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);

            if (FR_OK != HME_WriteMemory(fileConfig, &digitalSettingList[u8Index], sizeof(tsDigitalSensor), BASE_ADD_DIGITAL + (u8Index*SIZE_DIGITAL_T)))
            	Error_Handler();
            HAL_Delay(1);
            HAL_UART_Transmit(&huart3, (uint8_t*)"92_OK!", 6, HAL_MAX_DELAY);
            break;
        }
    }
}

static void appSerialSetting(void)
{
    uint8_t u8Index;
    char cSerialChannel[][7] = {"COM1", "COM2", "COM3", "COM4", "RS485", "SDI-12"};
    int j=0;
    char *token;
    char delimiter1[2] = ";";
    char *sub_control[20];
    token = strtok_r(NULL, delimiter1, &context);
    /* Find serial channel corresponding */
    for (u8Index = 0; u8Index < 6; u8Index++)
    {
        if (0 == strcmp(token, cSerialChannel[u8Index]))
        {
            memset(&serialSettingList[u8Index], 0, sizeof(tsConfig_SerialSensor));
            strcpy((char*)serialSettingList[u8Index].channel, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].name, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].status, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].baudrate, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].dataBit, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].parity, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].stopBit, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].flowControl, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].leadChar, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].endOfLine, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].interval, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].getDataCmd, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].unit, token);
            token = strtok_r(NULL, delimiter1, &context);
            strcpy((char*)serialSettingList[u8Index].dataSequence, token);
            char* dummy = strdup(token); // duplicate dataSequence to dump data

            snprintf((char*)&serialSettingList[u8Index].history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);

            token = strtok_r(dummy, "&", &context);

            while(token != NULL)
            {
//                printf("===>%s\n\r", token);
                sub_control[j] = token;
                j++;
                token = strtok_r(NULL, "&", &context);
            }
            serialSettingList[u8Index].numDataType = j;
            for(j = 0; j < serialSettingList[u8Index].numDataType ; j++)
            {
                token = strtok_r(sub_control[j], "=", &context);
                strcpy((char*)serialSettingList[u8Index].dataType[j].name, token);
//                printf(" %s\n\r", serialSettingList[u8Index].dataType[j].name);
                token = strtok_r(NULL, "=", &context);
                strcpy((char*)serialSettingList[u8Index].dataType[j].index, token);
//                printf(" %s\n\r", serialSettingList[u8Index].dataType[j].index);
                token = strtok_r(NULL, "=", &context);
                strcpy((char*)serialSettingList[u8Index].dataType[j].unit, token);
//                printf(" %s\n\r", serialSettingList[u8Index].dataType[j].unit);
            }

            if (FR_OK != HME_WriteMemory(fileConfig, &serialSettingList[u8Index], sizeof(tsConfig_SerialSensor), BASE_ADD_SERIAL + (u8Index*SIZE_SERIAL_T)))
            	Error_Handler();
            HAL_Delay(1);
            HAL_UART_Transmit(&huart3, (uint8_t*)"9R_OK!", 6, HAL_MAX_DELAY);
            Setting_channel(u8Index);

            if (serialSettingList[u8Index].status[0] == 'E')
                osTimerStart(periodicSerialTimer[u8Index], atoi(serialSettingList[u8Index].interval)*1000);
            else
                osTimerStop(periodicSerialTimer[u8Index]);

//            HME_WriteMemory(fileConfig, &serialSettingList[u8Index], sizeof(tsConfig_SerialSensor), BASE_ADD_SERIAL + (u8Index*SIZE_SERIAL_T));
//            HAL_Delay(1);
//            HAL_UART_Transmit(&huart3, (uint8_t*)"9R_OK!", 6, HAL_MAX_DELAY);
            break;
        }
    }
}

extern osTimerId periodicNetworkTimer;

static void appTransmissionSetting(void)
{
    char *token;
    char delimiter[2] = ";";

    memset(&transmission, 0, sizeof(tsTransmission));
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.startTime[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.interval[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.URL[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.port[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.useName[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.passWord[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.fileName[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.headerInterval[0], token);
    token = strtok_r(NULL, delimiter, &context);
    strcpy(&transmission.tcp_ip[0], token);
    snprintf((char*)&transmission.history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);
    if (FR_OK != HME_WriteMemory(fileConfig, &transmission, sizeof(tsTransmission), BASE_ADD_TRANSMISSION))
    	Error_Handler();
    HAL_Delay(1);
    HAL_UART_Transmit(&huart3, (uint8_t*)"11_OK!", 6, HAL_MAX_DELAY);

    Config_Ftp();
    //tcp_echoclient_connect();
    //osTimerStop(periodicNetworkTimer);
    //osTimerStart(periodicNetworkTimer, atoi(transmission.interval)*1000);
//	HAL_TIM_Base_Start_IT(&htim2);
	header_interval = atoi(transmission.headerInterval);
	header_count = header_interval;
}

static void appCellularSetting(void)
{
    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    if (0 == strcmp(token, "true"))
        strcpy(&cellular.cellularMode[0], "Enabled");
    else
        strcpy(&cellular.cellularMode[0], "Disabled");
    snprintf(&cellular.history[0], 20, "2023/02/28 %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);
    if (FR_OK != HME_WriteMemory(fileConfig, &cellular, sizeof(tsCellular), BASE_ADD_CELLULAR))
    	Error_Handler();
    HAL_Delay(1);
    HAL_UART_Transmit(&huart3, (uint8_t*)"15_OK!", 6, HAL_MAX_DELAY);
}

static void appSortSetting(void)
{
    char *token;
    char delimiter[2] = "!";
//    char u8Sortting[COMMAND_BUF_SIZE];

    /*
    memset(&u8SorttingChannel[0], 0, 50);
    token = strtok_r(NULL, delimiter, &context);
    strcpy((char*)u8SorttingChannel, token);
    HME_WriteMemory(fileConfig, &u8SorttingChannel, sizeof(char)*50, BASE_ADD_CHANNEL_SORT);
    */
    token = strtok_r(NULL, delimiter, &context);
//    strcpy((char*)u8Sortting, token);
    if (FR_OK != HME_WriteMemory(fileConfig, token, COMMAND_BUF_SIZE, BASE_ADD_CHANNEL_SORT))
    	Error_Handler();
    HAL_Delay(1);
    HAL_UART_Transmit(&huart3, (uint8_t*)"14_OK!", 6, HAL_MAX_DELAY);
}

static void appVirtualSetting(void)
{
    uint8_t u8Index;
    char cVirtualChannel[][5] = {"VS1", "VS2", "VS3", "VS4", "VS5", "VS6", "VS7", "VS8",\
                                 "VS9", "VS10", "VS11", "VS12", "VS13", "VS14", "VS15"};

    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    /* Find virtual channel corresponding */
    for (u8Index = 0; u8Index < 15; u8Index++)
    {
        if (0 == strcmp(token, cVirtualChannel[u8Index]))
        {
            memset(&virtualSensorList[u8Index], 0, sizeof(tsVirtualSensor));
            strcpy((char*)virtualSensorList[u8Index].channel, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].name, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].status, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].vsType, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].inputSensor1, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].index1, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].inputSensor2, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].index2, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].startTime, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].interval, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].sensorHeight, token);
            token = strtok_r(NULL, delimiter, &context);
            strcpy((char*)virtualSensorList[u8Index].dataSequence, token);
            if (FR_OK != HME_WriteMemory(fileConfig, &virtualSensorList[u8Index], sizeof(tsVirtualSensor), BASE_ADD_VIRTUAL + (u8Index*SIZE_VIRTUAL_T)))
            	Error_Handler();
            HAL_Delay(1);
            HAL_UART_Transmit(&huart3, (uint8_t*)"13_OK!", 6, HAL_MAX_DELAY);
            break;
        }
    }
}

static void appGetDataSequence(void)
{
    uint8_t u8Index;
    char cChannel[][3] = {"19", "20", "21", "22", "23", "24", "25", "26", "27", "28", \
                          "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39"};
    char *token;
    char delimiter[2] = ";";
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    token = strtok_r(NULL, delimiter, &context);
    /* Find serial channel corresponding */
    for (u8Index = 0; u8Index < 22; u8Index++)
    {
        if (0 == strcmp(token, cChannel[u8Index]) && (u8Index < 6)) {
            u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s!", (char*)u8RxSerialSensor[u8Index]);
            HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
            break;
        } else if (0 == strcmp(token, cChannel[u8Index]) && (u8Index >= 6)) {
            appProcessVirtualSensor(u8Index-6);
            break;
        }
    }
}

static void appProcessVirtualSensor(uint8_t u8Channel)
{
    uint8_t u8Type, u8Count;
    u8Type = atoi(virtualSensorList[u8Channel].vsType);
    char *pMessage = malloc(30);
    switch (u8Type)
    {
        case 0:
            /* MET Win */
            u8Count = sprintf(pMessage, "11,22,33,44,55,66,77,88!");
            break;
        case 1:
            /* Dew Point */
            u8Count = sprintf(pMessage, "11!");
            break;
        case 2:
            /* QNH */
            u8Count = sprintf(pMessage, "22!");
            break;
        case 3:
            /* Solar */
            u8Count = sprintf(pMessage, "33!");
            break;
        case 4:
            /* Sunshine */
            u8Count = sprintf(pMessage, "44!");
            break;
        case 5:
            /* Rain */
            u8Count = sprintf(pMessage, "55!");
            break;
        case 6:
            /* Minimum */
            u8Count = sprintf(pMessage, "66!");
            break;
        case 7:
            /* Maximum */
            u8Count = sprintf(pMessage, "77!");
            break;
        case 8:
            /* Average */
            u8Count = sprintf(pMessage, "88!");
            break;
        default:
            break;
    }
    HAL_UART_Transmit(&huart3, (uint8_t*)&pMessage[0], u8Count, HAL_MAX_DELAY);
    free(pMessage);
}

static void appAnalogStatus(uint8_t u8Channel)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0, ndigit=0;
    char buffer[8];

    ndigit = lenHelper((int)analogSettingList[u8Channel].value) + atoi(analogSettingList[u8Channel].rightDigit) + 2;
    snprintf(buffer, ndigit, "%f", analogSettingList[u8Channel].value);
    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s!", (char*)analogSettingList[u8Channel].channel,\
            (char*)analogSettingList[u8Channel].name, (char*)analogSettingList[u8Channel].status,\
            (char*)analogSettingList[u8Channel].inputRange, (char*)analogSettingList[u8Channel].interval,\
            (char*)analogSettingList[u8Channel].slope, (char*)analogSettingList[u8Channel].offset,\
            (char*)analogSettingList[u8Channel].rightDigit, (char*)analogSettingList[u8Channel].unit, buffer);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appDigitalStatus(uint8_t u8Channel)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%.2f!", (char*)digitalSettingList[u8Channel].channel,\
            (char*)digitalSettingList[u8Channel].name, (char*)digitalSettingList[u8Channel].status,\
            (char*)digitalSettingList[u8Channel].mode, (char*)digitalSettingList[u8Channel].interval,\
            (char*)digitalSettingList[u8Channel].slope, (char*)digitalSettingList[u8Channel].offset,\
            (char*)digitalSettingList[u8Channel].rightDigit,(char*)digitalSettingList[u8Channel].unit, digitalSettingList[u8Channel].value);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appSerialStatus(uint8_t u8Channel)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s!",\
            (char*)serialSettingList[u8Channel].channel, (char*)serialSettingList[u8Channel].name,\
            (char*)serialSettingList[u8Channel].status, (char*)serialSettingList[u8Channel].baudrate,\
            (char*)serialSettingList[u8Channel].dataBit, (char*)serialSettingList[u8Channel].parity,\
            (char*)serialSettingList[u8Channel].stopBit, (char*)serialSettingList[u8Channel].flowControl,\
            (char*)serialSettingList[u8Channel].leadChar, (char*)serialSettingList[u8Channel].endOfLine,\
            (char*)serialSettingList[u8Channel].interval, (char*)serialSettingList[u8Channel].getDataCmd,\
            (char*)serialSettingList[u8Channel].unit, (char*)serialSettingList[u8Channel].dataSequence);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appVirtualStatus(uint8_t u8Channel)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s!", (char*)virtualSensorList[u8Channel].channel,\
            (char*)virtualSensorList[u8Channel].name, (char*)virtualSensorList[u8Channel].status, (char*)virtualSensorList[u8Channel].vsType,\
            (char*)virtualSensorList[u8Channel].inputSensor1, (char*)virtualSensorList[u8Channel].index1, (char*)virtualSensorList[u8Channel].inputSensor2,\
            (char*)virtualSensorList[u8Channel].index2, (char*)virtualSensorList[u8Channel].startTime, (char*)virtualSensorList[u8Channel].interval,\
            (char*)virtualSensorList[u8Channel].sensorHeight, (char*)virtualSensorList[u8Channel].dataSequence);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appTransmissionStatus(void)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s;%s!", &transmission.startTime[0], &transmission.interval[0], \
            &transmission.URL[0], &transmission.port[0], &transmission.useName[0], &transmission.passWord[0], \
            &transmission.fileName[0], &transmission.headerInterval[0], &transmission.tcp_ip[0]);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appCellularStatus(void)
{
    uint8_t respone[COMMAND_BUF_SIZE], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE, "%s;%s;%s;%s;%s;%s;%s;%s!", &cellular.cellularMode[0], &cellular.modemStatus[0], \
            &cellular.operator[0], &cellular.modemAccess[0], &cellular.Band[0], &cellular.phoneNumber[0], \
            &cellular.ipV4[0], &cellular.signalLevel[0]);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
}

static void appSortStatus(void)
{
    if (FR_OK != HME_ReadMemory(fileConfig, &sortting[0], COMMAND_BUF_SIZE, BASE_ADD_CHANNEL_SORT))
    	Error_Handler();
    strcat((char*)sortting, "!");
    HAL_UART_Transmit(&huart3, &sortting[0], strlen((char*)sortting), HAL_MAX_DELAY);
}

static void appSensorStatus(void)
{
    char cChannel[][3] = {"01", "02", "03", "04", "05", "06", "07", "08",\
                          "09", "10", "11", "12", "13", "14", "15", "16",\
                          "17", "18", "19", "20", "21", "22", "23", "24",\
                          "25", "26", "27", "28", "29", "30", "31", "32",\
                          "33", "34", "35", "36", "37", "38", "39"};
    uint8_t u8Index;
    char *token;
    char delimiter[2] = ";";

    token = strtok_r(NULL, delimiter, &context);
    /* Find channel corresponding */
    for (u8Index = 0; u8Index < 39; u8Index++)
    {
        if (0 == strcmp(token, cChannel[u8Index])) {
            if ((0 <= u8Index) && (u8Index <= 11)) {
                appAnalogStatus(u8Index);
                break;
            } else if ((12 <= u8Index) && (u8Index <= 17)) {
                appDigitalStatus(u8Index%12);
                break;
            } else if ((18 <= u8Index) && (u8Index <= 23)) {
                appSerialStatus(u8Index%18);
                break;
            } else {
                appVirtualStatus(u8Index%24);
                break;
            }
        }
    }
}

static void appLogExport(void)
{
    uint8_t i;
    uint8_t respone[COMMAND_BUF_SIZE/2], u8Count=0;

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;System;Changed setting!", setting.history);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);

    for (i = 0; i < 12; i++) {
        u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;Sensor;Changed AI%d setting!", analogSettingList[i].history, i+1);
        HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
    }

    for (i = 0; i < 6; i++) {
        u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;Sensor;Changed DI%d setting!", digitalSettingList[i].history, i+1);
        HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
    }

    for (i = 0; i < 6; i++) {
        u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;Sensor;Changed %s setting!", serialSettingList[i].history, serialSettingList[i].channel);
        HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);
    }

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;System;Changed transmission setting!", transmission.history);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);

    u8Count = snprintf((char*)respone, COMMAND_BUF_SIZE/2, "%s;System;Changed cellular setting!", cellular.history);
    HAL_UART_Transmit(&huart3, &respone[0], u8Count, HAL_MAX_DELAY);

    HAL_UART_Transmit(&huart3, (uint8_t*)"!", 1, HAL_MAX_DELAY);
}

static void setTimeFromString(char *s, RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime)
{
	char buff[24];
	strcpy(buff, s);
	char *token;

	token = strtok_r(buff, "/: ", &context);
	sDate->Year = (atoi(token)-2000);

	token = strtok_r(NULL, "/: ", &context);
	sDate->Month = atoi(token);

	token = strtok_r(NULL, "/: ", &context);
	sDate->Date = atoi(token);

	token = strtok_r(NULL, "/: ", &context);
	sTime->Hours = atoi(token);

	token = strtok_r(NULL, "/: ", &context);
	sTime->Minutes = atoi(token);

	token = strtok_r(NULL, "/: ", &context);
	sTime->Seconds = atoi(token);

	if (HAL_RTC_SetTime(&hrtc, sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
	    Error_Handler();
	}

	if (HAL_RTC_SetDate(&hrtc, sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
	    Error_Handler();
	}
}
