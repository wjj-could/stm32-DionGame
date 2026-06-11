#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

/* 蜂鸣器引脚定义 - PA0 */
#define BUZZER_PIN      GPIO_PIN_0
#define BUZZER_PORT     GPIOA

#define BUZZER_ON()     HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET)
#define BUZZER_OFF()    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET)

/* 音效类型 */
typedef enum {
    SOUND_JUMP = 0,     /* 跳跃音效 */
    SOUND_SCORE,        /* 得分提示音 */
    SOUND_DEATH         /* 失败音效 */
} SoundType;

void Buzzer_Init(void);
void Buzzer_PlaySound(SoundType sound);

#endif
