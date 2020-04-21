#include "Heating.h"
#include "BUS_IO_GP.h"
#include "MS5611.h"
#include "UDP.h"
#include "Heating.h"

static int32_t temperature_setting = TEMPERATURE_INIT;

void manageTemperature(){
	int currentTemp = getTempMS5611()/100;
	if((temperature_setting  - TEMP_HYSTERESIS) > currentTemp)
	{
		BUS_IO_GP_set(IO_GP_HEAT_PIN);
	}
	else if ((temperature_setting  + TEMP_HYSTERESIS) < currentTemp)
	{
		BUS_IO_GP_reset(IO_GP_HEAT_PIN);
	}
}

void set_temperature(int32_t temperature)
{
	temperature_setting=temperature;
}
