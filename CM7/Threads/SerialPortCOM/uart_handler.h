/*
 * uart_handler.h
 *
 *  Created on: Oct 12, 2022
 *      Author: minht
 */

#ifndef SERIALPORTCOM_UART_HANDLER_H_
#define SERIALPORTCOM_UART_HANDLER_H_

#include "stm32h7xx_hal.h"

#define	   COMMAND_BUF_SIZE   	256

typedef struct
{
    uint8_t  u8Buffer[COMMAND_BUF_SIZE];
    uint8_t  u8Pos;
}tsCommand;

char u8RxSerialSensor[6][COMMAND_BUF_SIZE];

void UART_Init(void);
void Setting_channel(uint8_t channel);
void SerialPortCom_Task(void const * argument);
void vProcessRxChar(uint8_t u8Char, uint8_t port);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void GetIPConverted(const char *src, uint8_t *dst);

#endif /* SERIALPORTCOM_UART_HANDLER_H_ */
