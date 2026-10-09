#include "switch.h"

void SWITCH_Init(GPIO_Def_t *port, unsigned char pin)
{
    GPIO_SetDirection(port, pin, GPIO_INPUT);
}

Switch_State_t SWITCH_GetState(GPIO_Def_t *port, unsigned char pin)
{
    if (GPIO_ReadPin(port, pin) == GPIO_LOW)
        return SWITCH_PRESSED;

    return SWITCH_RELEASED;
}