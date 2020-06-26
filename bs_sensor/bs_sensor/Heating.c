#include "Heating.h"
#include "BUS_IO_GP.h"
#include "MS5611.h"
#include "UDP.h"
#include "Heating.h"

static int32_t temperature_setting = TEMPERATURE_INIT;
static int Kp = 6;
static int Ki = 0.48;
static int Kd = 10;
static int integral = 0;
static int derivative = 0;
static int last_error = 0;

void PIDController(){

	int currentTemp = getTempMS5611()/TEMPERATURE_SCALER;
	int error = temperature_setting - currentTemp;

	integral = integral + error;
	derivative = error - last_error;

	last_error = error;

	int pwm = (Kp * error) + (Ki * integral) + (Kd * error);

	if(pwm > 100){
		pwm = 100;
	}

	status = PWM_CCU4_SetDutyCycle(&PWM_CCU4_0, pwm*PWM_SCALER);

}

void manageTemperature(){



	if(currentTemp < (temperature_setting-TEMP_HYSTERESIS))
	{
		BUS_IO_GP_set(IO_GP_HEAT_PIN);
	}
	else
	{
		BUS_IO_GP_reset(IO_GP_HEAT_PIN);
	}
}

void set_temperature(int32_t temperature)
{
	temperature_setting=temperature;
}

