/*
 * command_process.h
 *
 *  Created on: Oct 28, 2022
 *      Author: minht
 */

#ifndef SERIALPORTCOM_COMMAND_PROCESS_H_
#define SERIALPORTCOM_COMMAND_PROCESS_H_

#include "stm32h7xx_hal.h"
#include "app_common.h"

void appProcessCommand(uint8_t *pInputData);
void appStartRountine(void);

/* data */
volatile float fAnalogDataList[12];
volatile float fDigitalDataList[6];

/* structure to store configure data */
extern tsOverview overview;
extern tsSetting setting;
extern tsAnalogSensor analogSettingList[12];
extern tsDigitalSensor digitalSettingList[6];
extern tsConfig_SerialSensor serialSettingList[6];
extern tsTransmission transmission;
extern tsCellular cellular;
extern tsVirtualSensor virtualSensorList[15];

uint8_t u8SorttingChannel[50];

#endif /* SERIALPORTCOM_COMMAND_PROCESS_H_ */
