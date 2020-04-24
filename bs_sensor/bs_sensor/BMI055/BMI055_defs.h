/*
 * inclino_sensor_board_config.h
 *
 *  Created on: Feb 17, 2020
 *      Author: erwin
 */

#ifndef INCLINO_SENSOR_BOARD_CONFIG_H_
#define INCLINO_SENSOR_BOARD_CONFIG_H_

#include "BMI055_config.h"
#include "data_config.h"
#include "DAVE.h"

/***********************************************************
 * BMI055 registers
 ***********************************************************/

#define BMI055_GYRO_BW_CTRL 		0x10
#define BMI055_GYRO_BW_1000DPS 		0x02
#define BMI055_ACCD_PMU_BW_CTRL 	0x10
#define BMI055_ACCD_PMU_BW_1000Hz 	0x0F

#define BMI055_GYRO_REG_RANGE 		0x0F
#define BMI055_GYRO_RANGE			0x04
#define BMI055_GYRO_RATE_X_LSB 		0x02
#define BMI055_GYRO_RATE_X_MSB 		0x03
#define BMI055_GYRO_RATE_Y_LSB 		0x04
#define BMI055_GYRO_RATE_Y_MSB 		0x05
#define BMI055_GYRO_RATE_Z_LSB 		0x06
#define BMI055_GYRO_RATE_Z_MSB		0x07
#define BMI055_ACCD_X_LSB 			0x02
#define BMI055_ACCD_X_MSB 			0x03
#define BMI055_ACCD_Y_LSB 			0x04
#define BMI055_ACCD_Y_MSB 			0x05
#define BMI055_ACCD_Z_LSB 			0x06
#define BMI055_ACCD_Z_MSB 			0x07
#define BMI055_ACCD_TEMP			0x08

#define BMI055_RESET_CS 0xFFFF
#define READ_MASK 0x80


#endif /* INCLINO_SENSOR_BOARD_CONFIG_H_ */
