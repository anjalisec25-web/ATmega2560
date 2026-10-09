#include "ultra.h"
#include "gpio.h"
#include "timer.h"

/* ================================
   Ultrasonic Configuration
   ================================ */

#define ULTRA_PORT       GPIO_PORT_B

#define ULTRA_TRIG_PIN   0
#define ULTRA_ECHO_PIN   1


/* ================================
   Timer3 Registers
   ================================ */

#define TCCR3A   (*(volatile unsigned char *)0x90)
#define TCCR3B   (*(volatile unsigned char *)0x91)

#define TCNT3L   (*(volatile unsigned char *)0x94)
#define TCNT3H   (*(volatile unsigned char *)0x95)

#define TIFR3    (*(volatile unsigned char *)0x38)

#define CS30     0
#define CS31     1
#define CS32     2

#define TOV3     0


/* ================================
   ULTRA_Init
   ================================ */

void ULTRA_Init(void)
{
    /* TRIG -> OUTPUT */
    GPIO_SetDirection(
        ULTRA_PORT,
        ULTRA_TRIG_PIN,
        GPIO_OUTPUT
    );

    /* ECHO -> INPUT */
    GPIO_SetDirection(
        ULTRA_PORT,
        ULTRA_ECHO_PIN,
        GPIO_INPUT
    );

    /* TRIG initially LOW */
    GPIO_WritePin(
        ULTRA_PORT,
        ULTRA_TRIG_PIN,
        GPIO_LOW
    );

    /* Timer3 Normal Mode */
    TCCR3A = 0x00;
    TCCR3B = 0x00;

    /* Clear Timer3 counter */
    TCNT3H = 0x00;
    TCNT3L = 0x00;

    /* Clear overflow flag */
    TIFR3 |= (1 << TOV3);
}


/* ================================
   ULTRA_GetDistance
   ================================ */

uint16_t ULTRA_GetDistance(void)
{
    uint16_t count;
    uint16_t distance;
    uint32_t timeout;


    /* ============================
       Send 10 us Trigger Pulse
       ============================ */

    GPIO_WritePin(
        ULTRA_PORT,
        ULTRA_TRIG_PIN,
        GPIO_LOW
    );

    TIMER_DelayUs(2);

    GPIO_WritePin(
        ULTRA_PORT,
        ULTRA_TRIG_PIN,
        GPIO_HIGH
    );

    TIMER_DelayUs(10);

    GPIO_WritePin(
        ULTRA_PORT,
        ULTRA_TRIG_PIN,
        GPIO_LOW
    );


    /* ============================
       Wait for ECHO HIGH
       ============================ */

    timeout = 0;

    while(GPIO_ReadPin(ULTRA_PORT, ULTRA_ECHO_PIN) == GPIO_LOW)
    {
        timeout++;

        if(timeout > 60000)
            return 0;
    }


    /* ============================
       Start Timer3
       Prescaler = 64
       ============================ */

    TCNT3H = 0x00;
    TCNT3L = 0x00;

    TIFR3 |= (1 << TOV3);

    TCCR3B |= (1 << CS31) | (1 << CS30);


    /* ============================
       Wait for ECHO LOW
       ============================ */

    timeout = 0;

    while(GPIO_ReadPin(ULTRA_PORT, ULTRA_ECHO_PIN) == GPIO_HIGH)
    {
        timeout++;

        if(timeout > 60000)
        {
            TCCR3B &= ~((1 << CS32) |
                        (1 << CS31) |
                        (1 << CS30));

            return 0;
        }
    }


    /* ============================
       Stop Timer3
       ============================ */

    TCCR3B &= ~((1 << CS32) |
                (1 << CS31) |
                (1 << CS30));


    /* ============================
       Read Timer3 Count
       ============================ */

    count = ((uint16_t)TCNT3H << 8) | TCNT3L;


    /* ============================
       Calculate Distance
       ============================

       CPU Clock = 16 MHz
       Prescaler = 64

       Timer frequency = 250 kHz
       1 timer tick = 4 us

       Distance(cm) = Time(us) / 58

       Distance = (count * 4) / 58
    */

    distance = (uint16_t)(((uint32_t)count * 4) / 58);

    return distance;
}