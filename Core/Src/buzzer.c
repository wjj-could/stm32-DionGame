#include "buzzer.h"

/**
 * 蜂鸣器初始化
 */
void Buzzer_Init(void)
{
    BUZZER_OFF();
}

/**
 * 蜂鸣器非阻塞延时发声
 * freq_hz: 频率(Hz)
 * duration_ms: 持续时间(ms)
 */
static void Buzzer_Tone(unsigned int freq_hz, unsigned int duration_ms)
{
    unsigned int delay_us;
    unsigned int cycles;

    if (freq_hz == 0 || duration_ms == 0) return;

    delay_us = 1000000 / freq_hz / 2;  /* 半个周期微秒数 */
    cycles = (unsigned int)((unsigned long)duration_ms * 1000 / (delay_us * 2));

    for (unsigned int i = 0; i < cycles; i++)
    {
        BUZZER_ON();
        /* 粗略延时 */
        for (volatile unsigned int d = 0; d < delay_us; d++)
        {
            __NOP();
        }
        BUZZER_OFF();
        for (volatile unsigned int d = 0; d < delay_us; d++)
        {
            __NOP();
        }
    }
}

/**
 * 播放指定音效(会短暂阻塞，时长很短不影响游戏)
 */
void Buzzer_PlaySound(SoundType sound)
{
    switch (sound)
    {
    case SOUND_JUMP:
        /* 短促上升音: 从低到高 */
        Buzzer_Tone(400, 30);
        Buzzer_Tone(600, 20);
        Buzzer_Tone(800, 20);
        break;

    case SOUND_SCORE:
        /* 清脆双音 */
        Buzzer_Tone(1000, 30);
        BUZZER_OFF();
        for (volatile unsigned int d = 0; d < 20000; d++) { __NOP(); }
        Buzzer_Tone(1500, 30);
        break;

    case SOUND_DEATH:
        /* 下降低沉音 */
        Buzzer_Tone(600, 50);
        Buzzer_Tone(400, 50);
        Buzzer_Tone(200, 80);
        break;

    default:
        break;
    }
}
