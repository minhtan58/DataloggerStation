/*
 * flash.c
 *
 *  Created on: Dec 27, 2022
 *      Author: HuynhLD
 */

#include "flash.h"

/**
  * @brief  Get the number of sector in the flash memory
  * @param  u32Address Specifies the flash address to be programmed.
  * @retval The number of sector
  */
uint32_t FLASH_GetSector(uint32_t u32Address)
{
    uint32_t sector;

    /* Bank1 || Bank2 */
    if ( ((u32Address >= 0x08000000) && (u32Address < 0x08020000)) || ((u32Address >= 0x08100000) && (u32Address < 0x08120000)) )
        sector = FLASH_SECTOR_0;
    else if ( ((u32Address >= 0x08020000) && (u32Address < 0x08040000)) || ((u32Address >= 0x08120000) && (u32Address < 0x08140000)) )
        sector = FLASH_SECTOR_1;
    else if ( ((u32Address >= 0x08040000) && (u32Address < 0x08060000)) || ((u32Address >= 0x08140000) && (u32Address < 0x08160000)) )
        sector = FLASH_SECTOR_2;
    else if ( ((u32Address >= 0x08060000) && (u32Address < 0x08080000)) || ((u32Address >= 0x08160000) && (u32Address < 0x08180000)) )
        sector = FLASH_SECTOR_3;
    else if ( ((u32Address >= 0x08080000) && (u32Address < 0x080A0000)) || ((u32Address >= 0x08180000) && (u32Address < 0x081A0000)) )
        sector = FLASH_SECTOR_4;
    else if ( ((u32Address >= 0x080A0000) && (u32Address < 0x080C0000)) || ((u32Address >= 0x081A0000) && (u32Address < 0x081C0000)) )
        sector = FLASH_SECTOR_5;
    else if ( ((u32Address >= 0x080C0000) && (u32Address < 0x080E0000)) || ((u32Address >= 0x081C0000) && (u32Address < 0x081E0000)) )
        sector = FLASH_SECTOR_6;
    else if ( ((u32Address >= 0x080E0000) && (u32Address < 0x08100000)) || ((u32Address >= 0x081E0000) && (u32Address < 0x08200000)) )
        sector = FLASH_SECTOR_7;

    return sector;
}

/**
  * @brief  Program a flash word at a specified address
  * @param  u32StartAddress Specifies the flash address to be programmed.
  * @param  *pData The pointer to the 32bit data, that you want into the flash
  * @param  u32Size the number of work, that needs to be the written in the flash
  * @retval The error code or HAL_OK if success
  */
uint32_t FLASH_WriteData(uint32_t u32StartAddress, uint32_t *pData, uint32_t u32Size)
{
    uint32_t u32Index = 0;

    HAL_FLASH_Unlock();

    while (u32Index < u32Size)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, u32StartAddress, &pData[u32Index]) == HAL_OK)
        {
            u32StartAddress += (4*FLASH_WORD_LEN);
            u32Index += FLASH_WORD_LEN;
        } else {
            return HAL_FLASH_GetError();
        }
    }

    HAL_FLASH_Lock();

    return HAL_OK;
}

/**
  * @brief  Read the data from flash
  * @param  u32StartAddress Specifies the flash address to be programmed.
  * @param  *pData The pointer to the 32bit data, that you want to store the data
  * @param  u32Size the number of work, that you want to read from the flash
  * @retval None
  */
void FLASH_ReadData(uint32_t u32StartAddress, uint32_t *pData, uint32_t u32Size)
{
    while(1)
    {
        *pData = *(__IO uint32_t*)u32StartAddress;
        u32StartAddress += 4;
        pData++;
        if (!(u32Size--)) break;
    }
}
