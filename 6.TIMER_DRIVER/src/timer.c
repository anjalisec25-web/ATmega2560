#include "timer.h"

/* Timer0 registers */

#define TCCR0A   (*(volatile unsigned char *)0x44)
#define TCCR0B   (*(volatile unsigned char *)0x45)
#define TCNT0    (*(volatile unsigned char *)0x46)
#define TIFR0    (*(volatile unsigned char *)0x35)
#define OCR0A    (*(volatile unsigned char *)0x47)

/* Timer0  bits */
#define CS00     0
#define CS01     1
#define CS02     2

#define WGM01    1

#define OCF0A    1
#define TOV0     0


void TIMER_Init(void)
{
    /* Normal mode */
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    /* Clear timer counter */
    TCNT0 = 0x00;

    /* Clear overflow flag */
    TIFR0 |= (1 << TOV0);
}


void TIMER_Start(void)
{
    /*
     * Start Timer0
     * Prescaler = 64
     */
    TCCR0B |= (1 << CS01) | (1 << CS00);
}


void TIMER_Stop(void)
{
    /*
     * Stop Timer0
     */
    TCCR0B &= ~((1 << CS02) |
                (1 << CS01) |
                (1 << CS00));
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
    /*
     * Timer overflow flag is cleared
     * by writing a logic 1 to TOV0.
     */
    TIFR0 |= (1 << TOV0);
}

void TIMER_DelayUs(unsigned int microseconds)
{
    unsigned int i;

    /*
     * Timer0 CTC mode
     *
     * CPU Clock = 16 MHz
     * Prescaler = 1
     *
     * Timer clock = 16 MHz
     * 1 tick = 62.5 ns
     *
     * OCR0A = 15
     * 16 timer counts = 1 us
     */

    TCCR0A = 0x00;
    TCCR0B = 0x00;

    OCR0A = 15;

    /* CTC mode */
    TCCR0A |= (1 << WGM01);

    /* No prescaler */
    TCCR0B |= (1 << CS00);

    for(i = 0; i < microseconds; i++)
    {
        /* Reset counter */
        TCNT0 = 0x00;

        /* Clear compare match flag */
        TIFR0 |= (1 << OCF0A);

        /* Wait for 1 us */
        while(!(TIFR0 & (1 << OCF0A)))
        {
            ;
        }
    }

    /* Stop Timer0 */
    TCCR0B = 0x00;

    /* Return Timer0 to normal mode */
    TCCR0A = 0x00;
}