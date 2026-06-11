#ifndef __TM1637_H
#define __TM1637_H

#include "main.h"

#define TM1637_CLK_PIN    GPIO_PIN_8
#define TM1637_DIO_PIN    GPIO_PIN_9
#define TM1637_PORT       GPIOA

#define TM1637_CLK_HIGH() HAL_GPIO_WritePin(TM1637_PORT, TM1637_CLK_PIN, GPIO_PIN_SET)
#define TM1637_CLK_LOW()  HAL_GPIO_WritePin(TM1637_PORT, TM1637_CLK_PIN, GPIO_PIN_RESET)
#define TM1637_DIO_HIGH() HAL_GPIO_WritePin(TM1637_PORT, TM1637_DIO_PIN, GPIO_PIN_SET)
#define TM1637_DIO_LOW()  HAL_GPIO_WritePin(TM1637_PORT, TM1637_DIO_PIN, GPIO_PIN_RESET)
#define TM1637_DIO_READ() HAL_GPIO_ReadPin(TM1637_PORT, TM1637_DIO_PIN)

#define TM1637_CMD_DATA     0x40
#define TM1637_CMD_ADDR     0xC0
#define TM1637_CMD_DISPLAY  0x8F

#define TM1637_UPDATE_INTERVAL  10

typedef enum {
    TM1637_MODE_SCORE = 0,
    TM1637_MODE_HIGHSCORE,
    TM1637_MODE_TEMP
} TM1637_Mode;

void TM1637_Init(void);
void TM1637_DisplayNum(unsigned int num, unsigned char leading_zero);
void TM1637_DisplayTemp(unsigned char temp);
void TM1637_DisplayHighScore(unsigned int score);
void TM1637_SetBrightness(unsigned char brightness);
void TM1637_Blink(unsigned char on);
void TM1637_Clear(void);

#endif
