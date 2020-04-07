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
	conv_temp,
	conv_baro,
	conv_temp_complete,
	conv_baro_complete,

}sensor_state_t;

static const SPI_MASTER_t * const spi_handler = &SPI_MASTER_1;
static TIMER_t *const handle_ptr = &MS5611_TIMER;

static uint32_t D_temp=0;
static uint32_t D_baro=0;
static uint32_t currentTemp = 0;
static bool temp_updat=false;
static bool baro_updat=false;

static uint16_t read_PROM( const uint8_t PROM_cmd );
static void start_conv ( uint8_t cmd);
static void reset_seq ( void );
static void start_timer(void);
static int32_t process_temperature(void);
static int32_t process_pressure(void);

static uint64_t coefficients[NROF_COEF];
static sensor_state_t current_state = idle;

void MS5611_start_press_conv( void )
{
	current_state = conv_baro;
	start_conv(BARO_ADC_CONV);
}
void MS5611_start_temp_conv ( void )
{
	current_state = conv_temp;
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

	if( current_state==conv_temp_complete )
	{
		D_temp=(uint32_t)(adc_rx[1] << 16) | (uint32_t)(adc_rx[2] << 8) |adc_rx[3];
		temp_updat=true;
	}
	if( current_state==conv_baro_complete )
	{
		D_baro=(uint32_t)(adc_rx[1] << 16) | (uint32_t)(adc_rx[2] << 8) |adc_rx[3];
		baro_updat=true;
	}
	current_state = idle;
}

bool MS5611_temp_conv_complete(void)
{
	return (current_state == conv_temp_complete);
}

bool MS5611_baro_conv_complete(void)
{
	return (current_state==conv_baro_complete);
}

void MS5611_timer_ISR(void)
{
	// stop conversion timer
	TIMER_Stop(handle_ptr);

	if(conv_temp==current_state)
	{
		current_state=conv_temp_complete;
	}
	if(conv_baro==current_state)
	{
		current_state=conv_baro_complete;
	}
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

	for (int i = 0; i < NROF_COEF; ++i )
	{
		coefficients[i] = read_PROM(PROM_cmds[i]);
	}
}

void MS5611_store_buffer( void )
{
	if (temp_updat)
	{
		MESSAGE_t temp_message=
		{
			.data=process_temperature(),
			.ic_id=MS5611_ID,
			.data_id=TEMP_ID,
		};
		temp_updat=false;
		// process temperature
		buffer_add_message(temp_message);
	}

	if(baro_updat)
	{
		MESSAGE_t baro_message=
		{
			.data=process_pressure(),
			.ic_id=MS5611_ID,
			.data_id=BARO,
		};
		baro_updat=false;
		// process pressure
		buffer_add_message(baro_message);
	}
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

	// start conversion timer
	start_timer();

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

static void start_timer(void)
{
	TIMER_Clear(handle_ptr);
	TIMER_SetTimeInterval(handle_ptr, ADC_CONV_TIME*TIMER_SCALER);
	TIMER_Start(handle_ptr);
}

static int32_t process_temperature(void)
{
	int32_t dT=D_temp-coefficients[TREF_COEFFICIENT_INDEX]*TEMPERATURE_DIFFERENCE_WEIGHT;	// Temperature Difference
	int32_t returnValue = AMBIENT_TEMP_OFFSET + (dT*coefficients[TEMPSENS_COEFFICIENT_INDEX])/TEMPERATURE_COEFFICIENT_WEIGHT;
	currentTemp = returnValue;
	return returnValue;
}

static int32_t process_pressure(void)
{
	int64_t dT=D_temp-(coefficients[TREF_COEFFICIENT_INDEX]*TEMPERATURE_DIFFERENCE_WEIGHT);	// Difference between actual and reference temperature

	int64_t OFF=coefficients[OFF_COEFFICIENT_INDEX]*PRESSURE_OFFSET_WEIGHT+
			(coefficients[TCO_COEFFICIENT_INDEX]*dT)/TEMP_COEFF_PRESS_OFFSET_WEIGHT;

	int64_t SENS=coefficients[SENS_COEFFICIENT_INDEX]*PRESSURE_SENS_WEIGHT+		// Sensitivity at actual temperature
			(coefficients[TCS_COEFFICIENT_INDEX]*dT)/TEMP_COEFF_PRESS_SENS_WEIGHT;

	int32_t pressure = ((D_baro*(SENS/SENS_AT_TEMP_WEIGHT)-OFF)/COMPENS_PRESS_WEIGHT);//	Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)
	return (int32_t)pressure;
}

int getTempMS5611(){
	return currentTemp;
}
