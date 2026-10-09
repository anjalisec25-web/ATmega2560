#include "timer.h"

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


void TIMER_Init(void)
{
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    TIFR0 |= (1 << TOV0);
}

void TIMER_Start(void)
{
    TCCR0B |= (1 << CS01) | (1 << CS00);
}


void TIMER_Stop(void)
{
    TCCR0B &= ~((1 << CS02) |(1 << CS01) |(1 << CS00));
}


unsigned char TIMER_GetOverflowFlag(void)
{
    if(TIFR0 & (1 << TOV0))
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

    for(i = 0; i < microseconds; i++)
    {
        TCNT0 = 0x00;
        TIFR0 |= (1 << OCF0A);
        while(!(TIFR0 & (1 << OCF0A)))
        {
            ;
        }
    }
    TCCR0B = 0x00;
    TCCR0A = 0x00;
}