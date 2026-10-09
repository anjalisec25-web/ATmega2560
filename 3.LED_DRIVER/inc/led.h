#ifndef LED_H
#define LED_H

#include "gpio.h"

typedef enum
{
    LED_OFF,
    LED_ON
} LED_State_t;

void LED_Init(GPIO_Def_t *port, unsigned char pin);
void LED_On(GPIO_Def_t *port, unsigned char pin);
void LED_Off(GPIO_Def_t *port, unsigned char pin);
void LED_Toggle(GPIO_Def_t *port, unsigned char pin);
LED_State_t LED_GetState(GPIO_Def_t *port, unsigned char pin);

#endif