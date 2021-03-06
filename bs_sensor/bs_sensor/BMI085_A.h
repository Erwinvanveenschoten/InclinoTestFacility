/*
 * BMI085_A.h
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#ifndef BMI085_A_H_
#define BMI085_A_H_

#include "DAVE.h"

#define BMI085_A_BUF_SIZE 19
#define BMI085_A_BUF_LIM 7
#define READMASK 0x80
#define BMI085_TEMP_MSB_INDEX 18
#define BMI085_TEMP_LSB_INDEX 17

#define BMI085_A_RESET_CMD 0xB6
#define BMI085_A_RESET_REG 0x7E
#define ACC_PWR_CTRL	0x7D
#define ACC_PWR_NORMAL 	0x04
#define ACC_RANGE_CTRL	0x41
#define ACC_RANGE_2g	0x00
#define ACC_X_LSB		0x12
#define ACC_X_MSB		0x13
#define ACC_Y_LSB		0x14
#define ACC_Y_MSB		0x15
#define ACC_Z_LSB		0x16
#define ACC_Z_MSB		0x17
#define TEMP_LSB		0x22
#define TEMP_MSB		0x23
#define REG_GYRO_BANDWIDTH 	0x10
#define GYRO_BANDWIDTH		0x02

void BMI085_G_init(void);
void BMI085_A_init ( void );
void BMI085_A_read ( void );
void BMI085_A_store_buffer( void );

#endif /* BMI085_A_H_ */
