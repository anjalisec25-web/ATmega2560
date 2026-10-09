#include "pwm.h"

/* Timer1 registers */

#define TCCR1A   (*(volatile unsigned char *)0x80)
#define TCCR1B   (*(volatile unsigned char *)0x81)

#define OCR1AL   (*(volatile unsigned char *)0x88)
#define OCR1AH   (*(volatile unsigned char *)0x89)

#define CS10     0
#define CS11     1
#define CS12     2

#define WGM10    0
#define WGM11    1
#define WGM12    3
#define WGM13    4

#define COM1A1   7
#define COM1A0   6


void PWM_Init(void)
{
    /*
     * PB5 = OC1A
     * Set PB5 as output
     */
    *((volatile unsigned char *)0x24) |= (1 << 5);

    /*
     * Fast PWM 8-bit
     *
     * WGM10 = 1
     * WGM11 = 0
     * WGM12 = 1
     * WGM13 = 0
     */
    TCCR1A = 0x00;
    TCCR1B = 0x00;

    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);

    /*
     * Non-inverting PWM on OC1A
     */
    TCCR1A |= (1 << COM1A1);

    /*
     * Initial duty cycle = 0%
     */
    OCR1AH = 0x00;
    OCR1AL = 0x00;
}


void PWM_Start(void)
{
    /*
     * Start Timer1
     * Prescaler = 64
     */
    TCCR1B |= (1 << CS11) | (1 << CS10);
}


void PWM_Stop(void)
{
    /*
     * Stop Timer1
     */
    TCCR1B &= ~((1 << CS12) |
                (1 << CS11) |
                (1 << CS10));
}


void PWM_SetDutyCycle(uint8_t duty)
{
    /*
     * 0   = 0%
     * 64  = 25%
     * 128 = 50%
     * 192 = 75%
     * 255 = 100%
     */
    OCR1AL = duty;
}