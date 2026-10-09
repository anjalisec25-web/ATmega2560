#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_Init(void);
void PWM_Start(void);
void PWM_Stop(void);
void PWM_SetDutyCycle(uint8_t duty);

#endif