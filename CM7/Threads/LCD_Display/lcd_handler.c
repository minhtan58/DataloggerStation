/*
 * lcd_display.c
 *
 * Created on: May 5, 2021
 * Author: Admin
 */
#include <lcd_handler.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "app_common.h"
#include "structs.h"
#include "insign_sensor.h"

extern RTC_HandleTypeDef hrtc;

osMessageQId GPIOQueueHandle;

/* structure to store configure data */

extern tsOverview overview;
extern tsSetting setting;
extern tsAnalogSensor analogSettingList[12];
extern tsDigitalSensor digitalSettingList[6];
extern tsConfig_SerialSensor serialSettingList[6];
extern tsTransmission transmission;
extern tsCellular cellular;
extern tsVirtualSensor virtualSensorList[15];

extern uint8_t u8SorttingChannel[50];

/* data */
extern volatile float fAnalogDataList[12];
extern volatile float fDigitalDataList[6];
float humi, temp;

static uint8_t u8cursor, u8preCursor, u8screenID;
static LCD_teState currentState;
static uint8_t u8usbStt = 0, u8sdcardStt = 0;


static void lcdCursorDown(void);
static void lcdCursorUp(void);
static void lcdHome(uint8_t screenID);
static void lcdOverview(uint8_t screenID);
static void lcdSystemSetting(uint8_t screenID);
static void lcdSensorStatus(uint8_t screenID);
static void lcdTransmissionSetting(uint8_t screenID);
static void lcdStorageSetting(void);
static void lcdStorageTimeSetting(void);
static void lcdUSBSetting(void);
static void lcdSDcardSetting(void);
static void lcdDeleteSetting(void);

float hum, temp;

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            HAL_Delay(1);
            break;
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
    }
    return 1;
}

uint8_t u8x8_byte_3wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                               void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_BYTE_SEND:
            HAL_SPI_Transmit(&hspi5, (uint8_t *) arg_ptr, arg_int, 100);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1, !arg_int);
        default:
            return 0;
    }
    return 1;
}

/**
  * @brief LCD thread handle
  * @param None
  * @retval None
  */
void Lcd_Task(void const *argument)
{
    osEvent event;
    osMessageQDef(buttonQueue, 12, uint16_t);
    GPIOQueueHandle = osMessageCreate(osMessageQ(buttonQueue), NULL);

    u8cursor = 0;
    u8preCursor = 0;
    u8screenID = 1;
    currentState = E_LCD_HOME;

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET); // Turn on background Led
    u8g2_Setup_st7920_s_192x32_f(&u8g2, U8G2_R0, u8x8_byte_3wire_hw_spi,
                                 u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_SetFontMode(&u8g2, 1);    // Transparent
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);

    lcdDisplay();
    insignWakeUp();
    insignSoftReset();
    osDelay(100);
    insignReadID();

    while(1)
    {
        event = osMessageGet(GPIOQueueHandle, 10);
        if( event.status == osEventMessage )
        {
            lcdChangeState(event.value.v);
        }
        lcdDisplay();
        osDelay(10);
    }

}

/**
  * @brief function move cursor down
  * @param None
  * @retval None
  */
void lcdCursorDown(void)
{
    u8preCursor = u8cursor;
    (u8cursor < 3) ? (u8cursor += 1) : (u8cursor = 0);
}

/**
  * @brief function move cursor up
  * @param None
  * @retval None
  */
void lcdCursorUp(void)
{
    u8preCursor = u8cursor;
    (u8cursor > 0) ? (u8cursor -= 1) : (u8cursor = 3);
}

/**
  * @brief function display information to monitor
  * @param None
  * @retval None
  */
void lcdDisplay(void)
{
    switch (currentState)
    {
        case E_LCD_HOME:
            lcdHome(u8screenID); break;
        case E_LCD_OVERVIEW:
            lcdOverview(u8screenID); break;
        case E_LCD_SYSTEM_SETTING:
            lcdSystemSetting(u8screenID); break;
        case E_LCD_SENSOR_STATUS:
            lcdSensorStatus(u8screenID); break;
        case E_LCD_TRANSMISSION_SETTING:
            lcdTransmissionSetting(u8screenID); break;
        case E_LCD_STORAGE_SETTING:
            lcdStorageSetting(); break;
        case E_LCD_STORAGE_TIME:
            lcdStorageTimeSetting(); break;
        case E_LCD_STORAGE_USB:
            lcdUSBSetting(); break;
        case E_LCD_STORAGE_SDCARD:
            lcdSDcardSetting(); break;
        case E_LCD_STORAGE_DELETE:
            lcdDeleteSetting(); break;
        default:
            break;
    }
}

/**
  * @brief function handle monitor state changing when button pressed
  * @param None
  * @retval None
  */
void lcdChangeState(uint8_t u8button)
{
    switch (currentState)
    {
        case E_LCD_HOME:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Button Select
                    if (1 == u8screenID) {
                        switch (u8cursor)
                        {
                            case 0: currentState = E_LCD_OVERVIEW; break;
                            case 1: currentState = E_LCD_SYSTEM_SETTING; break;
                            case 2: currentState = E_LCD_SENSOR_STATUS; break;
                            case 3: currentState = E_LCD_TRANSMISSION_SETTING; break;
                            default: break;
                         }
                    } else if (2 == u8screenID) {
                        switch (u8cursor)
                        {
                            case 0: currentState = E_LCD_STORAGE_SETTING; break;
                            default: break;
                        }
                    } else {}
                    u8screenID = 1;
                    u8cursor = 0; // reset cursor position before change screen
                    break;
                case APP_E_EVENT_BUTTON_UP: // Button Up
                    if (2 == u8screenID && 0 == u8cursor)
                        u8screenID = 1;
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Button Down
                    if (1 == u8screenID && 3 == u8cursor) // reach last row
                        u8screenID = 2;
                    lcdCursorDown();
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_OVERVIEW:
                switch(u8button)
                {
                    case APP_E_EVENT_BUTTON_UP: // Up
                        if (2 == u8screenID && 0 == u8cursor)  // reach first row
                            u8screenID = 1;
                        else if (3 == u8screenID && 0 == u8cursor)
                            u8screenID = 2;
                        lcdCursorUp();
                        break;
                    case APP_E_EVENT_BUTTON_DOWN: // Down
                        if (1 == u8screenID && 3 == u8cursor) // reach last row
                              u8screenID = 2;
                        else if (2 == u8screenID && 3 == u8cursor)
                              u8screenID = 3;
                        lcdCursorDown();
                        break;
                    case APP_E_EVENT_BUTTON_CANCEL: // Back
                        currentState = E_LCD_HOME;
                        u8cursor = 0;
                        u8screenID = 1;
                        break;
                    default:
                        break;
                }
            break;
        case E_LCD_SYSTEM_SETTING:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_UP: // Up
                    if (2 == u8screenID && 0 == u8cursor)
                        u8screenID = 1;
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    if (1 == u8screenID && 3 == u8cursor) // reach last row
                        u8screenID = 2;
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_HOME;
                    u8cursor = 1;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_SENSOR_STATUS:
            switch(u8button)
            {
                case APP_E_EVENT_BUTTON_UP: // Up
                    if (2 == u8screenID && 0 == u8cursor)
                        u8screenID = 1;
                    else if (3 == u8screenID && 0 == u8cursor)
                        u8screenID = 2;
                    else if (4 == u8screenID && 0 == u8cursor)
                        u8screenID = 3;
                    else if (5 == u8screenID && 0 == u8cursor)
                        u8screenID = 4;
                    else if (6 == u8screenID && 0 == u8cursor)
                        u8screenID = 5;
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    if (1 == u8screenID && 3 == u8cursor)
                        u8screenID = 2;
                    else if (2 == u8screenID && 3 == u8cursor)
                        u8screenID = 3;
                    else if (3 == u8screenID && 3 == u8cursor)
                        u8screenID = 4;
                    else if (4 == u8screenID && 3 == u8cursor)
                        u8screenID = 5;
                    else if (5 == u8screenID && 3 == u8cursor)
                        u8screenID = 6;
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_HOME;
                    u8cursor = 2;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_TRANSMISSION_SETTING:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_UP: // Up
                    if (2 == u8screenID && 0 == u8cursor) u8screenID = 1;
                    else if (3 == u8screenID && 0 == u8cursor) u8screenID = 2;
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    if (1 == u8screenID && 3 == u8cursor)
                        u8screenID = 2;
                    else if (2 == u8screenID && 3 == u8cursor)
                        u8screenID = 3;
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_HOME;
                    u8cursor = 3;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_STORAGE_SETTING:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Select
                    switch (u8cursor)
                    {
                        case 0: currentState = E_LCD_STORAGE_TIME; break;
                        case 1: currentState = E_LCD_STORAGE_USB; break;
                        case 2: currentState = E_LCD_STORAGE_SDCARD; break;
                        case 3: currentState = E_LCD_STORAGE_DELETE; break;
                        default: break;
                    }
                    u8cursor = 0;
                    u8screenID = 2;
                    break;
                case APP_E_EVENT_BUTTON_UP: // Up
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_HOME;
                    u8cursor = 0;
                    u8screenID = 2;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_STORAGE_TIME:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Select
                    switch (u8cursor)
                    {
                        case 1:
                        case 2:
                        case 3:
                        default: break;
                    }
                    break;
                case APP_E_EVENT_BUTTON_UP: // Up
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_STORAGE_SETTING;
                    u8cursor = 0;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_STORAGE_USB:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Select
                    switch (u8cursor)
                    {
                        case 1: // update variable value enable or disable
                        case 2:
                        default: break;
                    }
                    break;
                case APP_E_EVENT_BUTTON_UP: // Up
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_STORAGE_SETTING;
                    u8cursor = 1;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_STORAGE_SDCARD:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Select
                    switch (u8cursor)
                    {
                        case 1:
                        case 2:
                        default: break;
                    }
                    break;
                case APP_E_EVENT_BUTTON_UP: // Up
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_STORAGE_SETTING;
                    u8cursor = 2;
                    u8screenID = 1;
                    break;
                default:
                    break;
            }
            break;
        case E_LCD_STORAGE_DELETE:
            switch (u8button)
            {
                case APP_E_EVENT_BUTTON_ENTER: // Select
                    currentState = E_LCD_STORAGE_SETTING;
                    u8cursor = 0;
                    u8screenID = 1;
                    break;
                case APP_E_EVENT_BUTTON_UP: // Up
                    lcdCursorUp();
                    break;
                case APP_E_EVENT_BUTTON_DOWN: // Down
                    lcdCursorDown();
                    break;
                case APP_E_EVENT_BUTTON_CANCEL: // Back
                    currentState = E_LCD_STORAGE_SETTING;
                    u8cursor = 0;
                    u8screenID = 2;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case GPIO_PIN_3:
            osMessagePut(GPIOQueueHandle, APP_E_EVENT_BUTTON_DOWN, 0);
            break;
        case GPIO_PIN_4:
            osMessagePut(GPIOQueueHandle, APP_E_EVENT_BUTTON_CANCEL, 0);
            break;
        case GPIO_PIN_5:
            osMessagePut(GPIOQueueHandle, APP_E_EVENT_BUTTON_UP, 0);
            break;
        case GPIO_PIN_6:
            osMessagePut(GPIOQueueHandle, APP_E_EVENT_BUTTON_ENTER, 0);
            break;
        default:
            break;
    }
}

static void lcdHome(uint8_t screenID)
{
    u8g2_ClearBuffer(&u8g2);
    if (1 == screenID){
        u8g2_DrawStr(&u8g2, 1, 7, "OVERVIEW");
        u8g2_DrawStr(&u8g2, 1, 15, "SYSTEM SETTING");
        u8g2_DrawStr(&u8g2, 1, 23, "SENSOR STATUS");
        u8g2_DrawStr(&u8g2, 1, 31, "TRANSMISSION SETUP");
    } else {
        u8g2_DrawStr(&u8g2, 1, 7, "STORAGE SETTING");
    }
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdOverview(uint8_t screenID)
{
    char msg[28];

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "OVERVIEW");
    if (1 == screenID){
        u8g2_DrawStr(&u8g2, 1, 15, "MODEL: D-SL21");
        sprintf(msg, "STATION: %s", setting.stationName);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "STATION ID: %s", setting.stationID);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (2 == screenID){
        sprintf(msg, "SERIAL NO: %s", overview.serialNumber);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "FW VER: %s", overview.firmwareVer);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "MAC: %s", overview.macAddress);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else {
        sprintf(msg, "IP: %s", setting.ipAddress);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        insignMeasurements(&humi, &temp);
        sprintf(msg, "Temp: %.1f C", temp);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "Humi: %.1f %%", humi);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    }
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdSystemSetting(uint8_t screenID)
{
    char msg[28];
    RTC_TimeTypeDef sTimeSys;
    RTC_DateTypeDef sDateSys;

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "SYSTEM SETTING");
    if (1 == u8screenID){
        sprintf(msg, "IP: %s", setting.ipAddress);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "SUBNET: %s", setting.subnetMask);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "GATEWAY: %s", setting.gateway);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else {
        HAL_RTC_GetTime(&hrtc,&sTimeSys,RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc,&sDateSys,RTC_FORMAT_BIN);
        sprintf(msg, "DATE: %d/%d/%d", sDateSys.Date, sDateSys.Month, sDateSys.Year);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "TIME: %02d:%02d:%02d", sTimeSys.Hours, sTimeSys.Minutes, sTimeSys.Seconds);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
    }
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdSensorStatus(uint8_t screenID)
{
    char msg[28];

    u8g2_ClearBuffer(&u8g2);
    if (1 == u8screenID) {
        sprintf(msg, "%s  %s %s", analogSettingList[0].channel, analogSettingList[0].name, analogSettingList[0].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[1].channel, analogSettingList[1].name, analogSettingList[1].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[2].channel, analogSettingList[2].name, analogSettingList[2].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[3].channel, analogSettingList[3].name, analogSettingList[3].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (2 == u8screenID) {
        sprintf(msg, "%s  %s %s", analogSettingList[4].channel, analogSettingList[4].name, analogSettingList[4].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[5].channel, analogSettingList[5].name, analogSettingList[5].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[6].channel, analogSettingList[6].name, analogSettingList[6].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s  %s %s", analogSettingList[7].channel, analogSettingList[7].name, analogSettingList[7].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (3 == u8screenID) {
        sprintf(msg, "%s  %s %s", analogSettingList[8].channel, analogSettingList[8].name, analogSettingList[8].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s %s %s", analogSettingList[9].channel, analogSettingList[9].name, analogSettingList[9].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s %s %s", analogSettingList[10].channel, analogSettingList[10].name, analogSettingList[10].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s %s %s", analogSettingList[11].channel, analogSettingList[11].name, analogSettingList[11].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (4 == u8screenID) {
        sprintf(msg, "%s  %s %s", digitalSettingList[0].channel, digitalSettingList[0].name, digitalSettingList[0].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s  %s %s", digitalSettingList[1].channel, digitalSettingList[1].name, digitalSettingList[1].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s  %s %s", digitalSettingList[2].channel, digitalSettingList[2].name, digitalSettingList[2].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s  %s %s", digitalSettingList[3].channel, digitalSettingList[3].name, digitalSettingList[3].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (5 == u8screenID) {
        sprintf(msg, "%s  %s %s", digitalSettingList[4].channel, digitalSettingList[4].name, digitalSettingList[4].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s  %s %s", digitalSettingList[5].channel, digitalSettingList[5].name, digitalSettingList[5].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s  %s %s", serialSettingList[0].channel, serialSettingList[0].name, serialSettingList[0].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s  %s %s", serialSettingList[1].channel, serialSettingList[1].name, serialSettingList[1].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (6 == u8screenID) {
        sprintf(msg, "%s  %s %s", serialSettingList[2].channel, serialSettingList[2].name, serialSettingList[2].status);
        u8g2_DrawStr(&u8g2, 1, 7, &msg[0]);
        sprintf(msg, "%s  %s %s", serialSettingList[3].channel, serialSettingList[3].name, serialSettingList[3].status);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "%s %s %s", serialSettingList[4].channel, serialSettingList[4].name, serialSettingList[4].status);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "%s %s %s", serialSettingList[5].channel, serialSettingList[5].name, serialSettingList[5].status);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else {}

    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdTransmissionSetting(uint8_t screenID)
{
    char msg[28];

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "TRANSMIT SETTING");
    if (1 == screenID){
        sprintf(msg, "START: %s", transmission.startTime);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "INTERVAL: %s", transmission.interval);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "HEADER: %s", transmission.headerInterval);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (2 == screenID){
        sprintf(msg, "URL: %s", transmission.URL);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "TCP: %s", transmission.tcp_ip);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
        sprintf(msg, "PORT: %s", transmission.port);
        u8g2_DrawStr(&u8g2, 1, 31, &msg[0]);
    } else if (3 == screenID){
        sprintf(msg, "USER: %s", transmission.useName);
        u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
        sprintf(msg, "FILE: %s", transmission.fileName);
        u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
    }
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdStorageSetting(void)
{
    char msg[28];

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "STORAGE SETUP");
    if (0 == u8usbStt)
        sprintf(msg, "USB: <DISABLED>");
    else
        sprintf(msg, "USB: <ENABLED>");
    u8g2_DrawStr(&u8g2, 1, 15, &msg[0]);
    if (0 == u8sdcardStt)
        sprintf(msg, "SDCARD: <DISABLED>");
    else
        sprintf(msg, "SDCARD: <ENABLED>");
    u8g2_DrawStr(&u8g2, 1, 23, &msg[0]);
    u8g2_DrawStr(&u8g2, 1, 31, "DELETE STORAGE DATA");
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdStorageTimeSetting(void){
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "STORAGE TIME");
    u8g2_DrawStr(&u8g2, 1, 15, "<3 MOUNTH>");
    u8g2_DrawStr(&u8g2, 1, 23, "<6 MOUNTH>");
    u8g2_DrawStr(&u8g2, 1, 31, "<FOREVER>");
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdUSBSetting(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "USB SETTING");
    u8g2_DrawStr(&u8g2, 1, 15, "<ENABLE>");
    u8g2_DrawStr(&u8g2, 1, 23, "<DISABLE>");
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdSDcardSetting(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "SDCARD SETTING");
    u8g2_DrawStr(&u8g2, 1, 15, "<ENABLE>");
    u8g2_DrawStr(&u8g2, 1, 23, "<DISABLE>");
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}

static void lcdDeleteSetting(void)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 1, 7, "DELETE DATA");
    u8g2_DrawStr(&u8g2, 1, 15, "<YES>");
    u8g2_DrawStr(&u8g2, 1, 23, "<NO>");
    u8g2_DrawButtonUTF8(&u8g2, 0, (u8cursor*8+6), U8G2_BTN_INV|U8G2_BTN_BW0, 144, 0, 0, "");
    u8g2_DrawHLine(&u8g2, 0, (u8cursor*8+7), 144);
    u8g2_SendBuffer(&u8g2);
}
