/*
 * inclino_sens_board.c
 *
 *  Created on: Feb 11, 2020
 *      Author: erwin
 */

#include "BMI055.h"
#include "BMI055_config.h"
#include "BMI055_defs.h"
#include "DAVE.h"
#include "stdio.h"
#include "UDP.h"
#include "data_config.h"
#include "BUS_IO_GP.h"
#include "message_buffer.h"
#include "delay.h"

#define MSEC_SCALE 100
#define BUS_IO_HEAT_PIN 6

#if NROF_BMI055 > 0

#if BMI055_GYRO_ENA || BMI055_ACC_ENA || BMI055_TEMP_ENA

typedef enum sensor
{
	GYROSCOPE 	= 0,
	ACCELERO 	= 1,
} sensor_t;

/******************************************************************************
 * 	Runtime Constants
 ******************************************************************************/
static uint8_t GYRO_addr[] =
{

	BMI055_GYRO_RATE_X_LSB | READ_MASK,
	BMI055_GYRO_RATE_X_MSB | READ_MASK,
	BMI055_GYRO_RATE_Y_LSB | READ_MASK,
	BMI055_GYRO_RATE_Y_MSB | READ_MASK,
	BMI055_GYRO_RATE_Z_LSB | READ_MASK,
	BMI055_GYRO_RATE_Z_MSB | READ_MASK,
	0XFF,
};

#if BMI055_ACC_ENA || BMI055_TEMP_ENA
static uint8_t ACC_addr[] =
	{
	#if	BMI055_ACC_ENA
		BMI055_ACCD_X_LSB | READ_MASK,
		BMI055_ACCD_X_MSB | READ_MASK,
		BMI055_ACCD_Y_LSB | READ_MASK,
		BMI055_ACCD_Y_MSB | READ_MASK,
		BMI055_ACCD_Z_LSB | READ_MASK,
		BMI055_ACCD_Z_MSB | READ_MASK,
	#endif
	#if BMI055_TEMP_ENA
		BMI055_ACCD_TEMP  | READ_MASK,
	#endif
		0XFF,
	};

static const sensor_t sensor_to_read[] =
{
#if BMI055_GYRO_ENA
		GYROSCOPE,
#endif
#if BMI055_ACC_ENA || BMI055_TEMP_ENA
		ACCELERO,
#endif
};

static TIMER_t * const timer_handl = &TIME_MEASUREMENT;
static const SPI_MASTER_t * const SPI_HANDLER = &SPI_MASTER_2;
static BUS_IO_t * const BUS_HANDLER = &IO_GA_8;

/******************************************************************************
 * 	Global Data buffers
 * 	(These are available by all the routines in this file. This is necessary
 * 	since the interrupt handlers don't take any parameters.)
 ******************************************************************************/

#if BMI055_GYRO_ENA > 0
static uint8_t GYRO_buffer[GYRO_BUFFER_SIZE];
#endif

#if BMI055_ACC_ENA || BMI055_TEMP_ENA
static uint8_t  ACC_buffer[ACC_BUFFER_SIZE];
#endif

#if TEMP_BUFFER_SIZE > 0
static uint8_t TEMP_buffer[TEMP_BUFFER_SIZE];
#endif

/******************************************************************************
 * 	Global counters
 * 	(These are available by all the routines in this file. This is necessary
 * 	since the interrupt handlers don't take any parameters.)
 ******************************************************************************/

static volatile bool BMI055_update=false;
static volatile uint32_t BMI055_index = 0;
static volatile uint32_t Data_index = 0;
static volatile bool init_complete = false;


/************************************************************************************
 * (Static) function declarations
 ************************************************************************************/

#if NROF_BMI055 > 0
void read_gyro(uint32_t bmi055_index);
void read_acc (uint32_t bmi055_index);
static void start_spi_transmission ( uint32_t bmi055_index, sensor_t sensor );
#endif

/************************************************************************************
 * (Static) variable declarations
 ************************************************************************************/

/************************************************************************************
 * API function Definitions
 ************************************************************************************/

void BMI055_init(void)
{
	while(SPI_MASTER_STATUS_SUCCESS != SPI_MASTER_SetBaudRate (&SPI_MASTER_2, 5000000)){}

	uint8_t gyro_init_tx[]=
	{
		BMI055_GYRO_BW_CTRL,
		BMI055_GYRO_BW_1000DPS,
		0x0F,//REG_RANGE
		0x04,//RANGE
	};
	uint8_t gyro_range_tx[]=
	{
		BMI055_GYRO_REG_RANGE,
		BMI055_GYRO_RANGE,
	};

	uint8_t acc_init_tx[]=
	{
		BMI055_ACCD_PMU_BW_CTRL,
		BMI055_ACCD_PMU_BW_1000Hz,
	};

	for (int i = 0; i < NROF_BMI055; i++)
	{
		/************************************************************************************
		 * Gyro
		 ************************************************************************************/
		// set active low chipselect
		uint16_t CS_mask = ~(1 << (BMI055[i].cs_gyro));
		BUS_IO_Write(BUS_HANDLER, CS_mask);

		// Gyro initialisation
		if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( SPI_HANDLER, gyro_init_tx, sizeof(gyro_init_tx)/sizeof(gyro_init_tx[0])))
		{
			// Wait while transmission is complete
			while (SPI_MASTER_2.runtime->tx_busy || SPI_MASTER_2.runtime->rx_busy){}
		}
		delay(1);
		// reset active low chipselect
		BUS_IO_Write(BUS_HANDLER, 0xFFFF);

		//	RANGE
		CS_mask = ~(1 << (BMI055[i].cs_gyro));
		BUS_IO_Write(BUS_HANDLER, CS_mask);

		// Gyro initialisation
		if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( SPI_HANDLER, gyro_range_tx, sizeof(gyro_range_tx)/sizeof(gyro_range_tx[0])))
		{
			// Wait while transmission is complete
			while (SPI_MASTER_2.runtime->tx_busy || SPI_MASTER_2.runtime->rx_busy){}
		}
		delay(1);
		// reset active low chipselect
		BUS_IO_Write(BUS_HANDLER, 0xFFFF);

		/************************************************************************************
		 * Accelero
		 ************************************************************************************/
		// set active low chipselect
		CS_mask = ~(1 << ((BMI055[i].cs_acc)));
		BUS_IO_Write(BUS_HANDLER, CS_mask);

		// acc initialisation
		if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transmit( SPI_HANDLER, acc_init_tx, sizeof(acc_init_tx)/sizeof(acc_init_tx[0])))
		{
			// Wait while transmission is complete
			while (SPI_MASTER_2.runtime->tx_busy || SPI_MASTER_2.runtime->rx_busy){}
		}
		delay(1);
		// reset active low chipselect
		BUS_IO_Write(BUS_HANDLER, 0xFFFF);
	}
	init_complete=true;
}

static void start_spi_transmission ( uint32_t bmi055_index, sensor_t sensor )
{
	switch (sensor)
	{
		case GYROSCOPE:
			read_gyro(bmi055_index);
			break;
		case ACCELERO:
			read_acc(bmi055_index);
			break;
		default:
			break;
	}
}

void read_gyro( uint32_t bmi055_index )
{
#if BMI055_GYRO_ENA
	// Slave select
	const uint16_t CS_mask = ~(1 << (BMI055[bmi055_index].cs_gyro));

	BUS_IO_Write(BUS_HANDLER, CS_mask);

	// Transfer data
	/*SPI_MASTER_STATUS_t status = */SPI_MASTER_Transfer(	SPI_HANDLER,
															GYRO_addr,
															GYRO_buffer,
															GYRO_BUFFER_SIZE);
#endif
}

void read_acc( uint32_t bmi055_index )
{
	// Slave select
	const uint16_t CS_mask = ~(1 << (BMI055[bmi055_index].cs_acc));

	BUS_IO_Write(BUS_HANDLER, CS_mask);

	// Transfer data
	SPI_MASTER_Transfer(
			SPI_HANDLER,
			ACC_addr,
			ACC_buffer,
			ACC_BUFFER_SIZE);
#endif
}

void store_buffer( uint32_t bmi055_index, uint32_t Data_index )
{
	switch (sensor_to_read[Data_index])
	{
		case GYROSCOPE: ;
#if BMI055_GYRO_ENA
		MESSAGE_t gyro_message[] =
		{
			{
				.data = ((uint16_t)GYRO_buffer[GYRO_X_MSB_BUFFER_INDEX] << BMI055_GYR_REGISTER_SIZE) 	// Most significant byte
						| (GYRO_buffer[GYRO_X_LSB_BUFFER_INDEX]),					// Least significant byte
				.data_id = GYRO_X_ID,
				.ic_id = BMI055[bmi055_index].id,
			},
			{
				.data = ((uint16_t)GYRO_buffer[GYRO_Y_MSB_BUFFER_INDEX] << BMI055_GYR_REGISTER_SIZE) 	// Most significant byte
						| (GYRO_buffer[GYRO_Y_LSB_BUFFER_INDEX]),			// Least significant byte
				.data_id = GYRO_Y_ID,
				.ic_id = BMI055[bmi055_index].id,
			},
			{
				.data = ((uint16_t)GYRO_buffer[GYRO_Z_MSB_BUFFER_INDEX] << BMI055_GYR_REGISTER_SIZE) 	// Most significant byte
						| (GYRO_buffer[GYRO_Z_LSB_BUFFER_INDEX]),			// Least significant byte
				.data_id = GYRO_Z_ID,
				.ic_id = BMI055[bmi055_index].id,
			},
		};
		for ( int i = 0; i < sizeof(gyro_message)/sizeof(gyro_message[0]); ++i)
		{
			buffer_add_message(gyro_message[i]);
		}

#endif
			break;
		case ACCELERO: ;
#if BMI055_ACC_ENA
			MESSAGE_t acc_message[] =
			{
				{
					.data = ((uint16_t)ACC_buffer[ACC_X_MSB_BUFFER_INDEX] << BMI055_ACC_REGISTER_SIZE) 		// Most significant byte
							| (ACC_buffer[ACC_X_LSB_BUFFER_INDEX] >> BMI055_ACC_REGISTER_SIZE),				// Least significant byte,
					.data_id = ACC_X_ID,
					.ic_id = BMI055[bmi055_index].id,
				},
				{
					.data =	((uint16_t)ACC_buffer[ACC_Y_MSB_BUFFER_INDEX] << BMI055_ACC_REGISTER_SIZE) 		// Most significant byte
							| (ACC_buffer[ACC_Y_LSB_BUFFER_INDEX] >> BMI055_ACC_REGISTER_SIZE),				// Least significant byte
					.data_id = ACC_Y_ID,
					.ic_id = BMI055[bmi055_index].id,
				},
				{
					.data = ((uint16_t)ACC_buffer[ACC_Z_MSB_BUFFER_INDEX] << BMI055_ACC_REGISTER_SIZE) 		// Most significant byte
							| (ACC_buffer[ACC_Z_LSB_BUFFER_INDEX] >> BMI055_ACC_REGISTER_SIZE),				// Least significant byte,
					.data_id = ACC_Z_ID,
					.ic_id = BMI055[bmi055_index].id,
				},
#endif
#if BMI055_TEMP_ENA
				{
					.data = ACC_buffer[ACC_TEMP_BUFFER_INDEX],
					.data_id = TEMP_ID,
					.ic_id = BMI055[bmi055_index].id,
				},
#endif
			};
			for ( int i = 0; i < sizeof(acc_message)/sizeof(acc_message[0]); ++i)
			{
				buffer_add_message(acc_message[i]);
			}
			break;
		default:
			break;
	}
}

#endif

#endif

void BMI055_eo_recieve(void)
{
#if NROF_BMI055 > 0

#if BMI055_GYRO_ENA || BMI055_ACC_ENA || BMI055_TEMP_ENA
	if (!init_complete)
	{
		return;
	}

	// Store the SPI buffer as a message
	store_buffer(BMI055_index, Data_index);

	//	When all registers of interest of the IC's are read, signal completion to buffer module
	if ( BMI055_index == (NROF_BMI055-1) && Data_index == (NROF_BMI055_IMU_DATA-1))
	{
		buffer_signal_BMI055_complete();
	}
	else
	{
		//update counters
		if (Data_index == (NROF_BMI055_IMU_DATA-1))
		{
			BMI055_index++;
			Data_index = 0;
		}
		else
		{
			Data_index++;
		}

		// start next spi transmission
		start_spi_transmission( BMI055_index, sensor_to_read[Data_index]);
	}
#endif
#endif
}

void BMI055_start_transfer_seq( void )
{
#if NROF_BMI055 > 0
#if BMI055_GYRO_ENA || BMI055_ACC_ENA || BMI055_TEMP_ENA

	// Wait while SPI_MASTER is busy
	while (SPI_MASTER_IsTxBusy(SPI_HANDLER) || SPI_MASTER_IsRxBusy(SPI_HANDLER) ){}

	// Reset counters
	BMI055_index = 0;
	Data_index = 0;

	// start transmission sequence
	start_spi_transmission(BMI055_index, sensor_to_read[Data_index]);
#endif
#endif
}

void BMI055_advance(void)
{
	// When the go ahead is given, start transfer sequence
	if (BMI055_update)
	{
		BMI055_start_transfer_seq();
		BMI055_update=false;
	}
}

void BMI055_signal_update(void)
{
	BMI055_update=true;
}
