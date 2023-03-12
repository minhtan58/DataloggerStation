/*
 * sim4g_process.h
 *
 *  Created on: Nov 12, 2022
 *      Author: minht
 */

#ifndef SERIALPORTCOM_SIM4G_PROCESS_H_
#define SERIALPORTCOM_SIM4G_PROCESS_H_

#include "stm32h7xx_hal.h"

uint8_t Sim_sendCommand(char* command,char* response,uint32_t timeout);
void Sim_configGPRSnFTP(void);
void Sim_uploadFileFromFTP(char* data);
void Sim_downloadFileFromFTP(void);

#endif /* SERIALPORTCOM_SIM4G_PROCESS_H_ */
