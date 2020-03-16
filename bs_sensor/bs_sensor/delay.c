/*
 * delay.c
 *
 *  Created on: Mar 13, 2020
 *      Author: erwin
 */

#include "delay.h"

static TIMER_t *const handler = &DELAY_TIMER;

static bool timer_finished = false;

#define TIMER_SCALE 100

void delay ( uint32_t time_uSec )
{
	timer_finished = false;
	BUS_IO_Write(&GP_LED_BUS, 0xFF);
	TIMER_SetTimeInterval( handler, (time_uSec*TIMER_SCALE));
	TIMER_Start(handler);

	// Block until specified time has expired
	while (!timer_finished){}

	BUS_IO_Write(&GP_LED_BUS, 0x00);
}

void delay_timer_cb ( void )
{
	TIMER_Stop(handler);
	timer_finished = true;
}
