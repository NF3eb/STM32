#ifndef __AHT20_H__
#define __AHT20_H__

#include "i2c.h"

void AHT20_Init(void);
void AHT20_Measure(float *temperature,float *humidity);
void AHT20_Print(void);

#endif