
#ifndef DELAY_H
#define DELAY_H


#include "stm32f10x.h"
#include "stdint.h"

void delay_us(u32 n_us);
void delay_ms(u16 n_ms);
void systick_count(void);


#endif

