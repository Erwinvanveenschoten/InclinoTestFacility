/*
 * message_buffer.c
 *
 *  Created on: Mar 16, 2020
 *      Author: erwin
 */

#include "message_buffer.h"
#include "UDP.h"
#include "stdio.h"

MESSAGE_t message_buffer[BUFFER_SIZE];

static uint32_t write_index = 0;
/*static uint32_t read_index = 0;*/

static bool BMI055_complete = false;
static bool SCA103T_complete = false;

static void print_buffer( void );

void buffer_add_message(MESSAGE_t message)
{
	if (write_index < BUFFER_SIZE)
	{
		message_buffer[write_index] = message;
		write_index++;
	}
}
void buffer_signal_BMI055_complete( void )
{
	BMI055_complete = true;

	if (buffer_message_complete())
	{
		buffer_send();
	}
}

void buffer_signal_SCA103T_complete( void )
{
	SCA103T_complete = true;

	if (buffer_message_complete())
	{
		buffer_send();
	}

}

bool buffer_message_complete( void )
{
	return BMI055_complete && SCA103T_complete;
}

void buffer_send( void )
{
	send_messages((void*)message_buffer, sizeof(MESSAGE_t)*write_index, write_index);

	BMI055_complete = false;
	SCA103T_complete = false;
	write_index = 0;

	TIMER_Stop(&BMI055_TIME_MEASUREMENT);
	printf("Time to copy data: %f uSec\n\r", ((float)TIMER_GetTime(&BMI055_TIME_MEASUREMENT)/100.0));
}

void print_buffer( void )
{
	for (int i = 0; i < write_index; ++i)
	{
		printf("\n\r********************\n\r");
		printf("IC id   : %04d\n\r", (int)message_buffer[i].ic_id);
		printf("Data id : %04d\n\r", (int)message_buffer[i].data_id);
		printf("Data    : 0x%04X\n\r", (int)message_buffer[i].data);
	}
}
