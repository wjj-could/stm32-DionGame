#ifndef __SENSOR_H
#define __SENSOR_H

#include "main.h"

/* ADC 句柄 (由 CubeMX 在 main.c 中生成) */
extern ADC_HandleTypeDef hadc1;

/* 光敏电阻阈值 */
#define LIGHT_THRESHOLD   500

/* ADC 通道定义 */
#define ADC_CHANNEL_LIGHT       ADC_CHANNEL_1   /* 光敏电阻 - PA1 */
#define ADC_CHANNEL_TEMP        ADC_CHANNEL_2   /* 热敏电阻 - PA2 */

void Sensor_Init(void);
unsigned int Sensor_ReadLight(void);
unsigned int Sensor_ReadTemp(void);
unsigned char IsDarkMode(void);

/**
 * 将 ADC 值转换为温度(°C)
 * 使用简化的 NTC 热敏电阻公式
 * 假设: 10K NTC, 10K 上拉分压, B=3950, 参考温度25°C
 */
float Sensor_GetTemperature(void);

#endif
