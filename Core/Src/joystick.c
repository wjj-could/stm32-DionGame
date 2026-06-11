#include "joystick.h"
#include "adc.h"

static unsigned int Joystick_ReadADC(unsigned int channel)
{
     ADC1->SQR3 = channel;
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_ADON;
    while (!(ADC1->SR & ADC_SR_EOC));
    unsigned int val = ADC1->DR;
    return val;
}

void Joystick_Init(void)
{
}

JoystickAction Joystick_Read(void)
{
    static unsigned int tick = 0;
    static JoystickAction last_action = JOY_NONE;

    tick++;
    if (tick < JOY_READ_INTERVAL) return JOY_NONE;
    tick = 0;

    if (HAL_GPIO_ReadPin(JOY_PORT, JOY_SW_PIN) == GPIO_PIN_RESET)
    {
        if (last_action != JOY_PRESS)
        {
            last_action = JOY_PRESS;
            return JOY_PRESS;
        }
        return JOY_NONE;
    }

    unsigned int y_val = Joystick_ReadADC(JOY_ADC_CHANNEL_Y);

    if (y_val < JOY_THRESHOLD_UP)
    {
        if (last_action != JOY_UP)
        {
            last_action = JOY_UP;
            return JOY_UP;
        }
    }
    else if (y_val > JOY_THRESHOLD_DOWN)
    {
        last_action = JOY_DOWN;
        return JOY_DOWN;
    }
    else
    {
        last_action = JOY_NONE;
    }

    return JOY_NONE;
}

unsigned char Joystick_IsPressed(void)
{
    return (HAL_GPIO_ReadPin(JOY_PORT, JOY_SW_PIN) == GPIO_PIN_RESET) ? 1 : 0;
}
