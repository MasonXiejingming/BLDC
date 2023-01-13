
#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"


#define PWM_Period  1440


void TIM2_PWM(void);
void TIM3_1ms(void);
void Hall_Init(void);

#endif
