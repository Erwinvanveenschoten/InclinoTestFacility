/*
 * ADC.h
 *
 *  Created on: 6 Feb 2020
 *      Author: Kees
 */

#ifndef HEADER_ADC_H_
#define HEADER_ADC_H_

#include "Headers/tools.h"
#include <DAVE.h>

#define pi 3.14159265359
#define INCLINOMAXVOLTAGE 3.3
#define ADCBITS 12
#define ADCRES exp2(ADCBITS)

uint16_t readADC();
double ConvertToVoltage(uint16_t measuredValue);
double CalculateAngle(double voltage);

#endif /* HEADER_ADC_H_ */
