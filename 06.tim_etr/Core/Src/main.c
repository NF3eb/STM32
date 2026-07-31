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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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


/*Reset Mode*/
// static const char *message_reload_auto="Reloaded(Auto).\n";
// static const char *message_reload_manual="Reloaded(Manual).\n";
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//   if(htim==&htim2){
//     if(__HAL_TIM_GET_FLAG(htim,TIM_FLAG_TRIGGER)==SET){
//       __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_TRIGGER);
//       HAL_UART_Transmit_IT(&huart2, (uint8_t*)message_reload_manual,strlen(message_reload_manual));
//       return;
//     }else{
//       HAL_UART_Transmit_IT(&huart2, (uint8_t*)message_reload_auto,strlen(message_reload_auto));
//       return;
//     }
//   }
// }


/* Gated Mode/Trigger Mode */
// static const char *message_reload_auto="Reloaded(Auto).\n";
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//   if(htim==&htim2){
//     HAL_UART_Transmit_IT(&huart2, (uint8_t*)message_reload_auto,strlen(message_reload_auto));
//     return;
//   }
// }


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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(20);
  OLED_Init();
  // HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim2);
  int counter=0,counter_temp=-1;
  char sendBuffer[20]="";
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /*Counter*/
    counter=__HAL_TIM_GET_COUNTER(&htim2);
    if(counter!=counter_temp){
      OLED_NewFrame();
      sprintf(sendBuffer, "counter:%d",counter);
      OLED_PrintString(0, 0, sendBuffer, &font16x16, OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      counter_temp=counter;
      sprintf(sendBuffer, "counter:%d",counter);
      HAL_UART_Transmit_IT(&huart2, (uint8_t*)sendBuffer, strlen(sendBuffer));
    }



    /*Reset Mode*/
    // counter=__HAL_TIM_GET_COUNTER(&htim2);
    // sprintf(sendBuffer, "counter:%d",counter);
    // HAL_UART_Transmit_IT(&huart2, (uint8_t*)sendBuffer, strlen(sendBuffer));
    // HAL_Delay(500);



    /* Gated Mode */
    // const char *message_gate="Gated Mode triggered.\n";
    // if(__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_TRIGGER)==1){
    //   __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_TRIGGER);
    //   HAL_UART_Transmit_IT(&huart2, (uint8_t*)message_gate, strlen(message_gate));
    // }
    // counter=__HAL_TIM_GET_COUNTER(&htim2);
    // sprintf(sendBuffer, "counter:%d",counter);
    // HAL_UART_Transmit_IT(&huart2, (uint8_t*)sendBuffer, strlen(sendBuffer));
    // HAL_Delay(500);



    /* Trigger Mode */
    // const char *message_trigger="Trigger Mode triggered.\n";
    // if(__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_TRIGGER)==1){
    //   __HAL_TIM_CLEAR_FLAG(&htim2,TIM_FLAG_TRIGGER);
    //   HAL_UART_Transmit_IT(&huart2, (uint8_t*)message_trigger, strlen(message_trigger));
    // }
    // counter=__HAL_TIM_GET_COUNTER(&htim2);
    // sprintf(sendBuffer, "counter:%d",counter);
    // HAL_UART_Transmit_IT(&huart2, (uint8_t*)sendBuffer, strlen(sendBuffer));
    // HAL_Delay(500);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
