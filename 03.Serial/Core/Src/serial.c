#include "serial.h"
#include "stm32f1xx_hal_uart.h"
#include "string.h"
#include "usart.h"

static const struct {
  struct {
    const char *red_enabled;
    const char *red_disabled;
    const char *green_enabled;
    const char *green_disabled;
    const char *blue_enabled;
    const char *blue_disabled;
  } LED;
  const char *invalid;
} message={
    .LED={
        .red_enabled = "LED_Red enabled.",
        .red_disabled = "LED_Red disabled.",
        .green_enabled = "LED_Green enabled.",
        .green_disabled = "LED_Green disabled.",
        .blue_enabled = "LED_Blue enabled.",
        .blue_disabled = "LED_Blue disabled."
    },
    .invalid = "Invalid."
};
uint8_t receive_data[2];

void serial_Polling(void) {
  HAL_UART_Receive(&huart2, receive_data, 2, HAL_MAX_DELAY);
  switch (receive_data[0]) {
  case 'R': {
    switch (receive_data[1]) {
    case '0': {
      HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.red_disabled, strlen(message.LED.red_disabled), HAL_MAX_DELAY);
      break;
    }
    case '1': {
      HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.red_enabled, strlen(message.LED.red_enabled), HAL_MAX_DELAY);
      break;
    }
    }
    break;
  }
  case 'G': {
    switch (receive_data[1]) {
    case '0': {
      HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.green_disabled, strlen(message.LED.green_disabled), HAL_MAX_DELAY);
      break;
    }
    case '1': {
      HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.green_enabled, strlen(message.LED.green_enabled), HAL_MAX_DELAY);
      break;
    }
    }
    break;
  }
  case 'B': {
    switch (receive_data[1]) {
    case '0': {
      HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.blue_disabled, strlen(message.LED.blue_disabled), HAL_MAX_DELAY);
      break;
    }
    case '1': {
      HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.blue_enabled, strlen(message.LED.blue_enabled), HAL_MAX_DELAY);
      break;
    }
    }
    break;
  }
  default: {
    HAL_UART_Transmit(&huart2, (uint8_t*)message.invalid, strlen(message.invalid), HAL_MAX_DELAY);
  }
  }
  return;
}

int is_receiving=0;
void serial_IT(void){
  if(is_receiving==0){
    HAL_UART_Receive_IT(&huart2,receive_data, 2);
    is_receiving=1;
  }return;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if (huart==&huart2)
  {
    switch (receive_data[0]) {
    case 'R': {
      switch (receive_data[1]) {
      case '0': {
        HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.red_disabled, strlen(message.LED.red_disabled), HAL_MAX_DELAY);
        break;
      }
      case '1': {
        HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.red_enabled, strlen(message.LED.red_enabled), HAL_MAX_DELAY);
        break;
      }
      }
      break;
    }
    case 'G': {
      switch (receive_data[1]) {
      case '0': {
        HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.green_disabled, strlen(message.LED.green_disabled), HAL_MAX_DELAY);
        break;
      }
      case '1': {
        HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.green_enabled, strlen(message.LED.green_enabled), HAL_MAX_DELAY);
        break;
      }
      }
      break;
    }
    case 'B': {
      switch (receive_data[1]) {
      case '0': {
        HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.blue_disabled, strlen(message.LED.blue_disabled), HAL_MAX_DELAY);
        break;
      }
      case '1': {
        HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
        HAL_UART_Transmit(&huart2, (uint8_t*)message.LED.blue_enabled, strlen(message.LED.blue_enabled), HAL_MAX_DELAY);
        break;
      }
      }
      break;
    }
    default: {
      HAL_UART_Transmit(&huart2, (uint8_t*)message.invalid, strlen(message.invalid), HAL_MAX_DELAY);
    }
    }
  }
  is_receiving=0;
  return;
}

void serial_DMA(void){
  if(is_receiving==0){
    HAL_UART_Receive_DMA(&huart2,receive_data, 2);
    is_receiving=1;
  }
  return;
}

uint8_t receive_data_2[50];
void serial_idle(void){
  if(is_receiving==0){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, receive_data_2, sizeof(receive_data_2));
    is_receiving=1;
  }
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
  return;
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
  if(huart==&huart2){
    HAL_UART_Transmit_DMA(&huart2, receive_data_2, Size);
  }
  is_receiving=0;
  return;
}