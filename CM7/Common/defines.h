/*
 * defines.h
 *
 *  Created on: Aug 5, 2021
 *      Author: Admin
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/** @defgroup Overview setting screen
  * @{
  */
#define    SIZE_OVERVIEW_T      0x100
#define    BASE_ADD_OVERVIEW    0x2b00
/**
  * @}
  */

/** @defgroup Address System setting screen
  * @{
  */
#define    SIZE_SYSTEM_T      0x100
#define    BASE_ADD_SYSTEM    0x00
/**
  * @}
  */

/** @defgroup Address Setting analog channel screen
  * @{
  */
#define    SIZE_ANALOG_T      0x100
#define    BASE_ADD_ANALOG    0x100

/**
  * @}
  */

/** @defgroup Address Setting digital channel screen
  * @{
  */
#define    SIZE_DIGITAL_T      0x100
#define    BASE_ADD_DIGITAL    0xd00
/**
  * @}
  */

/** @defgroup Address Setting serial channel (RS232, RS485 & SDI12) screen
  * @{
  */
#define    SIZE_SERIAL_T      0x100
#define    BASE_ADD_SERIAL    0x1300
/**
  * @}
  */

/** @defgroup Address of Transmissions setting screen
  * @{
  */
#define    SIZE_TRANSMISSION_T      0x100
#define    BASE_ADD_TRANSMISSION    0x1900
/**
  * @}
  */

/** @defgroup Address of Virtual sensor setting screen
  * @{
  */
#define    SIZE_VIRTUAL_T      0x100
#define    BASE_ADD_VIRTUAL    0x1a00
/**
  * @}
  */

/** @defgroup Address of Cellular setting screen
  * @{
  */
#define    SIZE_CELLULAR_T      0x100
#define    BASE_ADD_CELLULAR    0x2900
/**
  * @}
  */

/** @defgroup Address of Cellular setting screen
  * @{
  */
#define    SIZE_CHANNEL_SORT_T      0x100
#define    BASE_ADD_CHANNEL_SORT    0x2a00
/**
  * @}
  */

#endif /* DEFINES_H_ */
