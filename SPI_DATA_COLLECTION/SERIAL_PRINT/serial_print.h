/*
 * serial_print.h
 *
 *  Created on: Feb 5, 2020
 *      Author: erwin
 */

#ifndef HEADER_SERIAL_PRINT_H_
#define HEADER_SERIAL_PRINT_H_

#include "stdint.h"
#include "Dave.h"

int _write(int file, uint8_t *buf, int nbytes);
int _read(int file, uint8_t *buf, int nbytes);

#endif /* HEADER_SERIAL_PRINT_H_ */
