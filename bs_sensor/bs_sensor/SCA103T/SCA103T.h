/*
 * SCA103T.h
 *
 *  Created on: Feb 27, 2020
 *      Author: erwin
 */

#ifndef SCA103T_SCA103T_H_
#define SCA103T_SCA103T_H_

#include "SCA103T_config.h"
#include "SCA103T_defs.h"

void SCA103T_ADC_ISR(void);

// start ADC conversion sequence manually (take note of oversampling in ADC MEASUREMENT ADC APP)
void SCA103T_start_adc_conv_seq(void);

// Periodically call in main loop. This advances ADC read sequence when timer ISR has signaled the go ahead
void SCA103T_advance(void);

// interrupt service routine to signal go ahead of ADC read sequence by means of timer
void SCA103T_update_ISR(void);

#endif /* SCA103T_SCA103T_H_ */
