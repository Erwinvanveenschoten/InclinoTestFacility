/*
 * data_config.h
 *
 *  Created on: Feb 17, 2020
 *      Author: erwin
 */

#ifndef DATA_CONFIG_H_
#define DATA_CONFIG_H_

#define GYRO_X_ID 	0
#define GYRO_Y_ID 	1
#define GYRO_Z_ID 	2
#define ACC_X_ID 	3
#define ACC_Y_ID 	4
#define ACC_Z_ID 	5
#define TEMP_ID 	6
#define INCL_A 		7
#define INCL_B 		8
#define VREF		9

#define BMI055_0_ID			1
#define BMI055_1_ID			2
#define BMI055_2_ID			3
#define BMI055_3_ID			4
#define BMI055_4_ID			5
#define BMI055_5_ID			6
#define BMI055_6_ID			7
#define BMI055_7_ID			8
#define SCA103T_0_ID		9
#define SCA103T_1_ID		10
#define SCA103T_2_ID		11
#define SCA103T_3_ID		12
#define SCA103T_4_ID		13
#define SCA103T_5_ID		14
#define SCA103T_6_ID		15
#define SCA103T_7_ID		16
#define BMI085_ID			17
#define LMS6DSO_ID			18
#define MS5611_01BA03_ID	19

typedef struct message
{
	uint32_t data;
	uint8_t data_id;
	uint8_t ic_id;
}MESSAGE_t;

#endif /* DATA_CONFIG_H_ */
