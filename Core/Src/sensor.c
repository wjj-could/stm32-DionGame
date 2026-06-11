#include "sensor.h"
#include <math.h>

void Sensor_Init(void)
{
}

unsigned int Sensor_ReadChannel(unsigned int channel)
{
    ADC1->SQR3 = channel;
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_ADON;
    while (!(ADC1->SR & ADC_SR_EOC));
    unsigned int val = ADC1->DR;
    return val;
}

unsigned int Sensor_ReadLight(void)
{
    return Sensor_ReadChannel(ADC_CHANNEL_LIGHT);
}

unsigned char IsDarkMode(void)
{
    return (Sensor_ReadLight() < LIGHT_THRESHOLD) ? 1 : 0;
}

unsigned int Sensor_ReadTemp(void)
{
    return Sensor_ReadChannel(ADC_CHANNEL_TEMP);
}

float Sensor_GetTemperature(void)
{
    unsigned int adc_val = Sensor_ReadTemp();

    if (adc_val == 0 || adc_val >= 4095)
    {
        return 25.0f;
    }

    float resistance = 10000.0f * (float)adc_val / (4095.0f - (float)adc_val);
    if (resistance < 0.1f) resistance = 0.1f;

    float temp_k = 1.0f / (1.0f / 298.15f + logf(resistance / 10000.0f) / 3950.0f);
    return temp_k - 273.15f;
}
