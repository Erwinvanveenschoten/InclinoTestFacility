/*
 * BMI085_G.c
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#include "BMI085_G.h"
#include "BUS_IO_GP.h"
#include "stdio.h"
#include "message_buffer.h"
#include "data_config.h"

static uint8_t rx_buf[BMI085_G_BUF_SIZE];

static uint8_t data_id[] =
{
		GYRO_X_ID,
		GYRO_Y_ID,
		GYRO_Z_ID,
};

static uint8_t tx_buf[BMI085_G_BUF_SIZE] =
{
	GYRO_X_LSB | READMASK,
	GYRO_X_MSB | READMASK,
	GYRO_Y_LSB | READMASK,
	GYRO_Y_MSB | READMASK,
	GYRO_Z_LSB | READMASK,
	GYRO_Z_MSB | READMASK,
	0xFF,
};

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;

void BMI085_G_read(void)
{
	BUS_IO_GP_reset(BMI085_CS_G_PIN);

	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( spi_handler, tx_buf, rx_buf, BMI085_G_BUF_SIZE )){}
	return;
}

void BMI085_G_store_buffer(void)
{
	for (int i = 1; i < BMI085_G_BUF_SIZE; i = (i + 2) )
	{
		MESSAGE_t message =
		{
			.data = ((uint32_t)(rx_buf[i+1] << 8) | rx_buf[i]),	// concatenate MSB and LSB
			.data_id = data_id[i/2],							// ID of the data
			.ic_id = BMI085_ID,									// ID of the IC
		};
		buffer_add_message(message);
	}
	buffer_signal_BMI085_G_complete();
}
