/*
 * serial_print.c
 *
 *  Created on: Feb 5, 2020
 *      Author: erwin
 */

#include "serial_print.h"
#include "BUS_IO_GP.h"

static const UART_t * handle = &UART_0;

#define UART_DE_PIN 5
#define UART_RE_PIN 4

/* Redirects printf to UART */
int _write(int file, uint8_t *buf, int nbytes)
{
	BUS_IO_GP_set(UART_DE_PIN);
    if(UART_Transmit(handle, buf, nbytes) == UART_STATUS_SUCCESS) {
       while(UART_0.runtime->tx_busy) {
        }
    }
    return nbytes;
    BUS_IO_GP_reset(UART_DE_PIN);
}

/* redirects UART to getchar() */
int _read(int file, uint8_t *buf, int nbytes)
{
	BUS_IO_GP_set(UART_RE_PIN);
    if(UART_Receive(handle, buf, 1) != UART_STATUS_SUCCESS) {
      nbytes = 0;
    } else {
      nbytes = 1;
    }
	return nbytes;
	BUS_IO_GP_reset(UART_RE_PIN);
}
