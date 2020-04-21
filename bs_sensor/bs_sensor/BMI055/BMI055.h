/*
 * inclino_sens_board.h
 *
 *  Created on: Feb 11, 2020
 *      Author: erwin
 */

#ifndef BMI055_H_
#define BMI055_H_

#include <BMI055_defs.h>
#include "Dave.h"

#if NROF_BMI055 > 0

#endif

typedef struct BMI055_data
{
	uint8_t  sensor_id;
	uint16_t gyro_x;
	uint16_t gyro_y;
	uint16_t gyro_z;
	uint16_t acc_x;
	uint16_t acc_y;
	uint16_t acc_z;
	uint8_t  temp;
}BMI055_data_t;

void BMI055_start_transfer_seq( void );
void BMI055_init(void);

// periodically call BMI055_advance(), for example in main loop, to start BMI055 transfer sequence when timer ISR has given the go ahead
void BMI055_advance(void);

// call from timer ISR to signal the go ahead for starting the BMI055 transfer sequence
void BMI055_signal_update(void);

#endif /* BMI055_H_ */
