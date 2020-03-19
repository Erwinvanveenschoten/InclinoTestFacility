/*
 * message_buffer.h
 *
 *  Created on: Mar 16, 2020
 *      Author: erwin
 */

#ifndef BMI055_MESSAGE_BUFFER_H_
#define BMI055_MESSAGE_BUFFER_H_

#include "DAVE.h"
#include "data_config.h"

#define BUFFER_SIZE 100

typedef struct message
{
	uint32_t data;
	uint8_t data_id;
	uint8_t ic_id;
}MESSAGE_t;

void buffer_add_message(MESSAGE_t message);
bool buffer_message_complete( void );
void buffer_send( void );

void buffer_signal_BMI055_complete( void );
void buffer_signal_BMI085_A_complete( void );
void buffer_signal_BMI085_G_complete( void );
void buffer_signal_LSM6DSO_complete( void );
void buffer_signal_MS5611_complete( void );
void buffer_signal_SCA103T_complete( void );


#endif /* BMI055_MESSAGE_BUFFER_H_ */
