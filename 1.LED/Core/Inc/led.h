#include "main.h"

#define STATE_R0G0B0 0000
#define STATE_R1G0B0 0001
#define STATE_R0G1B0 0002
#define STATE_R0G0B1 0003
#define STATE_R1G1B0 0004
#define STATE_R1G0B1 0005
#define STATE_R0G1B1 0006
#define STATE_R1G1B1 0007
#define MODE_ROLLING 0100
#define MODE_MANUAL 0101

int led_control_rolling(int currentstate);
void led_control_manual();