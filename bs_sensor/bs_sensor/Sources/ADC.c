#include "Headers/ADC.h"

void initADC(){
    ADC_MEASUREMENT_ADV_SoftwareTrigger(&ADC_MEASUREMENT_ADV_0);
}

uint16_t readADC(){

    uint16_t resultA, resultB;
    double differential, finalResult;

    resultA = ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_A);
    resultB = ADC_MEASUREMENT_ADV_GetResult(&ADC_MEASUREMENT_ADV_0_Channel_B);
    differential = ConvertToVoltage(resultA) - ConvertToVoltage(resultB);
    XMC_UNUSED_ARG(finalResult);
    return CalculateAngle(differential);
}

double ConvertToVoltage(uint16_t measuredValue){
    return INCLINOMAXVOLTAGE/ADCRES*measuredValue;
}

double CalculateAngle(double voltage){
    double offset = 0;
    double sensitivity = 280/15;
    double test = (voltage - offset)/sensitivity;
    return asin(test)*(180/pi);
}


