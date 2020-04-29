/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "ITF.h"
#include "delay.h"
#include "BUS_IO_GP.h"
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

void toggle_led(void);
DAVE_STATUS_t init(void);


int main(void)
{
	while(init() != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");
	}

	ITF_init();

	while(1U)
	{
		ITF_manage();
	}
}

void toggle_led(void)
{
	BUS_IO_Toggle(&GP_LED_BUS);

}

DAVE_STATUS_t init(void)
{
	DAVE_STATUS_t init_status;

	  init_status = DAVE_STATUS_SUCCESS;
	     /** @Initialization of APPs Init Functions */
	     init_status = (DAVE_STATUS_t)CLOCK_XMC4_Init(&CLOCK_XMC4_0);

	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_0 */
		 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_0);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_0 */
		 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_0);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_1 */
		 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_1);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of ADC_MEASUREMENT_ADV APP instance ADC_MEASUREMENT_ADV_2 */
		 init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_ADV_Init(&ADC_MEASUREMENT_ADV_2);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_1 */
		 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_1);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of SPI_MASTER APP instance SPI_MASTER_2 */
		 init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_MASTER_2);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of UART APP instance UART_0 */
		 init_status = (DAVE_STATUS_t)UART_Init(&UART_0);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of BUS_IO APP instance IO_GA_8 */
		 init_status = (DAVE_STATUS_t)BUS_IO_Init(&IO_GA_8);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of BUS_IO APP instance IO_GP */
		 init_status = (DAVE_STATUS_t)BUS_IO_Init(&IO_GP);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance TICK_TIMER */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&TICK_TIMER);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance TICK_TIMER_INTERRUPT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&TICK_TIMER_INTERRUPT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_0_RESULT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_0_RESULT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_1_RESULT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_1_RESULT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance ADC_MEASUREMENT_ADV_2_RESULT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ADC_MEASUREMENT_ADV_2_RESULT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance TIME_MEASUREMENT */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&TIME_MEASUREMENT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance DELAY_TIMER */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&DELAY_TIMER);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of BUS_IO APP instance GP_LED_BUS */
		 init_status = (DAVE_STATUS_t)BUS_IO_Init(&GP_LED_BUS);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance DELAY_TIMER_INTERRUPT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&DELAY_TIMER_INTERRUPT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance MS5611_TIMER */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&MS5611_TIMER);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance MS5611_TIMER_ISR */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&MS5611_TIMER_ISR);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance TEMP_UPDATE_TIMER */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&TEMP_UPDATE_TIMER);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance TEMP_UPDATE_INTERRUPT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&TEMP_UPDATE_INTERRUPT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of TIMER APP instance SCA103T_TIMER */
		 init_status = (DAVE_STATUS_t)TIMER_Init(&SCA103T_TIMER);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance SCA103T_TIMERINTERRUPT */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&SCA103T_TIMERINTERRUPT);
	   }
	  if (init_status == DAVE_STATUS_SUCCESS)
	  {
		 /**  Initialization of INTERRUPT APP instance TIME_MEASUREMENT_ISR */
		 init_status = (DAVE_STATUS_t)INTERRUPT_Init(&TIME_MEASUREMENT_ISR);
	   }
		// Disable reset of ethernet chip (active-low)
	  	const uint8_t PHY_RESET_PIN = 2;
	  	BUS_IO_GP_set(PHY_RESET_PIN);
		delay(1000000);
		if (init_status == DAVE_STATUS_SUCCESS)
		{
		 /**  Initialization of ETH_LWIP APP instance ETH_LWIP_0 */
			init_status = (DAVE_STATUS_t)ETH_LWIP_Init(&ETH_LWIP_0);
		}
		return init_status;
}
