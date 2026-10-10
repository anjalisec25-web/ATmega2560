#include "ultra.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>

#define ULTRA_PORT       GPIO_PORT_B
#define ULTRA_TRIG_PIN   0
#define ULTRA_ECHO_PIN   1



#define TCCR3A   (*(volatile unsigned char *)0x90)
#define TCCR3B   (*(volatile unsigned char *)0x91)
#define TCNT3L   (*(volatile unsigned char *)0x94)
#define TCNT3H   (*(volatile unsigned char *)0x95)
#define TIFR3    (*(volatile unsigned char *)0x38)

#define CS30     0
#define CS31     1
#define CS32     2
#define TOV3     0

#define ULTRA_TIMEOUT_TICKS   7500U
#define ULTRA_MAX_CM          400UL

static uint16_t ULTRA_ReadTimer(void)
{
uint8_t low;
uint8_t high;

low  = TCNT3L;
high = TCNT3H;

return (uint16_t)(((uint16_t)high << 8) | low);


}


void ULTRA_Init(void)
{
GPIO_SetDirection(
ULTRA_PORT,
ULTRA_TRIG_PIN,
GPIO_OUTPUT
);

GPIO_SetDirection(
    ULTRA_PORT,
    ULTRA_ECHO_PIN,
    GPIO_INPUT
);

GPIO_WritePin(
    ULTRA_PORT,
    ULTRA_TRIG_PIN,
    GPIO_LOW
);

TCCR3A = 0x00;
TCCR3B = 0x00;
TCNT3H = 0x00;
TCNT3L = 0x00;

TIFR3 = (1 << TOV3);
TCCR3B = (1 << CS31) | (1 << CS30);
}

uint16_t ULTRA_GetDistance(void)
{
uint16_t start;
uint16_t rise;
uint16_t fall;
uint16_t ticks;
uint32_t distance;

start = ULTRA_ReadTimer();

while (GPIO_ReadPin(ULTRA_PORT, ULTRA_ECHO_PIN) == GPIO_HIGH)
{
    if ((uint16_t)(ULTRA_ReadTimer() - start) >= ULTRA_TIMEOUT_TICKS)
        return 0;
}


GPIO_WritePin(ULTRA_PORT, ULTRA_TRIG_PIN, GPIO_LOW);
TIMER_DelayUs(2);

GPIO_WritePin(ULTRA_PORT, ULTRA_TRIG_PIN, GPIO_HIGH);
TIMER_DelayUs(10);

GPIO_WritePin(ULTRA_PORT, ULTRA_TRIG_PIN, GPIO_LOW);

start = ULTRA_ReadTimer();

while (GPIO_ReadPin(ULTRA_PORT, ULTRA_ECHO_PIN) == GPIO_LOW)
{
    if ((uint16_t)(ULTRA_ReadTimer() - start) >= ULTRA_TIMEOUT_TICKS)
        return 0;
}

rise = ULTRA_ReadTimer();

while (GPIO_ReadPin(ULTRA_PORT, ULTRA_ECHO_PIN) == GPIO_HIGH)
{
    if ((uint16_t)(ULTRA_ReadTimer() - rise) >= ULTRA_TIMEOUT_TICKS)
        return 0;
}

fall = ULTRA_ReadTimer();
ticks = (uint16_t)(fall - rise);
distance = (((uint32_t)ticks * 4UL) + 29UL) / 58UL;

if (distance > ULTRA_MAX_CM)
    return 0;

return (uint16_t)distance;
}
