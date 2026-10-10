#ifndef SEG7_H
#define SEG7_H

#include <stdint.h>

void SEG7_Init(void);
void SEG7_Refresh(void);
void SEG7_Clear(void);
void SEG7_DisplayDigit(uint8_t digit);
void SEG7_DisplayNumber(uint8_t number);
void SEG7_DisplayDash(void);

#endif