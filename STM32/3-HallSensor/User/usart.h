
#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "timer.h"

#define USART_REC_LEN 256

extern unsigned char USART_RX_BUFFER[USART_REC_LEN];
extern unsigned short USART_RX_STA;
extern float target;

void usart1_init(unsigned long bound);
void commander_run(void);

#endif
