/*
 * BUS_IO_GP.C
 *
 *  Created on: Mar 9, 2020
 *      Author: erwin
 */

#include "BUS_IO_GP.h"

BUS_IO_t * const BUS_IO_handler = &IO_GP;

#define BMI085_CS_A_PIN	8
#define BMI085_CS_G_PIN	0
#define LSM6DS0_CS_PIN	7
#define MS5611_CS_PIN	1

void BUS_IO_GP_set( uint16_t bus_nr )
{
	uint16_t mask = BUS_IO_Read(BUS_IO_handler);
	mask |= (1 << (bus_nr));
	BUS_IO_Write(BUS_IO_handler,mask);
}

void BUS_IO_GP_reset( uint16_t bus_nr )
{
	uint16_t mask = BUS_IO_Read(BUS_IO_handler);

	// reset all cs (low active)
	uint8_t cs_set_mask = mask |(1 << BMI085_CS_A_PIN)|
								(1 << BMI085_CS_G_PIN)|
								(1 << LSM6DS0_CS_PIN) |
								(1 << MS5611_CS_PIN);

	BUS_IO_Write(BUS_IO_handler,cs_set_mask);

	// Set pin accordingly
	mask &= ~(1 << (bus_nr));
	BUS_IO_Write(BUS_IO_handler,mask);
}
