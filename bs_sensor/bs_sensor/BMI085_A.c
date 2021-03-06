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

void BMI085_A_write(uint8_t addr, uint8_t data);

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
	ACC_Z_MSB | READMASK, 	// 0x17
	0xFF,				  	// 0x18
	0xFF,					// 0x19
	0xFF,					// 0x1A
	0xFF,					// 0x1B
	0xFF,				  	// 0x1C
	0xFF,					// 0x1D
	0xFF,					// 0x1E
	0xFF,					// 0x20
	0xFF,					// 0x21
	TEMP_LSB | READMASK,
	TEMP_MSB | READMASK,
	0xFF,
	0xFF,
};

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;

void BMI085_A_init ( void )
{
	// SW reset
	BMI085_A_write(BMI085_A_RESET_REG,BMI085_A_RESET_CMD);
	delay(10000); // 10 ms

	// Dummy write to turn on the accelero part of the sensor
	BMI085_A_write(0xFF, 0xFF);

	delay(1000);	//Wait 1 mSec

	BMI085_A_write(ACC_PWR_CTRL, ACC_PWR_NORMAL);
	delay(50000);	//Wait 50 mSec
	BMI085_A_write(ACC_RANGE_CTRL, ACC_RANGE_2g);
	delay(50000);	//Wait 50 mSec
}

void BMI085_A_read ( void )
{
	while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	BUS_IO_GP_reset(BMI085_CS_A_PIN);
	delay(1);
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
			.data = ((uint32_t)(rx_buf[i+2] << 8) | rx_buf[i+1]),	// concatenate MSB and LSB
			.data_id = data_id[i/2],							// ID of the data
			.ic_id = BMI085_ID,									// ID of the IC
		};
		buffer_add_message(message);
	}

	// TEMPERATURE MESSAGE
	MESSAGE_t message =
	{
		.data = ((uint32_t)(rx_buf[BMI085_TEMP_MSB_INDEX]*8) + rx_buf[BMI085_TEMP_LSB_INDEX]/32),	// concatenate MSB and LSB
		.data_id = TEMP_ID,									// ID of the data
		.ic_id = BMI085_ID,									// ID of the IC
	};
	buffer_add_message(message);
	buffer_signal_BMI085_A_complete();
}

void BMI085_A_write(uint8_t addr, uint8_t data)
{
	uint8_t tx[]=
	{
		addr,
		data,
	};
	BUS_IO_GP_reset(BMI085_CS_A_PIN);
	if(SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( &SPI_MASTER_1, tx, sizeof(tx)/sizeof(tx[0]) ))
	{
		while (SPI_MASTER_1.runtime->rx_busy){}
	}
	delay(1);
	BUS_IO_GP_set(BMI085_CS_A_PIN);
}
