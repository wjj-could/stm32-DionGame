/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled12864.h"
#include "buzzer.h"
#include "sensor.h"
#include "adc.h"
#include "joystick.h"
#include "ultrasonic.h"
#include "oled091.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCORE_SOUND_STEP    100
#define SENSOR_READ_PERIOD  30
#define BIRD_PROBABILITY    30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int get_key_val(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==0)
	{
		return 2;
	}
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==0)
	{
		return 1;
	}
	return 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	unsigned char key_num = 0;
	unsigned char cactus_category = 0;
	unsigned char cactus_length = 8;
	unsigned int score = 0;
	unsigned int highest_score = 0;
	int height = 0;
	int cactus_pos = 128;
	int bird_pos = 128;
	unsigned char cur_speed = 30;
	char failed = 0;
	char reset = 0;
	char is_bird = 0;
	char is_duck = 0;

	unsigned int score_milestone = SCORE_SOUND_STEP;
	unsigned int frame_count = 0;
	unsigned char dark_mode = 0;
	unsigned char last_dark_mode = 0;
	float temperature = 25.0f;
	unsigned char temp_bonus = 0;
	unsigned char death_sound_played = 0;
	unsigned char oled_blink_tick = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  Buzzer_Init();
  Sensor_Init();

  dark_mode = IsDarkMode();
  OLED_DisplayInvert(!dark_mode);
  last_dark_mode = dark_mode;

  Joystick_Init();
  Ultrasonic_Init();
  OLED091_Init();

  OLED_ShowString(0,0,"Test",4);
	OLED_DrawCover();

	while(get_key_val()!=2 && Joystick_IsPressed()==0)
	{
		unsigned char dm = IsDarkMode();
		if (dm != dark_mode)
		{
			OLED_DisplayInvert(!dm);
			dark_mode = dm;
			last_dark_mode = dm;
		}
	}
	HAL_Delay(100);
	OLED_Clear();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */


		if (failed == 1)
		{
			OLED_DrawRestart();

			if (!death_sound_played)
			{
				Buzzer_PlaySound(SOUND_DEATH);
				death_sound_played = 1;
			}

			if (score > highest_score)
			{
				highest_score = score;
				OLED091_ShowGameOver(score);
			}

			JoystickAction joy_act = Joystick_Read();
			key_num = get_key_val();

			if (key_num == 2 || joy_act == JOY_PRESS)
			{
				score = 0;
				score_milestone = SCORE_SOUND_STEP;
				failed = 0;
				death_sound_played = 0;
				height = 0;
				reset = 1;
				is_bird = 0;
				is_duck = 0;
				temp_bonus = 0;
				frame_count = 0;
				oled_blink_tick = 0;
				OLED091_ShowScore(0);
				OLED_DrawDinoJump(reset);
				OLED_DrawCactusRandom(cactus_category, reset);
				OLED_DrawBird(1);
				OLED_Clear();
			}
			continue;
		}


		score ++;

		if (score >= score_milestone)
		{
			Buzzer_PlaySound(SOUND_SCORE);
			score_milestone += SCORE_SOUND_STEP;
		}

		if (height <= 0 && !is_duck)
		{
			key_num = get_key_val();

			JoystickAction joy_act = Joystick_Read();
			if (joy_act == JOY_UP) key_num = 1;

			if (Ultrasonic_IsNear()) key_num = 1;

			if (joy_act == JOY_DOWN)
			{
				key_num = 0;
				is_duck = 1;
			}
			else
			{
				is_duck = 0;
			}
		}
		else if (is_duck && height <= 0)
		{
			JoystickAction joy_act2 = Joystick_Read();
			if (joy_act2 != JOY_DOWN)
			{
				is_duck = 0;
			}
		}

		if (key_num == 1 && height <= 0)
		{
			Buzzer_PlaySound(SOUND_JUMP);
		}

		OLED_DrawGround();
		OLED_DrawCloud();

		if (height>0 || key_num == 1) height = OLED_DrawDinoJump(reset);
		else if (is_duck) OLED_DrawDinoDuck();
		else OLED_DrawDino();

		if (is_bird == 0)
		{
			cactus_pos = OLED_DrawCactusRandom(cactus_category, reset);
			if(cactus_category == 0) cactus_length = 8;
			else if(cactus_category == 1) cactus_length = 16;
			else cactus_length = 24;

			if (cactus_pos + cactus_length < 0)
			{
				if ((rand() % 100) < BIRD_PROBABILITY)
				{
					is_bird = 1;
					OLED_DrawBird(1);
					bird_pos = 128;
				}
				else
				{
					cactus_category = rand()%4;
					OLED_DrawCactusRandom(cactus_category, 1);
				}
			}
		}

		if (is_bird == 1)
		{
			bird_pos = OLED_DrawBird(0);
			OLED_DrawCactusRandom(cactus_category, 1);
			if (bird_pos + 16 < 0)
			{
				is_bird = 0;
				OLED_DrawBird(1);
				cactus_category = rand()%4;
				OLED_DrawCactusRandom(cactus_category, 1);
			}
		}

		if (is_bird == 0)
		{
			if ((height < 16) && ((cactus_pos>=16 && cactus_pos <=32) || (cactus_pos + cactus_length>=16 && cactus_pos + cactus_length <=32)))
			{
				failed = 1;
				continue;
			}
		}
		else
		{
			if (!is_duck && ((bird_pos>=16 && bird_pos <=32) || (bird_pos + 16>=16 && bird_pos + 16 <=32)))
			{
				failed = 1;
				continue;
			}
		}

		/* ---- 定期读取传感器(每 SENSOR_READ_PERIOD 帧) ---- */
		if (frame_count % SENSOR_READ_PERIOD == 0)
		{
			dark_mode = IsDarkMode();
			if (dark_mode != last_dark_mode)
			{
				OLED_DisplayInvert(!dark_mode);
				last_dark_mode = dark_mode;
			}

			temperature = Sensor_GetTemperature();

			if (temperature > 30.0f)
			{
				temp_bonus = (unsigned char)((temperature - 30.0f) / 5.0f);
				if (temp_bonus > 10) temp_bonus = 10;
			}
			else
			{
				temp_bonus = 0;
			}
		}

		OLED_ShowString(48, 0, "HI:", 12);
		OLED_ShowNum(71, 0, highest_score, 5, 12);
		OLED_ShowNum(111, 0, score, 5, 12);

		{
			unsigned char temp_display = (unsigned char)(temperature + 0.5f);
			OLED_ShowString(0, 0, "T:", 12);
			OLED_ShowNum(12, 0, temp_display, 2, 12);
			OLED_ShowString(24, 0, "C", 12);
		}

		/* ---- 0.91寸 OLED 更新 ---- */
		if (frame_count % 10 == 0)
		{
			unsigned char temp_disp = (unsigned char)(temperature + 0.5f);
			unsigned char blink = 0;

			if (temperature > 35.0f)
			{
				oled_blink_tick++;
				if (oled_blink_tick >= 10) oled_blink_tick = 0;
				blink = 1;
			}

			OLED091_ShowInfo(score, highest_score, temp_disp, blink);
		}

		reset = 0;
		frame_count++;

		cur_speed = score/20 + temp_bonus;
		if (cur_speed > 29) cur_speed = 29;
		HAL_Delay(30 - cur_speed);

		key_num = 0;
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
