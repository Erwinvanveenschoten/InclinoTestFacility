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

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

static bool tick_update=false;

void tick_timer_ISR( void );

int main(void)
{
	DAVE_STATUS_t status;
	status = DAVE_Init();           /* Initialization of DAVE APPs  */

#ifdef ENABLE_UDP
	// init ethernet communication
	udp_initialize();
#endif
	spi_1_init();

	TIMER_Start (&TICK_TIMER);

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U){}
	}

	while(1U)
	{
		if (buffer_message_complete())
		{
			buffer_send();
		}
		if (tick_update)
		{
			tick_update=false;

			TIMER_Clear(&TIME_MEASUREMENT);
			TIMER_Start(&TIME_MEASUREMENT);

			// Trigger BMI055 transfer sequence
			BMI055_start_transfer_seq();

			// Trigger SCA103T transfer sequence
			SCA103T_start_adc_conv_seq();

			spi_1_start_transf_seq();
		}
	}
}

void tick_timer_ISR( void )
{
	tick_update=true;
}
