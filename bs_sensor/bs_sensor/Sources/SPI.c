/*
 * SPI.c
 *
 *  Created on: 30 Jan 2020
 *      Author: Kees
 */
#include "Headers/SPI.h"

void spi_flushrxbuffer(const SPI_MASTER_t *const handle)
{
	/* Clear the receive FIFO */
	XMC_USIC_CH_RXFIFO_Flush(handle->channel);
	while(!SPI_MASTER_IsRxFIFOEmpty(handle)) { SPI_MASTER_GetReceivedWord(handle); };
}




