#include "ws2812.h"
#include "tim.h"
#include <stdint.h>

#define Code0 30
#define Code1 60
#define CodeReset 0

void ws2812_update(){//GRB
    static uint16_t data[]={
        //FF0000
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,

        //00FF00
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,

        //0000FF
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,

        //FFFF00
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,

        //FF00FF
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,

        //00FFFF
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code0,Code0,Code0,Code0,Code0,Code0,Code0,Code0,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        
        //FFFFFF
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,
        Code1,Code1,Code1,Code1,Code1,Code1,Code1,Code1,

        CodeReset
    };
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)data, sizeof(data)/sizeof(uint16_t));
}
