/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include <stdio.h>
#include <stdint.h>
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BASE 32767  //使计数器以32767为原点，防止计数器从0减一到65535
#define INITIAL_BRIGHTNESS 50-BRIGHTNESS_STEP //初始数据和实际的INITIAL_BRIGHTNESS相差BRIGHTNESS_STEP，使程序开始进行时屏幕上显示出的信息是实际的INITIAL_BRIGHTNESS
#define INITIAL_DUTY 150-DUTY_STEP //初始数据和实际的INITIAL_DUTY相差DUTY_STEP，使程序开始进行时屏幕上显示出的信息是实际的INITIAL_BRIGHTNESS
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 255
#define MIN_DUTY 50
#define MAX_DUTY 250
#define BRIGHTNESS_STEP 1 //每个编码器脉冲时，brightness增加多少
#define DUTY_STEP -1*20 //每个编码器脉冲时，duty增加多少
#define CHANNEL_RED 2
#define CHANNEL_GREEN 1
#define CHANNEL_BLUE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t counter=0;
uint8_t duty=INITIAL_DUTY;//占空比转换：counter每加1，duty-=20
uint8_t brightness=INITIAL_BRIGHTNESS;//亮度转换：counter-BASE+50
uint8_t channel_index=CHANNEL_BLUE;
uint32_t channels[3]={TIM_CHANNEL_1,TIM_CHANNEL_2,TIM_CHANNEL_3};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(20);
  OLED_Init();

  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  HAL_TIM_PWM_Start(&htim3, channels[channel_index]);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  __HAL_TIM_SET_COUNTER(&htim1, BASE+1);//初始数据和BASE相差1，用于触发更新，使程序开始进行时屏幕上就显示出信息
  __HAL_TIM_SET_COMPARE(&htim3, channels[channel_index], brightness);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, duty);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    uint16_t counter=__HAL_TIM_GET_COUNTER(&htim1);
    int16_t diff=(int16_t)(counter-BASE);
    if(diff){//计数器值产生变化时：
      //设置duty
      int16_t newduty=duty+DUTY_STEP*diff;
      if((newduty>=MIN_DUTY) && (newduty<=MAX_DUTY)){//若newduty还在MIN_DUTY(50)到MAX_DUTY(250)之间，则直接更改
        duty+=DUTY_STEP*diff;
      }
      else if(newduty<MIN_DUTY){//若newduty已经超出了MIN_DUTY(50)的范围，说明已经达到下限，则将其设为MIN_DUTY(50)
        duty=MIN_DUTY;
      }
      else if(newduty>MAX_DUTY){//若newduty已经超出了MAX_DUTY(250)的范围，说明已经达到下限，则将其设为MAX_DUTY(250)
        duty=MAX_DUTY;
      }
      __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3,duty);

      //设置brightness
      int16_t newbrightness=brightness+BRIGHTNESS_STEP*diff;
      if((newbrightness>=0) && (newbrightness<=255)){//若newbrightness还在MIN_BRIGHTNESS(0)到MAX_BRIGHTNESS(255)之间，则直接更改
        brightness+=BRIGHTNESS_STEP*diff;
      }
      else if(newbrightness<MIN_BRIGHTNESS){//若newbrightness已经超出了MIN_BRIGHTNESS(0)的范围，说明已经达到下限，则将其设为MIN_BRIGHTNESS(0)
        brightness=MIN_BRIGHTNESS;
      }
      else if(newbrightness>MAX_BRIGHTNESS){//若newbrightness已经超出了MAX_BRIGHTNESS(255)的范围，说明已经达到下限，则将其设为MAX_BRIGHTNESS(255)
        brightness=MAX_BRIGHTNESS;
      }
      __HAL_TIM_SET_COMPARE(&htim3, channels[channel_index],brightness);

      //重新锚定计数器为BASE(32767)，防止计数器越界
      __HAL_TIM_SET_COUNTER(&htim1, BASE);

      //OLED屏幕显示
      OLED_NewFrame();
      char message[20]="";
      sprintf(message, "brightness:%d",brightness);
      OLED_PrintString(0, 0, message, &font16x16, OLED_COLOR_NORMAL);
      sprintf(message, "duty:%d(%d.%d%%)",duty,(int)(2.5+(((float)duty-MIN_DUTY)/(MAX_DUTY-MIN_DUTY)*(12.5-2.5))),(int)(10*(2.5+(((float)duty-MIN_DUTY)/(MAX_DUTY-MIN_DUTY)*(12.5-2.5))))%10);//50-250 -> 2.5-12.5
      OLED_PrintString(0, 16, message, &font16x16, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
    }
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
#ifdef USE_FULL_ASSERT
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
