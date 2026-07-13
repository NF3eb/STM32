#include "led.h"
#include "main.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

int led_control_rolling(int currentstate){
    switch (currentstate) {
        case STATE_R0G0B0:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
            break;
        }
        case STATE_R1G0B0:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
            break;
        }
        case STATE_R0G1B0:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
            break;
        }
        case STATE_R0G0B1:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
            break;
        }
        case STATE_R1G1B0:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_RESET);
            break;
        }
        case STATE_R1G0B1:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
            break;
        }
        case STATE_R0G1B1:{
            HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
            break;
        }
        case STATE_R1G1B1:{
           HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
           HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
           HAL_GPIO_WritePin(LED_Blue_GPIO_Port, LED_Blue_Pin, GPIO_PIN_SET);
           return STATE_R0G0B0;
           break;
        }
        default:{
            return -1;
        }
    }
    return currentstate+1;
}

void led_control_manual(){
    HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)==GPIO_PIN_RESET){
        HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
    }
    else{
        HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
    }

    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==GPIO_PIN_RESET){
        HAL_Delay(10);
        if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==GPIO_PIN_RESET)
        {
            HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin);
            while (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==GPIO_PIN_RESET) {}
        }
    }
    return;
}
