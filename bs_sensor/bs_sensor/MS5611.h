/*
 * MS5611.h
 *
 *  Created on: Mar 18, 2020
 *      Author: erwin
 */

#ifndef MS5611_H_
#define MS5611_H_

#include "DAVE.h"

#define MS5611_CS_PIN	1
#define READMASK 		0x80
#define MS5611_BUF_SIZE	1
#define NROF_COEF		6

#define RESET_SEQ			0x1E // Command to start reset sequence ( wait for 2.8 mSec afterwards)
#define ADC_READ 			0x00 // Command to Read ADC value (24-bit)
#define TEMP_ADC_CONV		0x58 // Command to start temperature conversion
#define BARO_ADC_CONV		0x48 // Command to start pressure conversion
#define PROM_READ_SENS		0xA2 // Command to read Pressure Sensitivity from PROM (16-bit)
#define PROM_READ_OFF		0xA4 // Command to read Pressure offset from PROM (16-bit)
#define PROM_READ_TCS		0xA6 // Command to read Temperature coefficient of pressure sensitivity from PROM (16-bit)
#define PROM_READ_TCO   	0xA8 // Command to read Temperature Coefficient of pressure offset from PROM (16-bit)
#define PROM_READ_TREF  	0xAA // Command to read Reference temperature from PROM (16-bit)
#define PROM_READ_TEMPSENS	0xAC // Command to read Temperature Coefficient of the temperature from PROM (16-bit)

void MS5611_start_press_conv( void ); // Start pressure conversion (8.22ms ADC CONVERSION time)
void MS5611_start_temp_conv ( void ); // Start temperature conversion (8.22ms ADC CONVERSION time)
void MS5611_read_adc 		( void ); // Read ADC value (make sure to wait until conversion is complete)
void MS5611_init			( void ); // Initialize MS5611 module by reading PROM registers
void MS5611_store_buffer 	( void ); // Store Temperature and Pressure Value in message buffer

#endif /* MS5611_H_ */
