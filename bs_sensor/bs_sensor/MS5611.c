/*
 * MS5611.c
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#include "MS5611.h"
#include "BUS_IO_GP.h"
#include "stdio.h"
#include "delay.h"
#include "message_buffer.h"

typedef enum
{
	idle,
	read_temp,
	read_baro,
	complete,
}sensor_state_t;

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;
static uint16_t read_PROM( const uint8_t PROM_cmd );
static void start_conv ( uint8_t cmd);
static void reset_seq ( void );

static uint16_t coefficients[NROF_COEF];
static sensor_state_t current_state = idle;
static uint32_t adc_data;

void MS5611_start_press_conv( void )
{
	current_state = read_baro;
	start_conv(BARO_ADC_CONV);
}
void MS5611_start_temp_conv ( void )
{
	current_state = read_temp;
	start_conv(TEMP_ADC_CONV);
}

void MS5611_read_adc( void )
{
	BUS_IO_GP_reset(MS5611_CS_PIN);
	uint8_t adc_tx[] = {ADC_READ, 0xFF, 0xFF, 0xFF};
	uint8_t adc_rx[sizeof(adc_tx)/sizeof(adc_tx[0])];

	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( spi_handler, adc_tx, adc_rx, sizeof(adc_tx)/sizeof(adc_tx[0])))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	BUS_IO_GP_set(MS5611_CS_PIN);

	adc_data = (uint32_t)(adc_rx[1] << 16) | (uint32_t)(adc_rx[2] << 8) |adc_rx[3];
	printf("********************\n\r");
	printf("ADC data: 0x%08X\n\r", (int)adc_data);
}

void MS5611_init( void )
{
	reset_seq();
	// Read PROM
	uint8_t PROM_cmds[NROF_COEF] =
	{
		PROM_READ_SENS,
		PROM_READ_OFF,
		PROM_READ_TCS,
		PROM_READ_TCO,
		PROM_READ_TREF,
		PROM_READ_TEMPSENS,
	};

	printf("********************\n\r");
	for (int i = 0; i < NROF_COEF; ++i )
	{
		coefficients[i] = read_PROM(PROM_cmds[i]);
		printf("PROM value: 0x%04X\n\r", coefficients[i]);
	}
}

void MS5611_store_buffer( void )
{
	buffer_signal_MS5611_complete();
}

static uint16_t read_PROM( const uint8_t PROM_cmd )
{
	BUS_IO_GP_reset(MS5611_CS_PIN);
	uint8_t PROM_tx[] = {PROM_cmd, 0xFF, 0xFF};
	uint8_t PROM_rx[sizeof(PROM_tx)/sizeof(PROM_tx[0])];

	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( spi_handler, PROM_tx, PROM_rx,sizeof(PROM_tx)/sizeof(PROM_tx[0])))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	BUS_IO_GP_set(MS5611_CS_PIN);

	return (uint16_t)(PROM_rx[1] << 8) | PROM_rx[2];
}

static void start_conv ( uint8_t cmd )
{   // check for invalid conversion command
	if (cmd != TEMP_ADC_CONV && cmd != BARO_ADC_CONV )
	{
		return;
	}
	BUS_IO_GP_reset(MS5611_CS_PIN);

	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( spi_handler, &cmd ,sizeof(cmd)))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	delay(1);

	BUS_IO_GP_set(MS5611_CS_PIN);
}

static void reset_seq ( void )
{
	BUS_IO_GP_reset(MS5611_CS_PIN);

	uint8_t reset_tx = RESET_SEQ;
	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( spi_handler, &reset_tx ,sizeof(reset_tx)))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}

	// wait 2.8 mSec for reset to complete
	delay(2800);

	BUS_IO_GP_set(MS5611_CS_PIN);
}
