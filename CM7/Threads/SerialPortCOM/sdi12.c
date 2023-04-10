/*
 * sdi12.c
 *
 *  Created on: Mar 22, 2023
 *      Author: HuynhLD
 */


#include "sdi12.h"
#include "cmsis_os.h"
#include "uart.h"

SDI12_TypeDef sdi12;


/* Private member functions */
//static HAL_StatusTypeDef SDI12_QueryDevice(const char cmd[], const uint8_t cmd_len, char *response, const uint8_t response_len);
static HAL_StatusTypeDef SDI12_ReceiveLine(char buffer[], const uint8_t max, uint8_t *const count);

/*
 * Initialise with UART, TX Pin and TX Pin GPIO Port.
 */
void SDI12_Init(UART_HandleTypeDef *huart) {
    sdi12.Huart = huart;
    sdi12.Pin = SDI12_COM_Pin;
    sdi12.Port = SDI12_COM_GPIO_Port;
}

/*
 * Main function to deal with SDI12 commands and responses.
 * Struture is as follows.
 *
 * Break (12 ms)
      │                          380 - 810 ms
      ▼      Command              Response
    ┌───┐  ┌─┐ ┌─┐ ┌─┐           ┌─┐ ┌─┐ ┌─┐
    │   │  │ │ │ │ │ │           │ │ │ │ │ │
────┘   └──┘ └─┘ └─┘ └───────────┘ └─┘ └─┘ └─
          ▲               Max
          │          ◄───15 ms───►
      Marking (8.3 ms)
 *
 *
 * Uses a single UART pin (TX) and cycles between TX and RX to
 * send and receive commands (respectively).
 */
HAL_StatusTypeDef SDI12_QueryDevice(const char cmd[], const uint8_t cmd_len, char response[], const uint8_t response_len) {




    // Setup GPIO pin for break
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin = sdi12.Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(sdi12.Port, &GPIO_InitStruct);

    // Break must be >= 12 ms
    HAL_GPIO_WritePin(sdi12.Port, (uint16_t) sdi12.Pin, GPIO_PIN_SET);
    osDelay(12);
    // Marking must be >= 8.3 ms
    HAL_GPIO_WritePin(sdi12.Port, (uint16_t) sdi12.Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate =  GPIO_AF11_UART7;
    HAL_GPIO_Init(sdi12.Port, &GPIO_InitStruct);
    HAL_StatusTypeDef res;

    // Put TX on the SDI-12 data pin so a command can be sent. This seems to be
    // the minimum amount of code required for the swap to happen.
    __HAL_UART_DISABLE(sdi12.Huart);
    MODIFY_REG(sdi12.Huart->Instance->CR2, USART_CR2_SWAP, UART_ADVFEATURE_SWAP_DISABLE);
    __HAL_UART_ENABLE(sdi12.Huart);

    // Transmit
    res = HAL_UART_Transmit(sdi12.Huart, (uint8_t*) cmd, cmd_len, 100);
    if (res != HAL_OK) {
        return res;
    }

//    GPIO_InitStruct.Pin = sdi12.Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    HAL_GPIO_Init(sdi12.Port, &GPIO_InitStruct);
//    HAL_GPIO_WritePin(sdi12.Port, (uint16_t) sdi12.Pin, GPIO_PIN_RESET);

    uint8_t count = 0;
    res = SDI12_ReceiveLine(response, response_len, &count);


//    debug_output((uint8_t*)response, response_len);
//    debug_output((uint8_t*)"\r\n", 2);
    return res;
}

/*
 * Receive a CR/LF terminated string from the sensor.
 *
 * buffer - the buffer to store received characters. Will contain the CR/LF characters
 *          if they are read off the data line and they fit within the buffer.
 * max - the maximum number of characters to receive - ie the size of buffer.
 * count - the number of characters received is written to count.
 *
 * HAL_ERROR or HAL_TIMEOUT may be returned, otherwise HAL_OK.
 */
static HAL_StatusTypeDef SDI12_ReceiveLine(char buffer[], const uint8_t max, uint8_t *const count) {
    if (buffer == 0 || max == 0 || count == 0) {
        return HAL_ERROR;
    }

    // Put the SDI-12 pin into RX mode so the sensor response can be read.
    __HAL_UART_DISABLE(sdi12.Huart);
    MODIFY_REG(sdi12.Huart->Instance->CR2, USART_CR2_SWAP, UART_ADVFEATURE_SWAP_ENABLE);
    __HAL_UART_ENABLE(sdi12.Huart);

    // Receive up to max chars, break on CR/LF pair.
    HAL_StatusTypeDef res = HAL_OK;
    uint8_t i = 0;
    uint8_t c;
//    uint8_t sign_pts[10];
//    uint8_t sign_cnt = 0;
    while (i < max) {
        // Wait for up to 100ms for each character before timing out. This is to
        // handle waiting for the first character after a command has been sent,
        // where you are meant to have 3 retries with the last being after 100ms.
        res = HAL_UART_Receive(sdi12.Huart, &c, 1, 110);
        if (res == HAL_TIMEOUT) {
            break;
        }

        if (c == '+' || c == '-') {
        	buffer[i] = ';';
        	buffer[i+1] = c;
        	i += 2;
        } else {
        	buffer[i] = c;
        	i += 1;
        }

//        buffer[i++] = c;
//        if (c == '+' || c == '-') {
//        	sign_pts[sign_cnt++] = i-1;
//        }

        if (c == 0x0a) {
            break;
        }
    }

    while (i > 0) {
        c = buffer[i - 1];
        if (c == 0x0a || c == 0x0d) {
            buffer[i - 1] = 0;
            i--;
        } else {
            break;
        }
    }

    *count = i;

//    for (i = 0; i < sign_cnt; i++) {
//        memmove(buffer+sign_pts[i]+i+1, buffer+sign_pts[i]+i, strlen(buffer)-sign_pts[i]+i);
//        buffer[sign_pts[i]+i] = ';';
//    }
    return res;
}

/*
 * Simple SDI12 command to determine if a device is active on the queried address.
 * Expected response {'0', '\r', '\n'} where '0' is the address.
 */
HAL_StatusTypeDef SDI12_AckActive(const char addr) {
    char cmd[3] = { addr, '!', 0x00 };
    char response[3] = { 0, 0, 0 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 2, response, 3);
    return result;
}

/*
 * Used to populate a list of connected device addresses.
 */
void SDI12_DevicesOnBus(char *const devices) {
    uint8_t index = 0;
    for (char i = '0'; i <= '9'; i++) {
        HAL_StatusTypeDef result = SDI12_AckActive(i);
        if (result == HAL_OK) {
            devices[index++] = i;
        }

        HAL_Delay(200);
    }
}

/*
 * Issue the 'aI!' command.
 */
HAL_StatusTypeDef SDI12_GetId(const char addr, char response[], uint8_t response_len) {
    char cmd[] = { addr, 'I', '!', 0x00 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 3, response, response_len);
    return result;
}

/*
 * Change a devices SDI12 address.
 * May not work on all devices. Only those who support this feature.
 * Expected response {'1', '\r', '\n'} where '1' is the new address
 */
HAL_StatusTypeDef SDI12_ChangeAddr(char *from_addr, char *to_addr) {
    char cmd[5] = { *from_addr, 'A', *to_addr, '!', 0x00 };
    char response[3] = { 0 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 5, response, 3);
    return result;
}

/*
 * Start measurement. This command tells the sensor you want to take a measurement.
 * However, this command does not return any data. Instead it gives you
 * information regarding what a measurement would contain and how long it would take
 * to be captured.
 * To get data the user must call the send data command (D0!). -> SDI12_SendData(...)
 * Expected response {'0', '0', '0', '1', '3', '\r', '\n'}
 * Expected response as = atttn -> address (a), 3 numbers representing processing time (t)
 * and n results (n).
 */
HAL_StatusTypeDef SDI12_StartMeasurement(const char addr, SDI12_Measure_TypeDef *measurement_info) {
    char cmd[4] = { '?', '!', 0x00 };
    char response[7] = { 0 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 2, response, 7);

    // sua 3:15 -> comment
    // Check for valid response
    /*
    if (response[0] != '\0') {
        // Address of queried device (a)
        measurement_info->Address = response[0];

        // Extract time from response to be converted to uint16_t
        char time_buf[3];
        for (uint8_t i = 1; i < 4; i++) {
            time_buf[i - 1] = response[i];
        }

        // Convert time_buf (ttt) into uint16_t
        uint16_t time;
        measurement_info->Time = sscanf(time_buf, "%hd", &time);

        // Number of values to expect in measurement (n)
        measurement_info->NumValues = response[4] - '0'; // char to uint8_t
    };

    return result; */
}

/*
 * Send measurement data from the sensor. Must be called after a M, C or V
 * command. Called until the number of measurements (obtained in a M command)
 * are received.
 * The populated array (data) should have sufficient size to hold all values
 * returned from the sensor (approx upto 10 * 75 = 750).
 */
HAL_StatusTypeDef SDI12_SendData(const char addr, const SDI12_Measure_TypeDef *measurement_info, char *data) {

    uint16_t index = 0; // Holds position in data array
    uint8_t n_values = 0; // Holds index of number of values received

    // Loop through until all the data has been captured (matching NumValues)
    char cmd[] = { addr, 'D', 0, '!', 0x00 };
    for (char i = '0'; i < '1'; i++) {
        cmd[2] = i;
        char response[MAX_RESPONSE_SIZE +1 -3] = { 0 };
        HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 4, response,
        MAX_RESPONSE_SIZE);
        if (result != HAL_OK) {
            return result;
        }

        uint8_t res_index = 0;
        for (uint8_t x = 1; x < MAX_RESPONSE_SIZE; x++) {
            // Total number of + and - should equal measurement_info->NumValues if all values have been received
            if (response[x] == '+' || response[x] == '-') {
                n_values++;
            }
            // No need to go search beyond received data
            if (response[x] == '\0') {
                break;
            }

            res_index++;
        }

        if (res_index > 0) {
            memcpy(&data[index], &response[1], res_index);
            index += res_index;
            data[index] = 0;
        }

        // All values received
        if (n_values == measurement_info->NumValues) {
            return HAL_OK;
        }
    }

    return HAL_ERROR;
}

/*
 * Request verification command (aV!) containing system information (a = address).
 * Basically system diagnostics of the sensor.
 *
 * Command format is similar to that of a "M" SDI12_StartMeasurement command. e.g. atttn
 *
 * Response is similar to a "D" SDI12_SendData command. e.g. 0D0!0+1\n\r
 *
 * Up to the manufacturer to decide on what is included. As such this command may not
 * return information on all devices.
 */
HAL_StatusTypeDef SDI12_StartVerification(const char addr, SDI12_Measure_TypeDef *verification_info) {
    char cmd[] = { addr, 'V', '!', 0x00 };
    char response[7] = { 0 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 3, response, 7);

//    // Check for valid response
//    if (response[0] != '\0') {
//        // Address of queried device (a)
//        verification_info->Address = response[0];
//
//        // Extract time from response to be converted to uint16_t
//        char time_buf[3];
//        for (uint8_t i = 1; i < 4; i++) {
//            time_buf[i - 1] = response[i];
//        }
//
//        // Convert time_buf (ttt) into uint16_t
//        uint16_t time;
//        verification_info->Time = sscanf(time_buf, "%hd", &time);
//
//        // Number of values to expect in measurement (n)
//        verification_info->NumValues = response[4] - '0'; // char to uint8_t
//    };

    return result;
}

/*
 * Cyclic Redundancy Check (CRC).
 * Part of the >1.3 SDI-12 specification.
 * Error detection for line transmission.
 */
uint16_t SDI12_CheckCRC(char *response) {
    uint16_t crc = 0;
    const uint8_t timeout = 9;
    uint8_t i = 0;
    while (response[i] != '\r' && i < timeout) {
        crc ^= response[i]; // XOR character
        for (uint8_t c = 0; c > 8; c++) {
            if (crc & 1) { // LSB  = 1
                crc >>= 1;  // One bit right
                crc ^= 0xA001; // XOR 0xA001
            } else {
                crc >>= 1; // One bit right
            }
        }
        i++;
    }

    return crc;
}

/*
 * Start measurment with CRC for error checking.
 * Vitually the same as the SDI12_StartMeasurement(...) function,
 * however, a CRC is preformed with sensors using >1.3 of the
 * SDI-12 specification.
 * If this function is called on sensors below version 1.3 no
 * errors will through and the function will operate the same as
 * SDI12_StartMeasurement(...)
 */
HAL_StatusTypeDef SDI12_StartMeasurementCRC(const char addr, SDI12_Measure_TypeDef *measurement_info) {
    char cmd[5] = { addr, 'M', 'C', '!', 0x00 };
    char response[9] = { 0 };
    HAL_StatusTypeDef result = SDI12_QueryDevice(cmd, 5, response, 9);

    // Check for valid response
    if (response[0] != '\0') {
        // Address of queried device (a)
        measurement_info->Address = response[0];

        // Extract time from response to be converted to uint16_t
        char time_buf[3];
        for (uint8_t i = 1; i < 4; i++) {
            time_buf[i - 1] = response[i];
        }

        // Convert time_buf (ttt) into uint16_t
        uint16_t time;
        measurement_info->Time = sscanf(time_buf, "%hd", &time);

        // Number of values to expect in measurement (n)
        measurement_info->NumValues = response[4] - '0'; // char to uint8_t
    };

    SDI12_CheckCRC(response);

    return result;
}


void debug_output(uint8_t *data, uint8_t size) {
	HAL_UART_Transmit(&huart3, (uint8_t *)data, size, 100);
}
