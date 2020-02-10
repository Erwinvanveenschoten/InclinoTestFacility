/*
 * tools.c
 *
 *  Created on: 3 Feb 2020
 *      Author: Kees
 */

#include "Headers/tools.h"


void cycledelay(uint32_t cycles)
{
	volatile uint32_t i;

	for(i = 0UL; i < cycles ;++i)
	{
		__asm__("NOP");
	}
}
