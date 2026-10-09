#include "ir.h"

void IR_Init(GPIO_Def_t *port, unsigned char pin)
{
    /* IR sensor -> INPUT */
    GPIO_SetDirection(
        port,
        pin,
        GPIO_INPUT
    );
}

IR_State_t IR_GetState(GPIO_Def_t *port, unsigned char pin)
{
    /*
     * IR sensor output:
     * LOW  -> Vehicle present
     * HIGH -> Vehicle absent
     */

    if(GPIO_ReadPin(port, pin) == GPIO_LOW)
        return IR_OCCUPIED;

    return IR_AVAILABLE;
}