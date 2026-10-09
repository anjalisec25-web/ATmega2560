#ifndef ADC_H
#define ADC_H

#include <stdint.h>

typedef enum
{
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7
} ADC_Channel_t;

void ADC_Init(void);
void ADC_SelectChannel(ADC_Channel_t channel);
void ADC_StartConversion(void);
uint16_t ADC_Read(void);

#endif