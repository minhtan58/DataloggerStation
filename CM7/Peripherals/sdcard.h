/*
 * SDCARD.h
 *
 * Created on: 2-Dec-2021
 * Author: Huynhld@dicom.com.vn
 *
 */

#ifndef __SDCARD_H
#define __SDCARD_H

#include "fatfs.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define LENGH_OF_LINE   120

char fileConfig[20];
char fileData[20];
char filePathConfig[50];

void MX_SDMMC1_SD_Init(void);
void MX_SDMMC2_SD_Init(void);

FRESULT HME_SDCARD_Remove_File(TCHAR* filename);
FRESULT HME_WriteMemory(TCHAR* filename, const void* buffer, uint16_t buffer_size, uint16_t address);
FRESULT HME_ReadMemory(TCHAR* filename, void* buffer, uint16_t buffer_size, uint16_t address);
FRESULT HME_WriteFile(TCHAR* filename, TCHAR* buffer, uint16_t buffer_size);
void CreateNewDir(uint16_t year, uint8_t month, uint8_t day);

#endif /* __SDCARD_H */
