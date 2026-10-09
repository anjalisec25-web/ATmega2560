#ifndef SEG7_H
#define SEG7_H

#include <stdint.h>

/*
 * 2-Digit 7-Segment Display
 *
 * Segment connections:
 * PA0 -> a
 * PA1 -> b
 * PA2 -> c
 * PA3 -> d
 * PA4 -> e
 * PA5 -> f
 * PA6 -> g
 * PA7 -> dp
 *
 * Digit select:
 * PC0 -> Tens
 * PC1 -> Ones
 */

void SEG7_Init(void);
void SEG7_Refresh(void);
void SEG7_Clear(void);
void SEG7_DisplayNumber(uint8_t number);

#endif