/*
 * BUS_IO_GP.C
 *
 *  Created on: Mar 9, 2020
 *      Author: erwin
 */

#include "BUS_IO_GP.h"

BUS_IO_t * const BUS_IO_handler = &IO_GP;

void BUS_IO_GP_set( uint16_t bus_nr )
{
	uint16_t mask = BUS_IO_Read(BUS_IO_handler);
	mask |= (1 << (bus_nr));
	BUS_IO_Write(BUS_IO_handler,mask);
}

void BUS_IO_GP_reset( uint16_t bus_nr )
{
	uint16_t mask = BUS_IO_Read(BUS_IO_handler);
	mask &= ~(1 << (bus_nr));
	BUS_IO_Write(BUS_IO_handler,mask);
}
