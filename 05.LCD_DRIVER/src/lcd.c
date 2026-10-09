#include "lcd.h"
#include "gpio.h"
#include "timer.h"

#define LCD_DATA_PORT       GPIO_PORT_A
#define LCD_D4_PIN          0
#define LCD_D5_PIN          1
#define LCD_D6_PIN          2
#define LCD_D7_PIN          3
#define LCD_CONTROL_PORT    GPIO_PORT_C
#define LCD_RS_PIN          0
#define LCD_EN_PIN          1
#define LCD_CLEAR_DISPLAY   0x01
#define LCD_RETURN_HOME     0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_ON      0x0C
#define LCD_FUNCTION_SET    0x28
#define LCD_LINE1           0x80
#define LCD_LINE2           0xC0

static void LCD_EnablePulse(void);
static void LCD_WriteNibble(uint8_t nibble);

static void LCD_WriteNibble(uint8_t nibble)
{
    GPIO_WritePin(LCD_DATA_PORT, LCD_D4_PIN,
                  (nibble & 0x01) ? GPIO_HIGH : GPIO_LOW);

    GPIO_WritePin(LCD_DATA_PORT, LCD_D5_PIN,
                  (nibble & 0x02) ? GPIO_HIGH : GPIO_LOW);

    GPIO_WritePin(LCD_DATA_PORT, LCD_D6_PIN,
                  (nibble & 0x04) ? GPIO_HIGH : GPIO_LOW);

    GPIO_WritePin(LCD_DATA_PORT, LCD_D7_PIN,
                  (nibble & 0x08) ? GPIO_HIGH : GPIO_LOW);

    LCD_EnablePulse();
}

static void LCD_EnablePulse(void)
{
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_LOW);
    TIMER_DelayUs(1);

    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_HIGH);
    TIMER_DelayUs(1);

    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_LOW);
    TIMER_DelayUs(1);
}

void LCD_Init(void)
{
    GPIO_SetDirection(LCD_DATA_PORT, LCD_D4_PIN, GPIO_OUTPUT);
    GPIO_SetDirection(LCD_DATA_PORT, LCD_D5_PIN, GPIO_OUTPUT);
    GPIO_SetDirection(LCD_DATA_PORT, LCD_D6_PIN, GPIO_OUTPUT);
    GPIO_SetDirection(LCD_DATA_PORT, LCD_D7_PIN, GPIO_OUTPUT);

    GPIO_SetDirection(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_OUTPUT);
    GPIO_SetDirection(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_OUTPUT);

    GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_LOW);
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_EN_PIN, GPIO_LOW);

    TIMER_DelayUs(20000);

    LCD_WriteNibble(0x03);
    TIMER_DelayUs(5000);

    LCD_WriteNibble(0x03);
    TIMER_DelayUs(200);

    LCD_WriteNibble(0x03);
    TIMER_DelayUs(200);

    LCD_WriteNibble(0x02);
    TIMER_DelayUs(200);

    LCD_Command(LCD_FUNCTION_SET);
    LCD_Command(LCD_DISPLAY_ON);
    LCD_Command(LCD_ENTRY_MODE);
    LCD_Clear();
}

void LCD_Command(uint8_t command)
{
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_LOW);

    LCD_WriteNibble((uint8_t)(command >> 4));
    LCD_WriteNibble((uint8_t)(command & 0x0F));

    TIMER_DelayUs(50);

    if ((command == LCD_CLEAR_DISPLAY) ||
        (command == LCD_RETURN_HOME))
    {
        TIMER_DelayUs(2000);
    }
}


void LCD_WriteChar(char data)
{
    GPIO_WritePin(LCD_CONTROL_PORT, LCD_RS_PIN, GPIO_HIGH);

    LCD_WriteNibble((uint8_t)data >> 4);
    LCD_WriteNibble((uint8_t)data & 0x0F);

    TIMER_DelayUs(50);
}

void LCD_WriteString(const char *string)
{
    if (string == 0)
        return;

    while (*string != '\0')
    {
        LCD_WriteChar(*string);
        string++;
    }
}

void LCD_Clear(void)
{
    LCD_Command(LCD_CLEAR_DISPLAY);
}


void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if ((row > 1) || (column > 15))
        return;

    if (row == 0)
        address = LCD_LINE1 + column;
    else
        address = LCD_LINE2 + column;

    LCD_Command(address);
}

void LCD_PrintNumber(uint16_t number)
{
    char buffer[5];
    uint8_t i = 0;

    if (number == 0)
    {
        LCD_WriteChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i++] = (char)('0' + (number % 10));
        number /= 10;
    }

    while (i > 0)
    {
        LCD_WriteChar(buffer[--i]);
    }
}