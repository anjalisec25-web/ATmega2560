#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/*

* 16x2 HD44780 LCD Driver
* 4-bit mode
*
* LCD DATA:
* D4 -> PA0
* D5 -> PA1
* D6 -> PA2
* D7 -> PA3
*
* CONTROL:
* RS -> PC0
* EN -> PC1
* RW -> GND
  */

void LCD_Init(void);
void LCD_Command(uint8_t command);
void LCD_WriteChar(char data);
void LCD_WriteString(const char *string);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_PrintNumber(uint16_t number);

#endif
