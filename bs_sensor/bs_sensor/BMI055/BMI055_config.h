/*
 * BMI055_config.h
 *
 *  Created on: Feb 17, 2020
 *      Author: erwin
 */

#ifndef BMI055_CONFIG_H_
#define BMI055_CONFIG_H_

#include "inclino_sens_board_config.h"
#include "Dave.h"
#include "data_config.h"

/***********************************************************
 * BMI055 configuration
 ***********************************************************/


// Configure which BMI055 is enabled for testing purposes
#define BMI055_0_ENA ISB_0_ENABLED
#define BMI055_1_ENA ISB_1_ENABLED
#define BMI055_2_ENA ISB_2_ENABLED
#define BMI055_3_ENA ISB_3_ENABLED
#define BMI055_4_ENA ISB_4_ENABLED
#define BMI055_5_ENA ISB_5_ENABLED
#define BMI055_6_ENA ISB_6_ENABLED
#define BMI055_7_ENA ISB_7_ENABLED

#define NROF_BMI055 	( 	BMI055_0_ENA + 	\
							BMI055_1_ENA + 	\
							BMI055_2_ENA +	\
							BMI055_3_ENA +	\
							BMI055_4_ENA +	\
							BMI055_5_ENA +	\
							BMI055_6_ENA + 	\
							BMI055_7_ENA )


/********************************************************************
 * BMI055 Data configuration
 ********************************************************************/

// Enable/disable data
#define BMI055_GYRO_ENA	1
#define BMI055_ACC_ENA	1
#define BMI055_TEMP_ENA	1

// Number of IMU data that has to be read
#if !BMI055_ACC_ENA || BMI055_TEMP_ENA

	#define NROF_BMI055_IMU_DATA	( BMI055_GYRO_ENA + \
								  	  1 )
#else

	#define NROF_BMI055_IMU_DATA	( BMI055_GYRO_ENA )

#endif


// BMI055 register size
#define BMI055_ACC_REGISTER_SIZE 4
#define BMI055_GYR_REGISTER_SIZE 8

// Number of registers per BMI055
#define NROF_BMI055_GYRO_REG 6
#define NROF_BMI055_ACC_REG  6
#define NROF_BMI055_TEMP_REG 1

// Buffer sizes of the different data
#define GYRO_BUFFER_SIZE 	BMI055_GYRO_ENA*NROF_BMI055_GYRO_REG 	+ 1
#define ACC_BUFFER_SIZE    (BMI055_ACC_ENA*NROF_BMI055_ACC_REG + \
							BMI055_TEMP_ENA*NROF_BMI055_TEMP_REG + 1)	// Take note: the temperature sensor register is located in the Accelerometer registers

// Buffer indices of BMI055
#define GYRO_X_LSB_BUFFER_INDEX 1
#define GYRO_X_MSB_BUFFER_INDEX 2
#define GYRO_Y_LSB_BUFFER_INDEX 3
#define GYRO_Y_MSB_BUFFER_INDEX 4
#define GYRO_Z_LSB_BUFFER_INDEX 5
#define GYRO_Z_MSB_BUFFER_INDEX 6

#define  ACC_X_LSB_BUFFER_INDEX 1
#define  ACC_X_MSB_BUFFER_INDEX 2
#define  ACC_Y_LSB_BUFFER_INDEX 3
#define  ACC_Y_MSB_BUFFER_INDEX 4
#define  ACC_Z_LSB_BUFFER_INDEX 5
#define  ACC_Z_MSB_BUFFER_INDEX 6

#if BMI055_ACC_ENA
#define   ACC_TEMP_BUFFER_INDEX 7
#else
#define ACC_TEMP_BUFFER_INDEX 1
#endif

/***********************************************************
 * BMI055 CS bus configuration
 ***********************************************************/
#define NROF_CS_PIN 16-1

#if BMI055_0_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_0_CS_ACC_PIN		0
#define BMI055_0_CS_GYRO_PIN	1
#endif

#if BMI055_1_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_1_CS_ACC_PIN		2
#define BMI055_1_CS_GYRO_PIN	3
#endif

#if BMI055_2_ENA == 1

// BMI055 Chip Select Pins
#define BMI055_2_CS_ACC_PIN		4
#define BMI055_2_CS_GYRO_PIN	5
#endif

#if BMI055_3_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_3_CS_ACC_PIN		6
#define BMI055_3_CS_GYRO_PIN	7
#endif

#if BMI055_4_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_4_CS_ACC_PIN		8
#define BMI055_4_CS_GYRO_PIN	9
#endif

#if BMI055_5_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_5_CS_ACC_PIN		10
#define BMI055_5_CS_GYRO_PIN	11
#endif

#if BMI055_6_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_6_CS_ACC_PIN		12
#define BMI055_6_CS_GYRO_PIN	13
#endif

#if BMI055_7_ENA == 1
// BMI055 Chip Select Pins
#define BMI055_7_CS_ACC_PIN		14
#define BMI055_7_CS_GYRO_PIN	15
#endif

/***********************************************************
 * BMI055 configuration data structure
 ***********************************************************/

typedef struct
{
	uint8_t id;
	uint16_t cs_acc;
	uint16_t cs_gyro;
}BMI055_conf_t;

static const BMI055_conf_t BMI055[] =
{
#if BMI055_0_ENA == 1
	{
		.id = BMI055_0_ID,
		.cs_acc = BMI055_0_CS_ACC_PIN,
		.cs_gyro = BMI055_0_CS_GYRO_PIN,
	},
#endif

#if BMI055_1_ENA == 1
	{
		.id = BMI055_1_ID,
		.cs_acc = BMI055_1_CS_ACC_PIN,
		.cs_gyro = BMI055_1_CS_GYRO_PIN,
	},
#endif

#if BMI055_2_ENA == 1
	{
		.id = BMI055_2_ID,
		.cs_acc = BMI055_2_CS_ACC_PIN,
		.cs_gyro = BMI055_2_CS_GYRO_PIN,
	},
#endif

#if BMI055_3_ENA == 1
	{
		.id = BMI055_3_ID,
		.cs_acc = BMI055_3_CS_ACC_PIN,
		.cs_gyro = BMI055_3_CS_GYRO_PIN,
	},
#endif

#if BMI055_4_ENA == 1
	{
		.id = BMI055_4_ID,
		.cs_acc = BMI055_4_CS_ACC_PIN,
		.cs_gyro = BMI055_4_CS_GYRO_PIN,
	},
#endif

#if BMI055_5_ENA == 1
	{
		.id = BMI055_5_ID,
		.cs_acc = BMI055_5_CS_ACC_PIN,
		.cs_gyro = BMI055_5_CS_GYRO_PIN,
	},
#endif

#if BMI055_6_ENA == 1
	{
		.id = BMI055_6_ID,
		.cs_acc = BMI055_6_CS_ACC_PIN,
		.cs_gyro = BMI055_6_CS_GYRO_PIN,
	},
#endif

#if BMI055_7_ENA == 1
	{
		.id = BMI055_7_ID,
		.cs_acc = BMI055_7_CS_ACC_PIN,
		.cs_gyro = BMI055_7_CS_GYRO_PIN,
	},
#endif
};

static const uint8_t BMI055_ACC_DATA[] =
{
	GYRO_X_ID,
	GYRO_Y_ID,
	GYRO_Z_ID,
};

static const uint8_t BMI055_GYRO_DATA[] =
{
	ACC_X_ID,
	ACC_Y_ID,
	ACC_Z_ID,
	TEMP_ID,
};


#endif /* BMI055_CONFIG_H_ */
