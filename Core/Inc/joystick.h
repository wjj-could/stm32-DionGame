#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "main.h"

#define JOY_VRX_PIN       GPIO_PIN_3
#define JOY_VRY_PIN       GPIO_PIN_4
#define JOY_SW_PIN        GPIO_PIN_5
#define JOY_PORT          GPIOA

#define JOY_ADC_CHANNEL_X ADC_CHANNEL_3
#define JOY_ADC_CHANNEL_Y ADC_CHANNEL_4

#define JOY_THRESHOLD_UP    1200
#define JOY_THRESHOLD_DOWN  2800

#define JOY_READ_INTERVAL   5

typedef enum {
    JOY_NONE = 0,
    JOY_UP,
    JOY_DOWN,
    JOY_PRESS
} JoystickAction;

void Joystick_Init(void);
JoystickAction Joystick_Read(void);
unsigned char Joystick_IsPressed(void);

#endif
