/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "UDP.h"
#include <BMI055.h>
#include <SCA103T.h>
#include "BUS_IO_GP.h"
#include "spi_master_1.h"
#include "delay.h"
#include "stdio.h"
#include "message_buffer.h"
#include "MS5611.h"
#include "Heating.h"

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

static bool tick_update=false;
static bool temp_update=false;
static bool baro_update=false;
static bool SCA103T_update=false;

void tick_timer_ISR( void );
void SCA103T_update_ISR(void);

int main(void)
{
	DAVE_STATUS_t status;
	status = DAVE_Init();           /* Initialization of DAVE APPs  */

#ifdef ENABLE_UDP
	// init ethernet communication
	udp_initialize();
#endif
	spi_1_init();
	BMI055_init();

	TIMER_Start (&TICK_TIMER);
	TIMER_Start (&TEMP_UPDATE_TIMER);
	TIMER_Start (&SCA103T_TIMER);

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U){}
	}

	while(1U)
	{
		manageTemperature();
		if (buffer_message_complete())
		{
			buffer_send();
		}
		// if temp/baro conversion is complete, read adc
		if(baro_update)
		{
			baro_update=false;
			MS5611_start_press_conv();
		}
		if(MS5611_baro_conv_complete())
		{
			MS5611_read_adc();
		}
		if (MS5611_temp_conv_complete())
		{
			MS5611_read_adc();
			baro_update=true;
		}
		if (temp_update)
		{
			temp_update=false;
			MS5611_start_temp_conv();
		}
		if (tick_update)
		{
			tick_update=false;

			TIMER_Clear(&TIME_MEASUREMENT);
			TIMER_Start(&TIME_MEASUREMENT);

			// Trigger BMI055 transfer sequence
			BMI055_start_transfer_seq();

			// if temp update start update sequence
			spi_1_start_transf_seq();
		}
		if (SCA103T_update)
		{
			SCA103T_update=false;

			// Trigger SCA103T transfer sequence
			SCA103T_start_adc_conv_seq();
		}
	}
}

void tick_timer_ISR( void )
{
	tick_update=true;
}

void temp_update_ISR(void)
{
	temp_update=true;
}

void SCA103T_update_ISR(void)
{
	SCA103T_update=true;
}
