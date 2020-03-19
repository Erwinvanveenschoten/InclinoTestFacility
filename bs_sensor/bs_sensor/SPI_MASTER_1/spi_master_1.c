/*
 * spi_master_1.c
 *
 *  Created on: Mar 17, 2020
 *      Author: erwin
 */

#include "spi_master_1.h"
#include "BUS_IO_GP.h"
#include <DAVE.h>
#include "BMI085_A.h"
#include "BMI085_G.h"
#include "LSM6DSO.h"
#include "MS5611.h"

typedef enum
{
	IDLE,
	READ_BMI085_A,
	READ_BMI085_G,
	READ_LSM6DSO,
	READ_MS5611_TEMP,
	READ_MS5611_BARO,
}spi_state_t;

spi_state_t current_state = IDLE;

static void disable_cs( void );

void spi_1_init( void )
{
	disable_cs();
	BMI085_A_init();
	LSM6DSO_init();
	MS5611_init();
}

void spi_1_eo_transf_cb( void )
{	disable_cs();

	switch (current_state)
	{
		case READ_BMI085_A:
			current_state = READ_BMI085_G;
			BMI085_G_read();
			BMI085_A_store_buffer();
			break;
		case READ_BMI085_G:
			current_state = READ_LSM6DSO;
			LSM6DSO_read();
			BMI085_G_store_buffer();
			break;
		case READ_LSM6DSO:
			LSM6DSO_buf();
			current_state = IDLE;
			break;
		case READ_MS5611_TEMP:
			break;
		case READ_MS5611_BARO:
			break;
		default:
			break;
	}
	return;
}

void spi_1_start_transf_seq( void )
{
	if (current_state != IDLE)
	{	// SPI is still busy
		return;
	}

	current_state = READ_BMI085_A;
	BMI085_A_read();
}

static void disable_cs( void )
{
	BUS_IO_GP_set(BMI085_CS_A_PIN);
	BUS_IO_GP_set(BMI085_CS_G_PIN);
	BUS_IO_GP_set(LSM6DS0_CS_PIN );
	BUS_IO_GP_set(MS5611_CS_PIN  );
}

