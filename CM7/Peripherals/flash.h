/*
 * flash.h
 *
 *  Created on: Dec 12, 2022
 *      Author: HuynhLD
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32h7xx_hal.h"

#define FLASH_WORD_LEN  8

uint32_t FLASH_GetSector(uint32_t u32Address);
uint32_t FLASH_WriteData(uint32_t u32StartAddress, uint32_t *pData, uint32_t u32Size);
void FLASH_ReadData(uint32_t u32StartAddress, uint32_t *pData, uint32_t u32Size); 

#endif /* FLASH_H_ */
