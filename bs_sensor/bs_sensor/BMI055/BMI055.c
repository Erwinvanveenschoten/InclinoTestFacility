/*
 * inclino_sens_board.c
 *
 *  Created on: Feb 11, 2020
 *      Author: erwin
 */

#include <BMI055.h>
#include "BMI055_config.h"
#include <BMI055_defs.h>
#include "DAVE.h"
#include "stdio.h"
#include "UDP.h"
#include "data_config.h"

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
static const sensor_t sensor_to_read[] =
{
#if BMI055_GYRO_ENA
		GYROSCOPE,
#endif
#if BMI055_ACC_ENA || BMI055_TEMP_ENA
		ACCELERO,
#endif
};
static const SPI_MASTER_t * const SPI_HANDLER = &SPI_MASTER_2;
static BUS_IO_t * const BUS_HANDLER = &IO_GA_8;

/******************************************************************************
 * 	Global Data buffers
 * 	(These are available by all the routines in this file. This is necessary
 * 	since the interrupt handlers don't take any parameters.)
 ******************************************************************************/
#if NROF_BMI055 > 0
static BMI055_data_t data_buffer[NROF_BMI055];
#endif

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
static volatile uint32_t BMI055_index = 0;
static volatile uint32_t Data_index = 0;


/************************************************************************************
 * (Static) function declarations
 ************************************************************************************/

#if NROF_BMI055 > 0
void read_gyro(uint32_t bmi055_index);
void read_acc (uint32_t bmi055_index);
static void start_spi_transmission ( uint32_t bmi055_index, sensor_t sensor );
static void BMI055_print_buffer ( void );
void send_buffer( void );
#endif

/************************************************************************************
 * (Static) variable declarations
 ************************************************************************************/

/************************************************************************************
 * API function Definitions
 ************************************************************************************/

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
	uint8_t addr[] =
	{

		BMI055_ACCD_X_LSB | READ_MASK,
		BMI055_ACCD_X_MSB | READ_MASK,
		BMI055_ACCD_Y_LSB | READ_MASK,
		BMI055_ACCD_Y_MSB | READ_MASK,
		BMI055_ACCD_Z_LSB | READ_MASK,
		BMI055_ACCD_Z_MSB | READ_MASK,

		0XFF,
	};
	// Slave select
	const uint16_t CS_mask = ~(1 << NROF_CS_PIN - (BMI055[bmi055_index].cs_gyro));

	BUS_IO_Write(BUS_HANDLER, CS_mask);

	// Transfer data
	SPI_MASTER_STATUS_t status = SPI_MASTER_Transfer(	SPI_HANDLER,
														addr,
														GYRO_buffer,
														GYRO_BUFFER_SIZE);
#endif
}

void read_acc( uint32_t bmi055_index )
{
#if BMI055_ACC_ENA || BMI055_TEMP_ENA
	uint8_t addr[] =
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
	// Slave select
	const uint16_t CS_mask = ~(1 << NROF_CS_PIN - (BMI055[bmi055_index].cs_acc));

	BUS_IO_Write(BUS_HANDLER, CS_mask);

	// Transfer data
	SPI_MASTER_Transfer(
			SPI_HANDLER,
			addr,
			ACC_buffer,
			ACC_BUFFER_SIZE);
#endif
}

void store_buffer( uint32_t bmi055_index, uint32_t Data_index )
{
	switch (sensor_to_read[Data_index])
	{
		case GYROSCOPE:
#if BMI055_GYRO_ENA
			data_buffer[bmi055_index].gyro_x =
					((uint16_t)GYRO_buffer[GYRO_X_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 	// Most significant byte
					| (GYRO_buffer[GYRO_X_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);			// Least significant byte

			data_buffer[bmi055_index].gyro_y =
					((uint16_t)GYRO_buffer[GYRO_Y_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 	// Most significant byte
					| (GYRO_buffer[GYRO_Y_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);			// Least significant byte

			data_buffer[bmi055_index].gyro_z =
					((uint16_t)GYRO_buffer[GYRO_Z_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 	// Most significant byte
					| (GYRO_buffer[GYRO_Z_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);			// Least significant byte
#endif
			break;
		case ACCELERO:
#if BMI055_ACC_ENA
			data_buffer[bmi055_index].acc_x =
					((uint16_t)ACC_buffer[ACC_X_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 		// Most significant byte
					| (ACC_buffer[ACC_X_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);				// Least significant byte

			data_buffer[bmi055_index].acc_y =
					((uint16_t)ACC_buffer[ACC_Y_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 		// Most significant byte
					| (ACC_buffer[ACC_Y_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);				// Least significant byte

			data_buffer[bmi055_index].acc_z =
					((uint16_t)ACC_buffer[ACC_Z_MSB_BUFFER_INDEX] << BMI055_REGISTER_SIZE) 		// Most significant byte
					| (ACC_buffer[ACC_Z_LSB_BUFFER_INDEX] >> BMI055_REGISTER_SIZE);				// Least significant byte
#endif
#if BMI055_TEMP_ENA
			data_buffer[bmi055_index].temp = ACC_buffer[ACC_TEMP_BUFFER_INDEX];
#endif
			break;
		default:
			break;
	}
	// before incrementing index, store sensor id
	data_buffer[bmi055_index].sensor_id = BMI055[bmi055_index].id;
}



void BMI055_print_buffer ( void )
{
	printf("\n\n");
	for (int i = 0; i < NROF_BMI055; i++)
	{
		// print BMI055 id
		printf("********************\n\r");
		printf("BMI055 %d\n\r", data_buffer[i].sensor_id);

		// print data
		printf("ACC_X:  0x%04X\n\r", data_buffer[i].acc_x);
		printf("ACC_Y:  0x%04X\n\r", data_buffer[i].acc_y);
		printf("ACC_Z:  0x%04X\n\r", data_buffer[i].acc_z);

		printf("GYRO_X: 0x%04X\n\r", data_buffer[i].gyro_x);
		printf("GYRO_Y: 0x%04X\n\r", data_buffer[i].gyro_y);
		printf("GYRO_Z: 0x%04X\n\r", data_buffer[i].gyro_z);

		printf("TEMP:   0x%04X\n\r", data_buffer[i].temp);
	}
}

void send_buffer( void )
{
#if BMI055_ACC_ENA | BMI055_GYRO_ENA
	for ( int i = 0; i < NROF_BMI055; i++)
	{
		const MESSAGE_t data_message[] =
		{
#if BMI055_ACC_ENA
			{
				.data = (uint32_t)data_buffer[i].acc_x,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
			{
				.data = (uint32_t)data_buffer[i].acc_y,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
			{
				.data = (uint32_t)data_buffer[i].acc_z,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
#endif
#if BMI055_GYRO_ENA
			{
				.data = (uint32_t)data_buffer[i].gyro_x,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
			{
				.data = (uint32_t)data_buffer[i].gyro_y,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
			{
				.data = (uint32_t)data_buffer[i].gyro_z,
				.data_id = ACC_X_ID,
				.ic_id = BMI055[i].id,
			},
		};
#endif
		const uint8_t SIZEOF_DATA_MESSAGE = (sizeof(data_message)/sizeof(data_message[0]));

		for ( int j = 0; j < SIZEOF_DATA_MESSAGE; j++)
		{
			udp_printStruct((void *)&data_message[j], sizeof(data_message[j]));
		}
	}
#endif
}

#endif

#endif

void BMI055_eo_recieve(void)
{
#if NROF_BMI055 > 0

#if BMI055_GYRO_ENA || BMI055_ACC_ENA || BMI055_TEMP_ENA

	// Reset chip select
	BUS_IO_Write(BUS_HANDLER, BMI055_RESET_CS);

	// Store the IMU buffer
	store_buffer(BMI055_index, Data_index);

	//	When all registers of interest of the IC's are read, transmit the data
	if ( BMI055_index == (NROF_BMI055-1) && Data_index == (NROF_BMI055_IMU_DATA-1))
	{
		// Transmit buffer
		send_buffer();
		BMI055_print_buffer();
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
