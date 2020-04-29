/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "ITF.h"
#include "delay.h"
#include "LSM6DSO.h"
#include "BMI085_G.h"
#include "BMI085_A.h"
#include "BUS_IO_GP.h"
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
#define ITF_ENA 1
#define INIT_DELAY 1000000
#if !ITF_ENA
void test(void);
uint32_t counter = 0;
#endif

void toggle_led(void);

int main(void)
{


	while(DAVE_Init() != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");
	}

	ITF_init();
	while(1U)
	{
#if ITF_ENA
		ITF_manage();
#else
		test();
#endif
	}
}

#if !ITF_ENA
void test(void)
{

	while(SPI_MASTER_STATUS_SUCCESS != SPI_MASTER_SetBaudRate (&SPI_MASTER_1, 10000000)){}

	const uint8_t DUMMY = 0xFF;
	const uint8_t WHO_AM_I = 0x0F;
	const uint8_t CHIP_ID = 0x00;
	uint8_t tx[]=
	{
		WHO_AM_I | READMASK,
		DUMMY,
	};
	const uint8_t tx_size = sizeof(tx)/sizeof(tx[0]);
	uint8_t rx[tx_size];

	BUS_IO_GP_reset(LSM6DS0_CS_PIN);

	if( SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer( &SPI_MASTER_1, tx, rx, tx_size))
	{
		while (SPI_MASTER_1.runtime->tx_busy || SPI_MASTER_1.runtime->rx_busy){}
	}
	BUS_IO_GP_set(LSM6DS0_CS_PIN);
	if(0x6C != rx[1])
	{
		counter++;
	}


}
#else
#endif

void toggle_led(void)
{
	BUS_IO_Toggle(&GP_LED_BUS);

}
