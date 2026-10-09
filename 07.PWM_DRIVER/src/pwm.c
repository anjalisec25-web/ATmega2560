#include "pwm.h"

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
    *((volatile unsigned char *)0x24) |= (1 << 5);
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);
    TCCR1A |= (1 << COM1A1);
    OCR1AH = 0x00;
    OCR1AL = 0x00;
}


void PWM_Start(void)
{
    TCCR1B |= (1 << CS11) | (1 << CS10);
}


void PWM_Stop(void)
{
    TCCR1B &= ~((1 << CS12) |(1 << CS11) |(1 << CS10));
}


void PWM_SetDutyCycle(uint8_t duty)
{
    OCR1AL = duty;
}