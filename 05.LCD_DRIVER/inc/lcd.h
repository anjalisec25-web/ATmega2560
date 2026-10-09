#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCD_Init(void);
void LCD_Command(uint8_t command);
void LCD_WriteChar(char data);
void LCD_WriteString(const char *string);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_PrintNumber(uint16_t number);

#endif
