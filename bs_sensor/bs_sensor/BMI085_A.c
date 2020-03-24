/*
 * BMI085_A.c
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#include "BMI085_A.h"
#include "BUS_IO_GP.h"
#include "stdio.h"
#include "delay.h"
#include "message_buffer.h"
#include "data_config.h"

static uint8_t rx_buf[BMI085_A_BUF_SIZE];

static uint8_t data_id[] =
{
		ACC_X_ID,
		ACC_Y_ID,
		ACC_Z_ID,
};

static uint8_t tx_buf[BMI085_A_BUF_SIZE] =
{
	ACC_X_LSB | READMASK,
	ACC_X_MSB | READMASK,
	ACC_Y_LSB | READMASK,
	ACC_Y_MSB | READMASK,
	ACC_Z_LSB | READMASK,
	ACC_Z_MSB | READMASK,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	TEMP_LSB | READMASK,
	TEMP_MSB | READMASK,
	0xFF,
};

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;

void BMI085_A_init ( void )
{
	BUS_IO_GP_set(BMI085_CS_A_PIN);

	delay(1000);	//Wait 1 mSec

	uint8_t init_tx[] =
	{	ACC_PWR_CTRL,		// Power mode control register
		ACC_PWR_NORMAL,	};	// Data to select normal power mode

	BUS_IO_GP_reset(BMI085_CS_A_PIN);
	if(SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( &SPI_MASTER_1, init_tx, sizeof(init_tx)/sizeof(init_tx[0]) ))
	{
		while (SPI_MASTER_1.runtime->rx_busy){}
	}
	delay(1);
	BUS_IO_GP_set(BMI085_CS_A_PIN);

	delay(50000);	//Wait 50 mSec
}

void BMI085_A_read ( void )
{
	BUS_IO_GP_reset(BMI085_CS_A_PIN);
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( spi_handler, tx_buf, rx_buf, BMI085_A_BUF_SIZE )){}
	return;
}

void BMI085_A_store_buffer( void )
{
	// ACCELERO MESSAGE
	for (int i = 1; i < BMI085_A_BUF_LIM; i = (i + 2) )
	{
		MESSAGE_t message =
		{
			.data = ((uint32_t)(rx_buf[i+1] << 8) | rx_buf[i]),	// concatenate MSB and LSB
			.data_id = data_id[i/2],							// ID of the data
			.ic_id = BMI085_ID,									// ID of the IC
		};
		buffer_add_message(message);
	}

	// TEMPERATURE MESSAGE
	MESSAGE_t message =
	{
		.data = ((uint32_t)(rx_buf[BMI085_TEMP_MSB_INDEX] << 8) |
							rx_buf[BMI085_TEMP_LSB_INDEX]),	// concatenate MSB and LSB
		.data_id = TEMP_ID,									// ID of the data
		.ic_id = BMI085_ID,									// ID of the IC
	};
	buffer_add_message(message);
	buffer_signal_BMI085_A_complete();
}


