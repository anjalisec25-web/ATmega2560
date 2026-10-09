#include "adc.h"

#define ADMUX    (*(volatile unsigned char *)0x7C)
#define ADCSRA   (*(volatile unsigned char *)0x7A)
#define ADCL     (*(volatile unsigned char *)0x78)
#define ADCH     (*(volatile unsigned char *)0x79)
#define REFS0    6
#define REFS1    7
#define ADPS0    0
#define ADPS1    1
#define ADPS2    2
#define ADSC     6
#define ADEN     7

void ADC_Init(void)
{
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) |(1 << ADPS2) |(1 << ADPS1) |(1 << ADPS0);
}

void ADC_SelectChannel(ADC_Channel_t channel)
{
    ADMUX &= 0xE0;
    ADMUX |= channel;
}

void ADC_StartConversion(void)
{
   
    ADCSRA |= (1 << ADSC);
}

uint16_t ADC_Read(void)
{
    uint16_t result;
    while(ADCSRA & (1 << ADSC))
    {
        ;
    }
    result = ADCL;
    result |= ((uint16_t)ADCH << 8);
    return result;
}