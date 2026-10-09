#ifndef IR_H
#define IR_H

#include "gpio.h"

typedef enum
{
    IR_AVAILABLE,
    IR_OCCUPIED
} IR_State_t;

void IR_Init(GPIO_Def_t *port, unsigned char pin);
IR_State_t IR_GetState(GPIO_Def_t *port, unsigned char pin);

#endif