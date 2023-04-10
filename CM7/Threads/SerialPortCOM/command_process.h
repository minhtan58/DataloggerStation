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
tsSort sSortting[50];

#endif /* SERIALPORTCOM_COMMAND_PROCESS_H_ */
