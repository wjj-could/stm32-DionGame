#ifndef __OLED091_H
#define __OLED091_H

#include "main.h"

#define OLED091_SCL_PIN    GPIO_PIN_8
#define OLED091_SDA_PIN    GPIO_PIN_9
#define OLED091_PORT       GPIOA

#define OLED091_SCL_H()  HAL_GPIO_WritePin(OLED091_PORT, OLED091_SCL_PIN, GPIO_PIN_SET)
#define OLED091_SCL_L()  HAL_GPIO_WritePin(OLED091_PORT, OLED091_SCL_PIN, GPIO_PIN_RESET)
#define OLED091_SDA_H()  HAL_GPIO_WritePin(OLED091_PORT, OLED091_SDA_PIN, GPIO_PIN_SET)
#define OLED091_SDA_L()  HAL_GPIO_WritePin(OLED091_PORT, OLED091_SDA_PIN, GPIO_PIN_RESET)

void OLED091_Init(void);
void OLED091_Clear(void);
void OLED091_ShowScore(unsigned int score);
void OLED091_ShowInfo(unsigned int score, unsigned int high_score, unsigned char temp, unsigned char blink);
void OLED091_ShowGameOver(unsigned int score);

#endif
