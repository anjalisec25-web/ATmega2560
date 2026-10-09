#include "led.h"

void LED_Init(GPIO_Def_t *port, unsigned char pin)
{
    GPIO_SetDirection(port, pin, GPIO_OUTPUT);
    LED_Off(port, pin);
}

void LED_On(GPIO_Def_t *port, unsigned char pin)
{
    GPIO_WritePin(port, pin, GPIO_HIGH);
}

void LED_Off(GPIO_Def_t *port, unsigned char pin)
{
    GPIO_WritePin(port, pin, GPIO_LOW);
}

void LED_Toggle(GPIO_Def_t *port, unsigned char pin)
{
    GPIO_TogglePin(port, pin);
}

LED_State_t LED_GetState(GPIO_Def_t *port, unsigned char pin)
{
    if (GPIO_ReadPin(port, pin) == GPIO_HIGH)
        return LED_ON;

    return LED_OFF;
}