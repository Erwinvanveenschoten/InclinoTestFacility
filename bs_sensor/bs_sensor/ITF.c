/*
 * ITF.c
 *
 *  Created on: Apr 21, 2020
 *      Author: erwin
 */

#include "ITF.h"
#include "UDP.h"
#include <BMI055.h>
#include <SCA103T.h>
#include "spi_master_1.h"
#include "message_buffer.h"
#include "Heating.h"

void ITF_manage(void)
{
	if(buffer_message_complete())
	{
		buffer_send();
	}
	manageTemperature();
	SCA103T_advance();
	BMI055_advance();
	spi_1_advance();
}

void ITF_init(void)
{
	// init ethernet communication
	udp_initialize();
	spi_1_init();
	BMI055_init();

	// start timers to for timed sampling
	TIMER_Start (&TICK_TIMER);
	TIMER_Start (&TEMP_UPDATE_TIMER);
	TIMER_Start (&SCA103T_TIMER);
}

void tick_timer_ISR( void )
{
	//TIMER_Clear(&TIME_MEASUREMENT);
	//TIMER_Start(&TIME_MEASUREMENT);

	BMI055_signal_update();
	spi_1_update();
}
