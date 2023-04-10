/**
  ******************************************************************************
  * @file    LwIP/LwIP_TCP_Echo_Client/Inc/tcp_echoclient.h
  * @author  MCD Application Team
  * @brief   Header file for tcp_echoclient.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCP_ECHOCLIENT_H__
#define __TCP_ECHOCLIENT_H__

/* Includes ------------------------------------------------------------------*/
uint8_t recev_buf[50];
uint8_t get_time;
uint8_t data[100];
/* Exported functions ------------------------------------------------------- */

void tcp_echoclient_connect(uint8_t ip_Adress0, uint8_t ip_Adress1, uint8_t ip_Adress2, uint8_t ip_Adress3, uint32_t port);

#endif /* __TCP_ECHOCLIENT_H__ */


