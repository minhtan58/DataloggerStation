/*
 * lcd_display.h
 *
 * Created on: May 17, 2021
 * Author: Admin
 */

#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "u8g2/u8g2.h"

/* HuynhLD add */
/* Enum screen ID */
typedef enum
{
    E_LCD_HOME,
    E_LCD_OVERVIEW,
    E_LCD_SYSTEM_SETTING,
    E_LCD_IO_SETTING,
    E_LCD_SENSOR_STATUS,
    E_LCD_TRANSMISSION_SETTING,
    E_LCD_STORAGE_SETTING,
    E_LCD_STORAGE_TIME,
    E_LCD_STORAGE_USB,
    E_LCD_STORAGE_SDCARD,
    E_LCD_STORAGE_DELETE
}LCD_teState;

u8g2_t u8g2;

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr);

uint8_t u8x8_byte_3wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                               void *arg_ptr);

void Lcd_Task(void const *argument);
void lcdDisplay(void);
void lcdChangeState(uint8_t u8button);

#endif /* LCD_DISPLAY_H_ */
