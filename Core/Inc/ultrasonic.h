#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

#define TRIG_PIN        GPIO_PIN_6
#define TRIG_PORT       GPIOA
#define ECHO_PIN        GPIO_PIN_7
#define ECHO_PORT       GPIOA

#define TRIG_ON()       HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET)
#define TRIG_OFF()      HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET)
#define ECHO_READ()     HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)

#define DISTANCE_JUMP_THRESHOLD   15

#define ULTRASONIC_INTERVAL       20

void Ultrasonic_Init(void);
unsigned char Ultrasonic_IsNear(void);
float Ultrasonic_GetDistance(void);

#endif
