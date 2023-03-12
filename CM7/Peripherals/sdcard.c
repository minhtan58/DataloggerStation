/*
 * sdcard.c
 *
 * Created on: 2-Dec-2021
 * Author: Huynhld@dicom.com.vn
 *
 */

#include "sdcard.h"
#include "main.h"

SD_HandleTypeDef hsd1;
SD_HandleTypeDef hsd2;

/* File system object for SD logical drive */
FATFS FatFs;
FIL myfile;
FRESULT fresult;
FILINFO fno;

/* File read/write count */
UINT br, bw;

/* Capacity */
FATFS *pfs;

char fileConfig[20] = "okela27.txt";
char filePathConfig[50] = "/public_html/DATA/";

void MX_SDMMC1_SD_Init(void)
{
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv = 4;
	if (HAL_SD_Init(&hsd1) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_SDMMC2_SD_Init(void)
{
	hsd2.Instance = SDMMC2;
	hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd2.Init.BusWide = SDMMC_BUS_WIDE_1B;  // 4bit SDMMC_BUS_WIDE_4B
	hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd2.Init.ClockDiv = 4;
	if (HAL_SD_Init(&hsd2) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hsd->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SDMMC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /**SDMMC1 GPIO Configuration
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    /* SDMMC1 interrupt Init */
    HAL_NVIC_SetPriority(SDMMC1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  /* USER CODE BEGIN SDMMC1_MspInit 1 */

  /* USER CODE END SDMMC1_MspInit 1 */
  }
  else if(hsd->Instance==SDMMC2)
  {
  /* USER CODE BEGIN SDMMC2_MspInit 0 */

  /* USER CODE END SDMMC2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
    PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SDMMC2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDMMC2 GPIO Configuration
    PB4 (NJTRST)     ------> SDMMC2_D3
    PD6     ------> SDMMC2_CK
    PB3 (JTDO/TRACESWO)     ------> SDMMC2_D2
    PD7     ------> SDMMC2_CMD
    PB15     ------> SDMMC2_D1
    PB14     ------> SDMMC2_D0
    */

    /**SDMMC2 GPIO Configuration
    PD6     ------> SDMMC2_CK
    PB3 (JTDO/TRACESWO)     ------> SDMMC2_D2
    PD7     ------> SDMMC2_CMD
    PB14     ------> SDMMC2_D0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF11_SDIO2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_SDIO2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_15|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_SDIO2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    */

    /* SDMMC2 interrupt Init */
    HAL_NVIC_SetPriority(SDMMC2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(SDMMC2_IRQn);
  /* USER CODE BEGIN SDMMC2_MspInit 1 */

  /* USER CODE END SDMMC2_MspInit 1 */
  }

}

/**
* @brief SD MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hsd: SD handle pointer
* @retval None
*/
void HAL_SD_MspDeInit(SD_HandleTypeDef* hsd)
{
  if(hsd->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspDeInit 0 */

  /* USER CODE END SDMMC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC1_CLK_DISABLE();

    /**SDMMC1 GPIO Configuration
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
    PC8     ------> SDMMC1_D0
	*/
    /**SDMMC1 GPIO Configuration
    PB3     ------> SDMMC2_D2
    PB4     ------> SDMMC2_D3
    PD6     ------> SDMMC2_CK
    PD7     ------> SDMMC2_CMD
    PB14     ------> SDMMC2_D0
    PB15     ------> SDMMC2_D1
    */
//    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12|GPIO_PIN_8);

//    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_8
                          |GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    /* SDMMC1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
  /* USER CODE BEGIN SDMMC1_MspDeInit 1 */

  /* USER CODE END SDMMC1_MspDeInit 1 */
  }
  else if(hsd->Instance==SDMMC2)
  {
  /* USER CODE BEGIN SDMMC2_MspDeInit 0 */

  /* USER CODE END SDMMC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDMMC2_CLK_DISABLE();

    /**SDMMC2 GPIO Configuration
    PB4 (NJTRST)     ------> SDMMC2_D3
    PD6     ------> SDMMC2_CK
    PB3 (JTDO/TRACESWO)     ------> SDMMC2_D2
    PD7     ------> SDMMC2_CMD
    PB15     ------> SDMMC2_D1
    PB14     ------> SDMMC2_D0
    */

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_14);
    //HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_15|GPIO_PIN_14);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_7);

    /* SDMMC2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SDMMC2_IRQn);
  /* USER CODE BEGIN SDMMC2_MspDeInit 1 */

  /* USER CODE END SDMMC2_MspDeInit 1 */
  }

}

/**
  * @brief Remove file.
  * @param filename File to be remoted.
  * @reval File function return code.
  */

FRESULT HME_SDCARD_Remove_File(TCHAR* filename)
{
    /* Check whether the file exists or not */
    fresult = f_stat(filename, &fno);
    if (fresult != FR_OK)
    {
        return fresult;
    }
    else
    {
        fresult = f_unlink(filename);
        if (fresult != FR_OK)
        {
        	return fresult;
        }
    }
    return fresult;
}

/**
  * @brief Write data to file.
  * @param filename File to be written.
  * @param data Pointer to data to be written.
  * @reval File function return code
  */

FRESULT HME_WriteMemory(TCHAR* filename, const void* buffer, uint16_t buffer_size, uint16_t address)
{
	f_mount(&FatFs,"",1);
    /* Create new file and start write to it. */
	fresult = f_open(&myfile, filename, FA_READ | FA_WRITE);
    if(fresult == FR_OK)
    {
    	f_lseek(&myfile, address);
    	fresult = f_write(&myfile, buffer, buffer_size, &bw);
       	f_close(&myfile);
    }
   	fresult = f_mount(&FatFs,"",0);
    return fresult;
}


FRESULT HME_ReadMemory(TCHAR* filename, void* buffer, uint16_t buffer_size, uint16_t address)
{
	f_mount(&FatFs,"",1);
	fresult = f_open(&myfile, filename, FA_READ | FA_OPEN_EXISTING);
    if(fresult == FR_OK)
    {
    	f_lseek(&myfile, address);
    	fresult = f_read(&myfile, buffer, buffer_size, &br);
       	f_close(&myfile);
    }
   	fresult = f_mount(&FatFs,"",0);
    return fresult;
}
