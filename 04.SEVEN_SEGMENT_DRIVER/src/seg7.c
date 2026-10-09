#include "seg7.h"
#include "gpio.h"

#define SEG7_PORT       GPIO_PORT_A
#define SEG7_TENS_PIN   0
#define SEG7_ONES_PIN   1
#define SEG7_DIGIT_PORT GPIO_PORT_C
#define DIGIT_ON        GPIO_HIGH
#define DIGIT_OFF       GPIO_LOW

static const uint8_t digit_lut[10] =
{
    0x3F,   
    0x06,   
    0x5B,   
    0x4F,   
    0x66,   
    0x6D,   
    0x7D,   
    0x07,   
    0x7F,   
    0x6F    
};

static volatile uint8_t display[2] = {0, 0};

static uint8_t active_digit = 0;

static void SEG7_WritePattern(uint8_t pattern)
{
    uint8_t pin;

    for (pin = 0; pin < 8; pin++)
    {
        if (pattern & (1 << pin))
        {
            GPIO_WritePin(
                SEG7_PORT,
                pin,
                GPIO_HIGH
            );
        }
        else
        {
            GPIO_WritePin(
                SEG7_PORT,
                pin,
                GPIO_LOW
            );
        }
    }
}

void SEG7_Init(void)
{
    uint8_t pin;
    for (pin = 0; pin < 8; pin++)
    {
        GPIO_SetDirection(
            SEG7_PORT,
            pin,
            GPIO_OUTPUT
        );
    }
    GPIO_SetDirection(
        SEG7_DIGIT_PORT,
        SEG7_TENS_PIN,
        GPIO_OUTPUT
    );
    GPIO_SetDirection(
        SEG7_DIGIT_PORT,
        SEG7_ONES_PIN,
        GPIO_OUTPUT
    );
    GPIO_WritePin(
        SEG7_DIGIT_PORT,
        SEG7_TENS_PIN,
        DIGIT_OFF
    );
    GPIO_WritePin(
        SEG7_DIGIT_PORT,
        SEG7_ONES_PIN,
        DIGIT_OFF
    );
    SEG7_WritePattern(0x00);
    SEG7_Clear();
}

void SEG7_Refresh(void)
{
    uint8_t pattern;
    GPIO_WritePin(
        SEG7_DIGIT_PORT,
        SEG7_TENS_PIN,
        DIGIT_OFF
    );
    GPIO_WritePin(
        SEG7_DIGIT_PORT,
        SEG7_ONES_PIN,
        DIGIT_OFF
    );
    active_digit ^= 1;
    pattern = display[active_digit];
    SEG7_WritePattern(pattern);
    if (active_digit == 0)
    {
        GPIO_WritePin(
            SEG7_DIGIT_PORT,
            SEG7_TENS_PIN,
            DIGIT_ON
        );
    }
    else
    {
        GPIO_WritePin(
            SEG7_DIGIT_PORT,
            SEG7_ONES_PIN,
            DIGIT_ON
        );
    }
}

void SEG7_Clear(void)
{
    display[0] = 0x00;
    display[1] = 0x00;
}

void SEG7_DisplayNumber(uint8_t number)
{
    uint8_t tens;
    uint8_t ones;
    if (number > 99)
    {
        number = 99;
    }
    tens = number / 10;
    ones = number % 10;
    display[0] = digit_lut[tens];
    display[1] = digit_lut[ones];
}