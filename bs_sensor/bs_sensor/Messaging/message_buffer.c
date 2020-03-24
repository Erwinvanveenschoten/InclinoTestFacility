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

static bool BMI055_complete 	= false;
static bool SCA103T_complete 	= false;
static bool BMI085_G_complete 	= false;
static bool BMI085_A_complete 	= false;
static bool LSM6DSO_complete 	= false;
static bool MS5611_complete 	= false;

//static void print_buffer( void );

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
}

void buffer_signal_SCA103T_complete( void )
{
	SCA103T_complete = true;
}

void buffer_signal_BMI085_A_complete( void )
{
	BMI085_A_complete = true;
}

void buffer_signal_BMI085_G_complete( void )
{
	BMI085_G_complete = true;
}
void buffer_signal_LSM6DSO_complete( void )
{
	LSM6DSO_complete = true;
}
void buffer_signal_MS5611_complete( void )
{
	MS5611_complete = true;
}

bool buffer_message_complete( void )
{
	return 	BMI055_complete && SCA103T_complete
			&& BMI085_G_complete && BMI085_A_complete
			&& LSM6DSO_complete/*&& MS5611_complete*/;
}

void buffer_send( void )
{
	BMI055_complete 	= false;
	SCA103T_complete 	= false;
	BMI085_G_complete 	= false;
	BMI085_A_complete 	= false;
	LSM6DSO_complete 	= false;
	MS5611_complete 	= false;
	send_messages((void*)message_buffer, sizeof(MESSAGE_t)*write_index, write_index);
	write_index = 0;

	TIMER_Stop(&TIME_MEASUREMENT);
	printf("Time to copy data: %f uSec\n\r", ((float)TIMER_GetTime(&TIME_MEASUREMENT)/100.0));
}

#ifdef PRINTF
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
#endif
