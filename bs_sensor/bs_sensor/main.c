/*
 * main.c
 *
 *  Created on: 2020 Feb 11 09:55:25
 *  Author: erwin
 */

#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "ITF.h"
/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

#define ITF_ENA 1

void BMI055_test(void);

static uint32_t counter=0;

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
		// place test code here
		delay(250);
		BMI055_test();
#endif
	}
}

void BMI055_test(void)
{
		const uint8_t NROF_CS 	= 15;
		const uint8_t ACC_CS  	= 3;
		const uint16_t CS_mask 	= ~(1 << (NROF_CS - (ACC_CS)));
		const uint16_t CS_reset	= 0xFFFF;
		const uint8_t READMASK	= 0x80;
		const uint8_t DUMMY		= 0xFF;

		const uint8_t REG_BGW_CHIPID	= 0x00;
		const uint8_t REG_PMU_BW		= 0x10;

		uint8_t tx[]=
		{
			REG_PMU_BW | READMASK,
			DUMMY,
		};
		uint8_t tx_size=sizeof(tx)/sizeof(tx[0]);
		uint8_t rx[tx_size];

		// set CS
		BUS_IO_Write(&IO_GA_8, CS_mask);

		SPI_MASTER_RXFIFO_DisableEvent( &SPI_MASTER_2, XMC_USIC_CH_RXFIFO_EVENT_STANDARD);

		// Transfer data
		if (SPI_MASTER_STATUS_SUCCESS == SPI_MASTER_Transfer(	&SPI_MASTER_2,
																tx,
																rx,
																tx_size))
		{
			// Wait while transmission is complete
			while (SPI_MASTER_2.runtime->tx_busy || SPI_MASTER_2.runtime->rx_busy){}
		}
		delay(2);

		BUS_IO_Write(&IO_GA_8, CS_reset);

		if(rx[1]!=0xFA)
		{
			counter++;
		}

		SPI_MASTER_RXFIFO_EnableEvent( &SPI_MASTER_2, XMC_USIC_CH_RXFIFO_EVENT_STANDARD);
}
