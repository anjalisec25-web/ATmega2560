#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum
{
    GPIO_INPUT,
    GPIO_OUTPUT
} GPIO_Direction_t;

typedef enum
{
    GPIO_LOW,
    GPIO_HIGH
} GPIO_State_t;

typedef struct
{
    volatile uint8_t PIN;
    volatile uint8_t DDR;
    volatile uint8_t PORT;
} GPIO_Def_t;

#define GPIO_PORT_A   ((GPIO_Def_t *)0x20)
#define GPIO_PORT_B   ((GPIO_Def_t *)0x23)
#define GPIO_PORT_C   ((GPIO_Def_t *)0x26)
#define GPIO_PORT_D   ((GPIO_Def_t *)0x29)
#define GPIO_PORT_E   ((GPIO_Def_t *)0x2C)
#define GPIO_PORT_F   ((GPIO_Def_t *)0x2F)
#define GPIO_PORT_G   ((GPIO_Def_t *)0x32)
#define GPIO_PORT_H   ((GPIO_Def_t *)0x100)
#define GPIO_PORT_J   ((GPIO_Def_t *)0x103)
#define GPIO_PORT_K   ((GPIO_Def_t *)0x106)
#define GPIO_PORT_L   ((GPIO_Def_t *)0x109)

void GPIO_SetDirection(GPIO_Def_t *port, unsigned char pin, GPIO_Direction_t direction);
void GPIO_WritePin(GPIO_Def_t *port, unsigned char pin, GPIO_State_t state);
GPIO_State_t GPIO_ReadPin(GPIO_Def_t *port, unsigned char pin);
void GPIO_TogglePin(GPIO_Def_t *port, unsigned char pin);

#endif