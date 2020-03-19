/*
 * LSM6DSO.c
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#include "LSM6DSO.h"
#include "BUS_IO_GP.h"
#include "stdio.h"
#include "delay.h"
#include "data_config.h"
#include "message_buffer.h"

static uint8_t rx_buf[LSM6DS0_BUF_SIZE];

static const uint8_t data_id[NROF_DATA] =
{
	TEMP_ID,
	GYRO_X_ID,
	GYRO_Y_ID,
	GYRO_Z_ID,
	ACC_X_ID,
	ACC_Y_ID,
	ACC_Z_ID,
};

static uint8_t tx_buf[LSM6DS0_BUF_SIZE] =
{
	OUT_TEMP_L 	| READMASK,
	OUT_TEMP_H 	| READMASK,
	OUTX_L_G	| READMASK,
	OUTX_H_G	| READMASK,
	OUTY_L_G	| READMASK,
	OUTY_H_G	| READMASK,
	OUTZ_L_G 	| READMASK,
	OUTZ_H_G	| READMASK,
	OUTX_L_A 	| READMASK,
	OUTX_H_A 	| READMASK,
	OUTY_L_A	| READMASK,
	OUTY_H_A	| READMASK,
	OUTZ_L_A 	| READMASK,
	OUTZ_H_A 	| READMASK,
	0xFF,
};

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;

void LSM6DSO_init (void)
{
	// activate accelero (high performance)
	uint8_t xl_init_tx[] = {CTRL1_XL, HIGH_PERF};
	BUS_IO_GP_reset(LSM6DS0_CS_PIN);
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( spi_handler, xl_init_tx, sizeof(xl_init_tx)/sizeof(xl_init_tx[0])))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	delay(1);
	BUS_IO_GP_set(LSM6DS0_CS_PIN);

	// activate gyroscope (high performance)
	uint8_t g_init_tx[] = {CTRL2_G, HIGH_PERF};
	BUS_IO_GP_reset(LSM6DS0_CS_PIN);
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( spi_handler, g_init_tx, sizeof(g_init_tx)/sizeof(g_init_tx[0])))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	delay(1);
	BUS_IO_GP_set(LSM6DS0_CS_PIN);

	// Manually disable I2C
	uint8_t I2C_disable_init_tx[] = {CTRL4_C, DIS_I2C};
	BUS_IO_GP_reset(LSM6DS0_CS_PIN);
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( spi_handler, I2C_disable_init_tx, sizeof(I2C_disable_init_tx)/sizeof(I2C_disable_init_tx[0])))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	BUS_IO_GP_set(LSM6DS0_CS_PIN);
}

void LSM6DSO_read (void)
{
	BUS_IO_GP_reset(LSM6DS0_CS_PIN);
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( spi_handler, tx_buf, rx_buf, LSM6DS0_BUF_SIZE )){}
}

void LSM6DSO_buf  (void)
{

	int i = 3;	// skip temperature sensor
	for (i ; i < LSM6DS0_BUF_SIZE; i = (i + 2) )
	{
		MESSAGE_t message =
		{
			.data = ((uint32_t)(rx_buf[i+1] << 8) | rx_buf[i]),	// concatenate MSB and LSB
			.data_id = data_id[i/2],							// ID of the data
			.ic_id = LMS6DSO_ID,								// ID of the IC
		};
		buffer_add_message(message);
	}
	buffer_signal_LSM6DSO_complete( );
}
