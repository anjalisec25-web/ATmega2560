#include "gpio.h"

void GPIO_SetDirection(GPIO_Def_t *port, unsigned char pin, GPIO_Direction_t direction)
{
    if(direction == GPIO_OUTPUT)
        port->DDR |= (1 << pin);
    else
        port->DDR &= ~(1 << pin);
}


void GPIO_WritePin(GPIO_Def_t *port, unsigned char pin, GPIO_State_t state)
{
    if(state == GPIO_HIGH)
        port->PORT |= (1 << pin);
    else
        port->PORT &= ~(1 << pin);
}


GPIO_State_t GPIO_ReadPin(GPIO_Def_t *port, unsigned char pin)
{
    if(port->PIN & (1 << pin))
        return GPIO_HIGH;
    else
        return GPIO_LOW;
}


void GPIO_TogglePin(GPIO_Def_t *port,
                    unsigned char pin)
{
    port->PORT ^= (1 << pin);
}