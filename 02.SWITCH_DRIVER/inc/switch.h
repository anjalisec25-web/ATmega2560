#ifndef SWITCH_H
#define SWITCH_H

#include "gpio.h"

typedef enum
{
    SWITCH_RELEASED,
    SWITCH_PRESSED
} Switch_State_t;

void SWITCH_Init(GPIO_Def_t *port, unsigned char pin);
Switch_State_t SWITCH_GetState(GPIO_Def_t *port, unsigned char pin);

#endif