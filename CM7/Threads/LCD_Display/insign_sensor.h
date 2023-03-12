#ifndef INSIGN_SENSOR_H_
#define INSIGN_SENSOR_H_

#include "stm32h7xx_hal.h"

#define SHTC3_ADDRESS_BASE    0x70
#define SHTC3_ADDRESS    (SHTC3_ADDRESS_BASE << 1)

#define SHTC3_PRODUCT_CODE_MASK    0x083F
#define SHTC3_SENSOR_ID_MASK     0xF7C0

#define SHTC3_CMD_SLEEP    0xB098
#define SHTC3_CMD_WAKEUP    0x3517
#define SHTC3_CMD_SW_RESET    0x805D
#define SHTC3_CMD_READ_ID    0xEFC8

#define SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST              0x5C24
#define SHTC3_CMD_CLK_STRETCH_READ_HUM_FIRST_LOW_POWER    0x44DE
#define SHTC3_CMD_CLK_STRETCH_READ_TEM_FIRST              0x7CA2
#define SHTC3_CMD_CLK_STRETCH_READ_TEM_FIRST_LOW_POWER    0x6458

#define SHTC3_CMD_POLL_READ_HUM_FIRST              0x58E0
#define SHTC3_CMD_POLL_READ_HUM_FIRST_LOW_POWER    0x401A
#define SHTC3_CMD_POLL_READ_TEM_FIRST              0x7866
#define SHTC3_CMD_POLL_READ_TEM_FIRST_LOW_POWER    0x609C

void insignReadID(void);
void insignWakeUp(void);
void insignSleep(void);
void insignSoftReset(void);
HAL_StatusTypeDef insignMeasurements(float *hum, float *temp);
HAL_StatusTypeDef insignMeasurementsModeLowPower(float *hum, float *temp);

#endif /* INSIGN_SENSOR_H_ */
