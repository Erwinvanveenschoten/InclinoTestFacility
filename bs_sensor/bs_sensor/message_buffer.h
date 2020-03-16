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

void buffer_add_message(MESSAGE_t message);
void buffer_signal_BMI055_complete( void );
void buffer_signal_SCA103T_complete( void );
bool buffer_message_complete( void );
void buffer_send( void );


#endif /* BMI055_MESSAGE_BUFFER_H_ */
