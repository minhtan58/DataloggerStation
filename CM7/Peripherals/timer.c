/*
 * timer.c
 *
 *  Created on: Oct 19, 2022
 *      Author: minht
 */
#include "timer.h"
#include "cmsis_os.h"
#include "enums.h"
#include "tcp_echoclient.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim8;

extern osMessageQId NetworkQueueHandle;
extern osMessageQId digitalQueueHandle;

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		HAL_IncTick();
	}

	if(htim->Instance == htim2.Instance)
	{
		get_time = 1;
		tcp_echoclient_connect(129,6,15,28,13);
	}
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM2_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 40000-1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 100000-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/* TIM3 init function */
void MX_TIM3_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 200-1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* TIM4 init function */
void MX_TIM4_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 4000-1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 4000-1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/* TIM5 init function */
void MX_TIM5_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 40000-1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 40000-1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

///* TIM8 init function */
//void MX_TIM8_Init(void)
//{
//	/* USER CODE BEGIN TIM8_Init 0 */
//
//	/* USER CODE END TIM8_Init 0 */
//
//	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//	TIM_MasterConfigTypeDef sMasterConfig = {0};
//	TIM_OC_InitTypeDef sConfigOC = {0};
//	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
//
//	/* USER CODE BEGIN TIM8_Init 1 */
//
//	/* USER CODE END TIM8_Init 1 */
//	htim8.Instance = TIM8;
//	htim8.Init.Prescaler = 100-1;
//	htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htim8.Init.Period = 100-1;
//	htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	htim8.Init.RepetitionCounter = 0;
//	htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//	if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//	if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//	if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	sConfigOC.OCMode = TIM_OCMODE_PWM1;
//	sConfigOC.Pulse = 0;
//	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
//	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
//	if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//	sBreakDeadTimeConfig.DeadTime = 0;
//	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//	sBreakDeadTimeConfig.BreakFilter = 0;
//	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
//	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
//	sBreakDeadTimeConfig.Break2Filter = 0;
//	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
//	if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
//	{
//	Error_Handler();
//	}
//	/* USER CODE BEGIN TIM8_Init 2 */
//
//	/* USER CODE END TIM8_Init 2 */
//	HAL_TIM_MspPostInit(&htim8);
//
//}

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	if(timHandle->Instance==TIM8)
//	{
//	/* USER CODE BEGIN TIM8_MspPostInit 0 */
//
//	/* USER CODE END TIM8_MspPostInit 0 */
//
//	__HAL_RCC_GPIOJ_CLK_ENABLE();
//	/**TIM8 GPIO Configuration
//	PJ8     ------> TIM8_CH1
//	*/
//	GPIO_InitStruct.Pin = GPIO_PIN_8;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
//	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);
//
//	/* USER CODE BEGIN TIM8_MspPostInit 1 */
//
//	/* USER CODE END TIM8_MspPostInit 1 */
//	}
//}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(tim_baseHandle->Instance==TIM2)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		/* TIM2 interrupt Init */
		HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(tim_baseHandle->Instance==TIM3)
	{
	/* USER CODE BEGIN TIM3_MspInit 0 */

	/* USER CODE END TIM3_MspInit 0 */
	/* TIM3 clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**TIM3 GPIO Configuration
	PB5     ------> TIM3_CH2
	PA6     ------> TIM3_CH1
	PB1     ------> TIM3_CH4
	PB0     ------> TIM3_CH3
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	/* USER CODE BEGIN TIM3_MspInit 1 */

	/* USER CODE END TIM3_MspInit 1 */
	}
	else if(tim_baseHandle->Instance==TIM5)
	{
	/* USER CODE BEGIN TIM5_MspInit 0 */

	/* USER CODE END TIM5_MspInit 0 */
	/* TIM5 clock enable */
	__HAL_RCC_TIM5_CLK_ENABLE();

	/* TIM5 interrupt Init */
	HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	/* USER CODE BEGIN TIM5_MspInit 1 */

	/* USER CODE END TIM5_MspInit 1 */
	}
//	else if(tim_baseHandle->Instance==TIM8)
//	{
//	/* USER CODE BEGIN TIM8_MspInit 0 */
//
//	/* USER CODE END TIM8_MspInit 0 */
//	/* TIM8 clock enable */
//	__HAL_RCC_TIM8_CLK_ENABLE();
//
//	/* TIM8 interrupt Init */
//	HAL_NVIC_SetPriority(TIM8_CC_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
//	/* USER CODE BEGIN TIM8_MspInit 1 */
//
//	/* USER CODE END TIM8_MspInit 1 */
//	}
}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
	if(tim_baseHandle->Instance==TIM2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();

		/* TIM2 interrupt DeInit */
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
	}
	else if(tim_baseHandle->Instance==TIM3)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM3_CLK_DISABLE();

		/**TIM3 GPIO Configuration
		PB5     ------> TIM3_CH2
		PA6     ------> TIM3_CH1
		PB1     ------> TIM3_CH4
		PB0     ------> TIM3_CH3
		*/
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_0);

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

		/* TIM3 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
	}
	else if(tim_baseHandle->Instance==TIM4)
	{
		/* TIM4 clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();

		/* TIM4 interrupt Init */
		HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(tim_baseHandle->Instance==TIM5)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM5_CLK_DISABLE();

		/* TIM5 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM5_IRQn);
	}
//	else if(tim_baseHandle->Instance==TIM8)
//	{
//		/* USER CODE BEGIN TIM8_MspDeInit 0 */
//
//		/* USER CODE END TIM8_MspDeInit 0 */
//		/* Peripheral clock disable */
//		__HAL_RCC_TIM8_CLK_DISABLE();
//
//		/* TIM8 interrupt Deinit */
//		HAL_NVIC_DisableIRQ(TIM8_CC_IRQn);
//		/* USER CODE BEGIN TIM8_MspDeInit 1 */
//
//		/* USER CODE END TIM8_MspDeInit 1 */
//	}
}
