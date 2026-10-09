#include "adc.h"

/* ================================
   ADC Registers
   ================================ */

#define ADMUX    (*(volatile unsigned char *)0x7C)
#define ADCSRA   (*(volatile unsigned char *)0x7A)
#define ADCL     (*(volatile unsigned char *)0x78)
#define ADCH     (*(volatile unsigned char *)0x79)

/* ================================
   ADMUX Bits
   ================================ */

#define REFS0    6
#define REFS1    7

/* ================================
   ADCSRA Bits
   ================================ */

#define ADPS0    0
#define ADPS1    1
#define ADPS2    2
#define ADSC     6
#define ADEN     7


/* ================================
   ADC_Init
   ================================ */

void ADC_Init(void)
{
    /* AVCC as reference voltage */
    ADMUX = (1 << REFS0);

    /* Enable ADC
       Prescaler = 128 */
    ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
}


/* ================================
   ADC_SelectChannel
   ================================ */

void ADC_SelectChannel(ADC_Channel_t channel)
{
    /* Keep reference bits */
    ADMUX &= 0xE0;

    /* Select ADC channel */
    ADMUX |= channel;
}


/* ================================
   ADC_StartConversion
   ================================ */

void ADC_StartConversion(void)
{
    /* Start ADC conversion */
    ADCSRA |= (1 << ADSC);
}


/* ================================
   ADC_Read
   ================================ */

uint16_t ADC_Read(void)
{
    uint16_t result;

    /* Wait until conversion completes */
    while(ADCSRA & (1 << ADSC))
    {
        ;
    }

    /* Read low byte first */
    result = ADCL;

    /* Read high byte */
    result |= ((uint16_t)ADCH << 8);

    return result;
}