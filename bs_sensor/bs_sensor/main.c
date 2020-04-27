/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "ITF.h"
#include "delay.h"
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

#define ITF_ENA 1

int main(void)
{
	DAVE_STATUS_t status;
	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	ITF_init();

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U){}
	}

	while(1U)
	{
#if ITF_ENA
		ITF_manage();
#else

#endif
	}
}
