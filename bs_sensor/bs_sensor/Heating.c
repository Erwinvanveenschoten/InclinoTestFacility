#include "Heating.h"
#include "BUS_IO_GP.h"
#include "MS5611.h"
#include "UDP.h"

void manageTemperature(int desiredTemp){
	int currentTemp = getTempMS5611()/100;
	desiredTemp = getDesiredTemp();
	if((desiredTemp + TEMP_HYSTERESIS) > currentTemp){
		BUS_IO_GP_set(6);//3.7 heat pin
	}
	else if((desiredTemp - TEMP_HYSTERESIS) < currentTemp){
		BUS_IO_GP_reset(6);//3.7 heat pin
	}
}
