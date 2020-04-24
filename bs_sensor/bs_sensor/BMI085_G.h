/*
 * BMI085_G.h
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#ifndef BMI085_G_H_
#define BMI085_G_H_

#include "DAVE.h"

#define BMI085_CS_G_PIN	0
#define READMASK 0x80
#define BMI085_G_BUF_SIZE 7

#define REG_GYRO_BANDWIDTH 	0x10
#define GYRO_BANDWIDTH		0x02
#define BMI085_GYRO_REG_RANGE 		0x0F
#define BMI085_GYRO_RANGE			0x04
#define GYRO_X_LSB 0x02
#define GYRO_X_MSB 0x03
#define GYRO_Y_LSB 0x04
#define GYRO_Y_MSB 0x05
#define GYRO_Z_LSB 0x06
#define GYRO_Z_MSB 0x07

void BMI085_G_read(void);
void BMI085_G_store_buffer(void);

#endif /* BMI085_G_H_ */
