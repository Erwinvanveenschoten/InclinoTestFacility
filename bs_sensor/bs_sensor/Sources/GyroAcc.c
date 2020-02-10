/*
 * GyroAcc.cpp
 *
 *  Created on: 30 Jan 2020
 *      Author: Kees
 */

#include "Headers/GyroAcc.h"
#include "Headers/SPI.h"

uint8_t gyroDataRegister[6];
uint8_t accDataRegister[6];

void bmi055_start(void){
		SPI_MASTER_SetBaudRate(&SPI_MASTER_0, SPI_MASTER_0.config->channel_config->baudrate);
		XMC_GPIO_SetMode(BMI_CSA_PORT, BMI_CSA_PIN, BMI_CSA_MODE);
		XMC_GPIO_SetMode(BMI_CSG_PORT, BMI_CSG_PIN, BMI_CSG_MODE);

		for(int i = 0; i < 500000;i++){
			__asm__("nop");
		}

		XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);
		XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);

		spi_flushrxbuffer(&SPI_MASTER_0);

		memset(gyroDataRegister, 0, 6);
		memset(accDataRegister, 0, 6);
}

uint8_t readAcc(uint8_t read_register){
	read_register |= 0x80; //define that action is a read
	uint8_t readData = 0;
	uint16_t readRegister16 = (read_register << 8);

	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSA_PORT, BMI_CSA_PIN);
	SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&readRegister16, (uint8_t*)&readData, 1);//Receive response send zeros for clock
	XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);
	return readData;
}

uint8_t readGyro(uint8_t read_register){
	read_register |= 0x80; //define that action is a read
	uint8_t readData;
	uint16_t readRegister16 = (read_register << 8);

	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSG_PORT, BMI_CSG_PIN);
	SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&readRegister16, (uint8_t*)&readData, 1);//Receive response, send zeros for clock
	XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);

	return readData;
}

void writeAcc(uint8_t write_register, uint8_t write_data){
	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSA_PORT, BMI_CSA_PIN);
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&write_register, 1);//Send register which we want to write
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&write_data, 1);//Send data which we want to write
	XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);
}

void writeGyro(uint8_t write_register, uint8_t write_data){
	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSG_PORT, BMI_CSG_PIN);
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&write_register, 1);//Send register which we want to write
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&write_data, 1);//Send data which we want to write
	XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);
}

void readGyroData(){
	uint8_t read_register = 0x82; //define that action is a read
	uint8_t emptyline = 0x00;

	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSG_PORT, BMI_CSG_PIN);
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&read_register, 1);//Send register which we want to read
	for(int i = 0; i < 10;i++){
		asm("nop");
	}
	SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&emptyline, (uint8_t*)&gyroDataRegister, 6);//Receive response, send zeros for clock
	XMC_GPIO_SetOutputHigh(BMI_CSG_PORT, BMI_CSG_PIN);
}

void readAccData(){
	uint8_t read_register = 0x82; //define that action is a read
	uint8_t emptyline = 0x00;

	spi_flushrxbuffer(&SPI_MASTER_0);
	XMC_GPIO_SetOutputLow(BMI_CSA_PORT, BMI_CSA_PIN);
	SPI_MASTER_Transmit(&SPI_MASTER_0, (uint8_t *)&read_register, 1);//Send register which we want to read
	for(int i = 0; i < 5;i++){
		asm("nop");
	}
	SPI_MASTER_Transfer(&SPI_MASTER_0, (uint8_t *)&emptyline, (uint8_t*)&accDataRegister, 6);//Receive response, send zeros for clock
	XMC_GPIO_SetOutputHigh(BMI_CSA_PORT, BMI_CSA_PIN);
}



