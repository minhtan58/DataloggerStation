/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi5;
SPI_HandleTypeDef hspi4;

DMA_HandleTypeDef hdma_spi5_rx;
DMA_HandleTypeDef hdma_spi5_tx;

/* SPI4 init function */
void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI5 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode              = SPI_MODE_MASTER;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi4.Init.Direction         = SPI_DIRECTION_2LINES;
  hspi4.Init.CLKPhase          = SPI_PHASE_2EDGE;
  hspi4.Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi4.Init.DataSize          = SPI_DATASIZE_8BIT;
  hspi4.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial     = 7;
  hspi4.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  hspi4.Init.NSS               = SPI_NSS_SOFT;
  hspi4.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
  hspi4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/* SPI5 init function */
void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */
	  hspi5.Instance               = SPI5;
	  hspi5.Init.Mode 				= SPI_MODE_MASTER;
	  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	  hspi5.Init.Direction         = SPI_DIRECTION_2LINES;
	  hspi5.Init.CLKPhase          = SPI_PHASE_1EDGE;
	  hspi5.Init.CLKPolarity       = SPI_POLARITY_LOW;
	  hspi5.Init.DataSize          = SPI_DATASIZE_8BIT;
	  hspi5.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	  hspi5.Init.TIMode            = SPI_TIMODE_DISABLE;
	  hspi5.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	  hspi5.Init.CRCPolynomial     = 7;
	  hspi5.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	  hspi5.Init.NSS               = SPI_NSS_HARD_OUTPUT;
	  hspi5.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;  /* Recommended setting to avoid glitches */
  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */

  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hspi->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspInit 0 */

  /* USER CODE END SPI5_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI5;
    PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* SPI5 clock enable */
    __HAL_RCC_SPI5_CLK_ENABLE();

    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    /**SPI5 GPIO Configuration
    PK0     ------> SPI5_SCK
    PJ11     ------> SPI5_MISO
    PJ10     ------> SPI5_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    /* SPI5 DMA Init */
    /* SPI5_RX Init */
    hdma_spi5_rx.Instance = DMA1_Stream0;
    hdma_spi5_rx.Init.Request = DMA_REQUEST_SPI5_RX;
    hdma_spi5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_rx.Init.Mode = DMA_NORMAL;
    hdma_spi5_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmarx,hdma_spi5_rx);

    /* SPI5_TX Init */
    hdma_spi5_tx.Instance = DMA1_Stream1;
    hdma_spi5_tx.Init.Request = DMA_REQUEST_SPI5_TX;
    hdma_spi5_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi5_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi5_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi5_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi5_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi5_tx.Init.Mode = DMA_NORMAL;
    hdma_spi5_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi5_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi5_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmatx,hdma_spi5_tx);

    /* SPI5 interrupt Init */
    HAL_NVIC_SetPriority(SPI5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI5_IRQn);
  /* USER CODE BEGIN SPI5_MspInit 1 */

  /* USER CODE END SPI5_MspInit 1 */
  }
  else if (hspi->Instance==SPI4)
  {
  /* USER CODE BEGIN SPI4_MspInit 0 */

  /* USER CODE END SPI4_MspInit 0 */

  /** Initializes the peripherals clock
    */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI4;
    PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
  	  Error_Handler();
  	}

  	/* Peripheral clock enable */
  	__HAL_RCC_SPI4_CLK_ENABLE();

  	__HAL_RCC_GPIOE_CLK_ENABLE();
  	/**SPI4 GPIO Configuration
  	PE2     ------> SPI4_SCK
  	PE5     ------> SPI5_MISO
  	PE6     ------> SPI5_MOSI
  	*/
  	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6;
  	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  	GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
  	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  	/* SPI4 interrupt Init */
  	HAL_NVIC_SetPriority(SPI4_IRQn, 6, 0);
  	HAL_NVIC_EnableIRQ(SPI4_IRQn);
  	/* USER CODE BEGIN SPI4_MspInit 1 */

  	/* USER CODE END SPI4_MspInit 1 */
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI5)
  {
  /* USER CODE BEGIN SPI5_MspDeInit 0 */

  /* USER CODE END SPI5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI5_CLK_DISABLE();

    /**SPI5 GPIO Configuration
    PK0     ------> SPI5_SCK
    PJ11     ------> SPI5_MISO
    PJ10     ------> SPI5_MOSI
    */
    HAL_GPIO_DeInit(GPIOK, GPIO_PIN_0|GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOJ, GPIO_PIN_11|GPIO_PIN_10);

    /* SPI5 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmarx);
    HAL_DMA_DeInit(hspi->hdmatx);

    /* SPI5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(SPI5_IRQn);
  /* USER CODE BEGIN SPI5_MspDeInit 1 */

  /* USER CODE END SPI5_MspDeInit 1 */
  }
  else if(hspi->Instance==SPI4)
  {
  /* USER CODE BEGIN SPI5_MspDeInit 0 */

  /* USER CODE END SPI5_MspDeInit 0 */
    /* Peripheral clock disable */
	__HAL_RCC_SPI4_CLK_DISABLE();

	/**SPI5 GPIO Configuration
	PE2     ------> SPI5_SCK
	PE5     ------> SPI5_MISO
	PE6     ------> SPI5_MOSI
	*/
	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6);

	/* SPI4 interrupt DeInit */
	HAL_NVIC_DisableIRQ(SPI4_IRQn);
  /* USER CODE BEGIN SPI4_MspDeInit 1 */

  /* USER CODE END SPI4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
