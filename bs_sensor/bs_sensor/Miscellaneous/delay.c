/*
 * delay.c
 *
 *  Created on: Mar 13, 2020
 *      Author: erwin
 */

#include "delay.h"

static TIMER_t *const handler = &DELAY_TIMER;

#define TIMER_SCALE 100

void delay ( const uint32_t time_uSec )
{
	BUS_IO_Toggle(&GP_LED_BUS);
	uint32_t delay_cnt = time_uSec * TIMER_SCALE;
	TIMER_ClearEvent(handler);
	TIMER_SetTimeInterval( handler, delay_cnt);
	TIMER_Start(handler);

	while (!TIMER_GetInterruptStatus(handler)){}

	TIMER_Stop(handler);
	TIMER_Clear(handler);
}

void delay_timer_cb ( void )
{
}
