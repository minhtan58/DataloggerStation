/*
 * structs.h
 *
 *  Created on: Aug 3, 2021
 *      Author: Admin
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdbool.h>
#include "enums.h"
#include "main.h"
#include "defines.h"

/**
  * @brief  Overview structures definition
  */
typedef struct
{
    char modelName[8];       ///< Model Name
    char serialNumber[8];    ///< Serial Number
    char firmwareVer[8];     ///< Firmware Version
    char macAddress[20];     ///< Mac Address
    char batteryVolt[5];     ///< Battery Voltage
} tsOverview;

/**
  * @brief  System Setting structures definition
  */
typedef struct
{
    char stationName[14];     ///< Station Name
    char stationID[8];        ///< Station ID
    char dateTime[24];        ///< Date & time
    char ipAddress[20];       ///< IP Address
    char subnetMask[20];      ///< Subnet Mask
    char gateway[20];         ///< Gateway
    char history[20];         ///< Latest config
} tsSetting;

/**
  * @brief  Setting Analog channel structures definition
  */
typedef struct
{
    char channel[6];        ///< Channel Name
    char status[10];        ///< Status
    char name[10];          ///< Name
    char inputRange[10];    ///< Input Range
    char interval[6];       ///< Interval
    char slope[6];          ///< Slope
    char offset[6];         ///< Offset
    char rightDigit[2];     ///< Right Digit
    char unit[6];           ///< Unit
    char history[20];       ///< Latest config
} tsAnalogSensor;

/**
  * @brief  Setting Digital channel structures definition
  */
typedef struct
{
    char channel[6];        ///< Channel Name
    char status[10];        ///< Status
    char name[10];          ///< Name
    char mode[10];          ///< Mode
    char interval[6];       ///< Interval
    char slope[6];          ///< Slope
    char offset[6];         ///< Offset
    char rightDigit[2];     ///< Right Digit
    char unit[6];           ///< Unit
    char history[20];       ///< Latest config
} tsDigitalSensor;

/**
  * @brief  Setting Serial channel structures definition
  */

typedef struct {
    char name[10];
    char index[2];
    char unit[5];
}tsUsedData;

typedef struct
{
    char channel[8];       ///< Channel Name
    char status[10];        ///< Status
    char name[10];         ///< Name
    char baudrate[7];      ///< Baudrates
    char dataBit[2];       ///< Data bits
    char parity[6];        ///< Parity
    char stopBit[2];       ///< Stop bits
    char flowControl[6];   ///< Flow control
    char leadChar[12];     ///< Lead char
    char endOfLine[12];    ///< End of line
    char interval[6];      ///< Interval
    char getDataCmd[12];   ///< Get data command
    char unit[6];
    char dataSequence[50];
    char numDataType;
	tsUsedData dataType[10];
    char history[20];       ///< Latest config
} tsConfig_SerialSensor;

/**
  * @brief  Serial Transmissions setting structures definition
  */
typedef struct
{
    char startTime[15];        ///< Start time
    char interval[6];          ///< Interval
    char URL[30];              ///< URL
    char port[6];              ///< Port
    char useName[30];          ///< Username
    char passWord[30];         ///< Password
    char fileName[20];         ///< File name
    char headerInterval[6];    ///< Header interval
    char tcp_ip[6];            ///< TCP/IP
    char history[20];          ///< Latest config
} tsTransmission;

/**
  * @brief  Serial Cellular setting structures definition
  */
typedef struct
{
    char cellularMode[10];
    char modemStatus[10];
    char operator[10];
    char modemAccess[10];
    char Band[20];
    char phoneNumber[12];
    char ipV4[20];
    char signalLevel[10];
    char history[20];       ///< Latest config
} tsCellular;

/**
  * @brief  Virtual sensor setting structures definition
  */
typedef struct
{
    char channel[6];
    char name[4];
    char status[10];
    char vsType[2];
    char inputSensor1[6];
    char index1[2];
    char inputSensor2[6];
    char index2[2];
    char startTime[15];
    char interval[6];
    char sensorHeight[6];
    char dataSequence[128];
} tsVirtualSensor;

#endif /* STRUCTS_H_ */
