#ifndef TIMER_H
#define TIMER_H

void TIMER_Init(void);
void TIMER_Start(void);
void TIMER_Stop(void);
unsigned char TIMER_GetOverflowFlag(void);
void TIMER_ClearOverflowFlag(void);
void TIMER_DelayUs(unsigned int microseconds);

#endif