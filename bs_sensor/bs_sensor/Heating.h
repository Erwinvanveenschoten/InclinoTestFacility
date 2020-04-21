/*
 * Heating.h
 *
 *  Created on: 3 Apr 2020
 *      Author: Kees
 */

#ifndef HEATING_H_
#define HEATING_H_

#include "DAVE.h"

#define TEMPERATURE_INIT 30
#define IO_GP_HEAT_PIN 6
#define TEMP_HYSTERESIS 1

void manageTemperature(void);
void set_temperature(int32_t temperature);

#endif /* HEATING_H_ */
