/*
 * BUS_IO_GP.h
 *
 *  Created on: Mar 9, 2020
 *      Author: erwin
 */

#ifndef BUS_IO_GP_H_
#define BUS_IO_GP_H_

#include "Dave.h"

#define BMI085_CS_A_PIN	8
#define BMI085_CS_G_PIN	0
#define LSM6DS0_CS_PIN	7
#define MS5611_CS_PIN	1

void BUS_IO_GP_set( uint16_t bus_nr );
void BUS_IO_GP_reset( uint16_t bus_nr );

#endif /* BUS_IO_GP_H_ */
