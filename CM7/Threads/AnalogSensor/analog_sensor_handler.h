/*
 * analog_sensor_handler.h
 *
 *  Created on: Nov 17, 2022
 *      Author: minht
 */

#ifndef ANALOGSENSOR_ANALOG_SENSOR_HANDLER_H_
#define ANALOGSENSOR_ANALOG_SENSOR_HANDLER_H_

void AnalogSensor_Task(void const * argument);
void Analog_setup();
char* Get_AnalogData(void);
char* Get_AnalogName(void);
uint8_t lenHelper(uint32_t number);

#endif /* ANALOGSENSOR_ANALOG_SENSOR_HANDLER_H_ */
