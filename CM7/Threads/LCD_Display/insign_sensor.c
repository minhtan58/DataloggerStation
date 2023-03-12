/*
 * insign_sensor.c
 *
 *  Created on: Nov 12, 2022
 *      Author: HuynhLD
 */

#include "i2c.h"
#include "insign_sensor.h"


static HAL_StatusTypeDef insignCheckCRC(uint16_t data, uint8_t expected);


void insignReadID(void)
{
    uint8_t data[2];
    uint8_t command[2];

    command[0] = (uint8_t)(SHTC3_CMD_READ_ID >> 8 );
    command[1] = (uint8_t)SHTC3_CMD_READ_ID;

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, command, 2, 100))
        Error_Handler();

    if (HAL_OK != HAL_I2C_Master_Receive(&hi2c4, SHTC3_ADDRESS, data, 2, 100))
        Error_Handler();

    uint16_t result = ((data[0] << 8) | data[1]);
    uint16_t id = result & SHTC3_PRODUCT_CODE_MASK;
    if (id != 0x0807)
        Error_Handler();
}

void insignWakeUp(void)
{
    uint8_t command[2];

    command[0] = (uint8_t)(SHTC3_CMD_WAKEUP >> 8);
    command[1] = (uint8_t)SHTC3_CMD_WAKEUP;

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, command, 2, 100))
        Error_Handler();
}

void insignSleep(void)
{
    uint8_t command[2];;

    command[0] = (uint8_t)(SHTC3_CMD_SLEEP >> 8);
    command[1] = (uint8_t)SHTC3_CMD_SLEEP;

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, command, 2, 100))
        Error_Handler();
}

void insignSoftReset(void)
{
    uint8_t command[2];

    command[0] = (uint8_t)(SHTC3_CMD_SW_RESET >> 8 );
    command[1] = (uint8_t)SHTC3_CMD_SW_RESET;

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, command, 2, 100))
        Error_Handler();
}

HAL_StatusTypeDef insignMeasurements(float *hum, float *temp)
{
    uint8_t result[6]; /* |T MSB|T LSB|T CRC|H MSB|H LSB|H CRC| */
    uint8_t command[2];

    command[0] = (uint8_t)(SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST >> 8);
    command[1] = (uint8_t)SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST;

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, command, 2, 100))
        return HAL_ERROR;

    if (HAL_OK != HAL_I2C_Master_Receive(&hi2c4, SHTC3_ADDRESS, result, 6, 100))
        return HAL_ERROR;

    uint16_t raw_hum = result[0] << 8 | result[1];
    uint16_t raw_temp = result[3] << 8 | result[4];

    *hum = (float)(raw_hum * 100.0) / 65535.0;
    *temp = ((float)(raw_temp * 175.0) / 65535.0) - 45.0;

    return HAL_OK;
}

HAL_StatusTypeDef insignMeasurementsModeLowPower(float *hum, float *temp)
{
    uint8_t result[6];
    uint8_t command[2];

    command[0] = ((SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST_LOW_POWER >> 8) & 0xFF);
    command[1] = (SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST_LOW_POWER & 0xFF);

    if (HAL_OK != HAL_I2C_Master_Transmit(&hi2c4, SHTC3_ADDRESS, (uint8_t*)&command[0], 2, 100))
        return HAL_ERROR;

    if (HAL_OK != HAL_I2C_Master_Receive(&hi2c4, SHTC3_ADDRESS, (uint8_t*)&result[0], 6, 100))
        return HAL_ERROR;

    uint16_t raw_hum = result[0] << 8 | result[1];
    uint16_t raw_temp = result[3] << 8 | result[4];

    if (HAL_OK != insignCheckCRC(raw_hum, result[2]))
        return HAL_ERROR;

    if (HAL_OK != insignCheckCRC(raw_temp, result[5]))
        return HAL_ERROR;

    *hum = (float)(raw_hum * 100.0) / 65535.0;
    *temp = ((float)(raw_temp * 175.0) / 65535.0) - 45.0;

    return HAL_OK;
}

static HAL_StatusTypeDef insignCheckCRC(uint16_t data, uint8_t expected)
{
    uint8_t value[2];
    uint8_t crc = 0xFF;
    uint8_t poly = 0x31;

    value[0] = (uint8_t)(data >> 8);
    value[1] = (uint8_t)data;

    for (uint8_t indi = 0; indi < 2; indi++) {
        crc ^= value[indi];
        for (uint8_t indj = 0; indj < 8; indj++) {
            if (crc & 0x80) {
                crc = (uint8_t)((crc << 1) ^ poly);
            } else {
                crc <<= 1;
            }
        }
    }
    if (expected ^ crc) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

