#include "timer.h"
#include <stdint.h>

#define TCCR0A   (*(volatile unsigned char *)0x44)
#define TCCR0B   (*(volatile unsigned char *)0x45)
#define TCNT0    (*(volatile unsigned char *)0x46)
#define TIFR0    (*(volatile unsigned char *)0x35)
#define OCR0A    (*(volatile unsigned char *)0x47)

#define CS00     0
#define CS01     1
#define CS02     2

#define WGM01    1

#define OCF0A    1
#define TOV0     0

#define TCCR4A   (*(volatile unsigned char *)0xA0)
#define TCCR4B   (*(volatile unsigned char *)0xA1)
#define TCNT4L   (*(volatile unsigned char *)0xA4)
#define TCNT4H   (*(volatile unsigned char *)0xA5)

#define CS40     0
#define CS41     1

static uint16_t ms_last = 0;
static uint32_t ms_count = 0;
static uint32_t ms_rem = 0;


static void TIMER_MillisInit(void)
{
    TCCR4A = 0x00;
    TCCR4B = (1 << CS41) | (1 << CS40);

    TCNT4H = 0x00;
    TCNT4L = 0x00;

    ms_last = 0;
    ms_count = 0;
    ms_rem = 0;
}

unsigned long TIMER_GetMs(void)
{
    uint8_t low;
    uint8_t high;
    uint16_t now;
    uint16_t delta;

    low = TCNT4L;
    high = TCNT4H;
    now = ((uint16_t)high << 8) | low;
    delta = (uint16_t)(now - ms_last);
    ms_last = now;
    ms_rem += delta;
    ms_count += ms_rem / 250UL;
    ms_rem %= 250UL;

    return ms_count;
}


void TIMER_Init(void)
{
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    TIFR0 |= (1 << TOV0);
    TIMER_MillisInit();
}


void TIMER_Start(void)
{
    TCCR0B |= (1 << CS01) | (1 << CS00);
}


void TIMER_Stop(void)
{
    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}


unsigned char TIMER_GetOverflowFlag(void)
{
    if (TIFR0 & (1 << TOV0))
        return 1;
    else
        return 0;
}


void TIMER_ClearOverflowFlag(void)
{
    TIFR0 |= (1 << TOV0);
}


void TIMER_DelayUs(unsigned int microseconds)
{
    unsigned int i;

    TCCR0A = 0x00;
    TCCR0B = 0x00;

    OCR0A = 15;

    TCCR0A |= (1 << WGM01);

    TCCR0B |= (1 << CS00);

    for (i = 0; i < microseconds; i++)
    {
        TCNT0 = 0x00;

        TIFR0 |= (1 << OCF0A);

        while (!(TIFR0 & (1 << OCF0A)))
        {
            ;
        }
    }

    TCCR0B = 0x00;
    TCCR0A = 0x00;
}