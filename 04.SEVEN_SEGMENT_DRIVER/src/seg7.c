#include "seg7.h"
#include "gpio.h"
#include <stdint.h>

/*
 * 7-Segment Configuration
 * Segments: PA0 to PA7
 * Tens digit select: PC0
 * Ones digit select: PC1
 */

#define SEG7_PORT          GPIO_PORT_A
#define SEG7_TENS_PIN      0
#define SEG7_ONES_PIN      1
#define SEG7_DIGIT_PORT    GPIO_PORT_C

#define DIGIT_ON           GPIO_HIGH
#define DIGIT_OFF          GPIO_LOW

/*
 * 0 = Common Cathode
 * 1 = Common Anode
 */
#define SEG7_COMMON_ANODE  0

/*
 * Segment patterns:
 * bit0 = a, bit1 = b, bit2 = c, bit3 = d
 * bit4 = e, bit5 = f, bit6 = g, bit7 = dp
 */

static const uint8_t digit_lut[10] =
{
    0x3F,   /* 0 */
    0x06,   /* 1 */
    0x5B,   /* 2 */
    0x4F,   /* 3 */
    0x66,   /* 4 */
    0x6D,   /* 5 */
    0x7D,   /* 6 */
    0x07,   /* 7 */
    0x7F,   /* 8 */
    0x6F    /* 9 */
};

#define SEG7_DASH  0x40

static volatile uint8_t display[2] = {0, 0};
static uint8_t active_digit = 0;


/* Write a segment pattern using the GPIO driver */

static void SEG7_WritePattern(uint8_t pattern)
{
    uint8_t pin;

#if SEG7_COMMON_ANODE
    pattern = (uint8_t)~pattern;
#endif

    for (pin = 0; pin < 8; pin++)
    {
        if (pattern & (1U << pin))
        {
            GPIO_WritePin(SEG7_PORT, pin, GPIO_HIGH);
        }
        else
        {
            GPIO_WritePin(SEG7_PORT, pin, GPIO_LOW);
        }
    }
}


/* Initialize the 7-segment display */

void SEG7_Init(void)
{
    uint8_t pin;

    /* Configure segment pins PA0-PA7 as outputs */
    for (pin = 0; pin < 8; pin++)
    {
        GPIO_SetDirection(SEG7_PORT, pin, GPIO_OUTPUT);
    }

    /* Configure digit-select pins PC0 and PC1 */
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

    /* Initially disable both digits */
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

    active_digit = 0;

    SEG7_Clear();
}


/* Refresh the display.
 * Call repeatedly from the application.
 */

void SEG7_Refresh(void)
{
    uint8_t pattern;

    /* Disable both digits before changing segments */
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

    /* Alternate between the two digits */
    active_digit ^= 1U;

    pattern = display[active_digit];

    SEG7_WritePattern(pattern);

    /* Enable the selected digit */
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


/* Clear the display */

void SEG7_Clear(void)
{
    display[0] = 0x00;
    display[1] = 0x00;

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
}


/* Display a number from 0 to 99 */

void SEG7_DisplayNumber(uint8_t number)
{
    uint8_t tens;
    uint8_t ones;

    if (number > 99)
    {
        number = 99;
    }

    tens = number / 10U;
    ones = number % 10U;

    display[0] = digit_lut[tens];
    display[1] = digit_lut[ones];
}


/* Display an individual digit from 0 to 9.
 * The other digit is cleared.
 */

void SEG7_DisplayDigit(uint8_t digit)
{
    if (digit > 9)
    {
        SEG7_DisplayDash();
        return;
    }

    display[0] = 0x00;
    display[1] = digit_lut[digit];
}


/* Display a dash on both digits */

void SEG7_DisplayDash(void)
{
    display[0] = SEG7_DASH;
    display[1] = SEG7_DASH;
}