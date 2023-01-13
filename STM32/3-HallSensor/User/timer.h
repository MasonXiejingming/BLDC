
#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"


#define PWM_Period  1440
#define MotorEnable GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define MotorDisable GPIO_ResetBits(GPIOA, GPIO_Pin_3)


void TIM2_PWM(void);
void TIM3_1ms(void);
void PWM_Enable(void);
void Hall_Init(void);
//void TIM4_IRQHandler(void);

#endif
