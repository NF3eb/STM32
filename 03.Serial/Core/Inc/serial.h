#ifndef __SERIAL_H
#define __SERIAL_H

#include "serial.h"
#include "usart.h"

void serial_Polling(void);
void serial_IT(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void serial_DMA(void);
void serial_idle(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

#endif