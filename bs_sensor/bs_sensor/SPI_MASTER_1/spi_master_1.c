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
#include "delay.h"

typedef enum
{
	IDLE,
	READ_BMI085_A,
	READ_BMI085_G,
	READ_LSM6DSO,
	STORE_BUF_MS5611,
}spi_state_t;

static spi_state_t current_state = IDLE;
static bool spi_1_start_update=false;
static bool temp_update=false;
static bool baro_update=false;
static bool spi_transf_complete=false;

static void disable_cs( void );
void spi_1_update_state(void);


void spi_1_init( void )
{
	while(SPI_MASTER_STATUS_SUCCESS != SPI_MASTER_SetBaudRate (&SPI_MASTER_1, 4000000)){}
	disable_cs();

	delay(50);

	BMI085_G_init();
	BMI085_A_init();
	LSM6DSO_init();
	MS5611_init();
}

void spi_1_eo_transf_cb( void )
{
	disable_cs();
	spi_transf_complete=true;
	return;
}

void spi_1_start_transf_seq( void )
{
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

void spi_1_advance(void)
{
	if (spi_transf_complete)
	{
		spi_transf_complete=false;
		spi_1_update_state();
	}
	// if temp/baro conversion is complete, read adc
	else if(baro_update)
	{
		baro_update=false;
		MS5611_start_press_conv();
	}
	else if(MS5611_baro_conv_complete())
	{
		MS5611_read_adc();
	}
	else if (MS5611_temp_conv_complete())
	{
		MS5611_read_adc();
		baro_update=true;
	}
	else if (temp_update)
	{
		temp_update=false;
		MS5611_start_temp_conv();
	}
	else if(spi_1_start_update)
	{
		spi_1_start_update=false;
		spi_1_start_transf_seq();
	}
}

void spi_1_update(void)
{
	spi_1_start_update=true;
}

void spi_1_update_temp_ISR(void)
{
	temp_update=true;
}

void spi_1_update_state(void)
{
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
			current_state = STORE_BUF_MS5611;
			LSM6DSO_buf();
		case STORE_BUF_MS5611:
			MS5611_store_buffer();
			current_state = IDLE;
			break;
		default:
			break;
	}
}
