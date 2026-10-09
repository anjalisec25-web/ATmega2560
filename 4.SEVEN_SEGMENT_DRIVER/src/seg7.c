#include "seg7.h"
#include "gpio.h"


/* ================================
   7-Segment Configuration
   ================================ */

#define SEG7_PORT       GPIO_PORT_A

#define SEG7_TENS_PIN   0
#define SEG7_ONES_PIN   1

#define SEG7_DIGIT_PORT GPIO_PORT_C


/* 
 * Digit control
 *
 * Active HIGH:
 * HIGH -> digit ON
 * LOW  -> digit OFF
 */
#define DIGIT_ON        GPIO_HIGH
#define DIGIT_OFF       GPIO_LOW


/* ================================
   Digit Patterns
   ================================

   PA0 -> a
   PA1 -> b
   PA2 -> c
   PA3 -> d
   PA4 -> e
   PA5 -> f
   PA6 -> g
   PA7 -> dp

   1 = segment ON
   0 = segment OFF
   ================================ */

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


/* 
 * Display buffer
 *
 * display[0] -> tens
 * display[1] -> ones
 */
static volatile uint8_t display[2] = {0, 0};


/* Currently active digit */
static uint8_t active_digit = 0;


/* ================================
   Internal Function
   ================================ */

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


/* ================================
   SEG7_Init
   ================================ */

void SEG7_Init(void)
{
    uint8_t pin;

    /* Make PA0-PA7 outputs */
    for (pin = 0; pin < 8; pin++)
    {
        GPIO_SetDirection(
            SEG7_PORT,
            pin,
            GPIO_OUTPUT
        );
    }

    /* Make digit select pins outputs */
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

    /* Turn both digits OFF */
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

    /* Clear segment outputs */
    SEG7_WritePattern(0x00);

    /* Clear display buffer */
    SEG7_Clear();
}


/* ================================
   SEG7_Refresh
   ================================

   Called repeatedly by timer.

   TENS -> ONES -> TENS -> ONES...
   ================================ */

void SEG7_Refresh(void)
{
    uint8_t pattern;

    /* Turn both digits OFF */
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


    /* Select next digit */
    active_digit ^= 1;


    /* Get segment pattern */
    pattern = display[active_digit];


    /* Output segment pattern */
    SEG7_WritePattern(pattern);


    /* Turn selected digit ON */
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


/* ================================
   SEG7_Clear
   ================================ */

void SEG7_Clear(void)
{
    display[0] = 0x00;
    display[1] = 0x00;
}


/* ================================
   SEG7_DisplayNumber
   ================================ */

void SEG7_DisplayNumber(uint8_t number)
{
    uint8_t tens;
    uint8_t ones;

    /* Maximum value for 2 digits */
    if (number > 99)
    {
        number = 99;
    }

    /* Separate tens and ones */
    tens = number / 10;
    ones = number % 10;

    /* Store patterns */
    display[0] = digit_lut[tens];
    display[1] = digit_lut[ones];
}