/*
 * serial_print.c
 *
 *  Created on: Feb 5, 2020
 *      Author: erwin
 */

#include "serial_print.h"

static const UART_t * handle = &UART_0;

/* Redirects printf to UART */
int _write(int file, uint8_t *buf, int nbytes)
{
    if(UART_Transmit(handle, buf, nbytes) == UART_STATUS_SUCCESS) {
       while(UART_0.runtime->tx_busy) {
        }
    }
    return nbytes;
}

/* redirects UART to getchar() */
int _read(int file, uint8_t *buf, int nbytes)
{
    if(UART_Receive(handle, buf, 1) != UART_STATUS_SUCCESS) {
      nbytes = 0;
    } else {
      nbytes = 1;
    }
	return nbytes;
}
