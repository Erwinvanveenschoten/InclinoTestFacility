/*
 * GyroAcc.h
 *
 *  Created on: 30 Jan 2020
 *      Author: Kees
 */

#ifndef SOURCE_GYROACC_H_
#define SOURCE_GYROACC_H_

#include <math.h>
#include <stdint.h>

#include <Headers/SPI.h>



#define BMI_CSA_PORT XMC_GPIO_PORT2
#define BMI_CSA_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSA_PIN 6

#define BMI_CSG_PORT XMC_GPIO_PORT5
#define BMI_CSG_MODE XMC_GPIO_MODE_OUTPUT_PUSH_PULL
#define BMI_CSG_PIN 6

	void bmi055_start(void);
	uint8_t readAcc(uint8_t read_register);
	uint8_t readGyro(uint8_t read_register);
	void writeAcc(uint8_t write_register, uint8_t write_data);
	void writeGyro(uint8_t write_register, uint8_t write_data);
	void readGyroData();
	void readAccData();



#endif /* SOURCE_GYROACC_H_ */
