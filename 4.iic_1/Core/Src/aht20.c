#include "aht20.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define AHT20_ADDRESS 0x70

void AHT20_Init(void){
    HAL_Delay(40);
    uint8_t readBuffer;
    HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, &readBuffer, 1, HAL_MAX_DELAY);
    if((readBuffer & 0x08) == 0x00){//用 按位与(&) 的操作检测readBuffer第3位是否为1，如果是0，则返回0x00，如果是1，则返回0x08
        static uint8_t sendBuffer[3]={0xBE,0x08,0x00};
        HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
    }
    return;
}

void AHT20_Measure(float *temperature,float *humidity){
    static uint8_t sendBuffer[3]={0xAC,0x33,0x00};
    HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDRESS, sendBuffer, 3, HAL_MAX_DELAY);
    char *message_measuring="Measuring...";
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)message_measuring, strlen(message_measuring));
    HAL_Delay(75);

    uint8_t readBuffer[6]={0};
    HAL_I2C_Master_Receive(&hi2c1, AHT20_ADDRESS, readBuffer, 6, HAL_MAX_DELAY);
    if((readBuffer[0] & 0x80)==0x00){
        uint32_t data=0;
        data=((uint32_t)readBuffer[3]>>4)+((uint32_t)readBuffer[2]<<4)+((uint32_t)readBuffer[1]<<12);
        *humidity=data*100.0f/(1<<20);
        data=((uint32_t)readBuffer[5])+((uint32_t)readBuffer[4]<<8)+(((uint32_t)readBuffer[3] & 0x0F)<<16);
        *temperature=data*200.0f/(1<<20)-50.0;
    }
    return;
}

void AHT20_Print(void){
    float temperature=-1.0,humidity=-1.0;
    char message[50];
    struct {
        int integer;
        int floating;
    }temperature_temp,humidity_temp;
    AHT20_Measure(&temperature,&humidity);
    temperature_temp.integer=(int)temperature;
    temperature_temp.floating=(int)((temperature-temperature_temp.integer)*10);
    humidity_temp.integer=(int)humidity;
    humidity_temp.floating=(int)((humidity-humidity_temp.integer)*10);
    sprintf(message, "Temperature:%d.%d,Humidity:%d.%d%%\n\r",temperature_temp.integer,temperature_temp.floating,humidity_temp.integer,humidity_temp.floating);
    HAL_UART_Transmit(&huart2, (uint8_t*)message,strlen(message), HAL_MAX_DELAY);
    return;
}