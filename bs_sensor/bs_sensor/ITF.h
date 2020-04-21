/*
 * ITF.h
 *
 *  Created on: Apr 21, 2020
 *      Author: erwin
 */

#ifndef ITF_H_
#define ITF_H_

#include "Dave.h"

void tick_timer_ISR( void );
void ITF_init(void);
void ITF_manage(void);

#endif /* ITF_H_ */
