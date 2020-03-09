/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "UDP.h"
#include <BMI055.h>
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

void tick_timer_ISR( void );

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

#ifdef ENABLE_UDP
  // init ethernet communication
  udp_initialize();
#endif

  if(status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {
    }
  }

  while(1U)
  {
  }
}

void tick_timer_ISR( void )
{
	printf("test\n\r");
//	// Trigger BMI055 transfer sequence
//	BMI055_start_transfer_seq();
//
//	// Trigger SCA103T softwarechannels
//	ADC_MEASUREMENT_ADV_SoftwareTrigger(&ADC_MEASUREMENT_ADV_0);
//	ADC_MEASUREMENT_ADV_SoftwareTrigger(&ADC_MEASUREMENT_ADV_1);
//	ADC_MEASUREMENT_ADV_SoftwareTrigger(&ADC_MEASUREMENT_ADV_2);

	// Trigger
}
