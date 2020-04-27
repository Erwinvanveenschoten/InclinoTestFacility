/*
 * LSM6DSO.h
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#ifndef LSM6DSO_H_
#define LSM6DSO_H_

#include "DAVE.h"

#define LSM6DS0_BUF_SIZE 	15
#define NROF_DATA			7
#define LSM6DS0_CS_PIN		7
#define READMASK 			0x80

#define OUT_TEMP_L 	0x20
#define OUT_TEMP_H 	0x21
#define OUTX_L_G	0x22
#define OUTX_H_G	0x23
#define OUTY_L_G	0x24
#define OUTY_H_G	0x25
#define OUTZ_L_G	0x26
#define OUTZ_H_G	0x27
#define OUTX_L_A	0x28
#define OUTX_H_A	0x29
#define OUTY_L_A	0x2A
#define OUTY_H_A	0x2B
#define OUTZ_L_A	0x2C
#define OUTZ_H_A	0x2D

#define CTRL1_XL 	0x10
#define CTRL2_G		0x11
#define HIGH_PERF	0xA0
#define CTRL3_C		0x12
#define IF_INC		0x04
#define CTRL4_C		0x13
#define DIS_I2C		0x04
#define CTRL2_G 	0x11
#define LSM6DSO_GYRO_BANDWIDTH 	0x80
#define LSM6DSO_GYRO_RANGE		0X02
#define LSM6DS0_XL_BANDWIDTH 	0x80
#define	LSM6DS0_XL_RANGE		0x00

void LSM6DSO_init (void);
void LSM6DSO_read (void);
void LSM6DSO_buf  (void);

#endif /* LSM6DSO_H_ */
