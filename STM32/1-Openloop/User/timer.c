

#include "timer.h"


//********************************************************
//initiate TIM2 for PWM control 
//********************************************************
void TIM2_PWM(void)
{
	GPIO_InitTypeDef 						GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   	TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef         	TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_CKD_DIV1: TDTS = Tck_tim, Sample frequency = the input frequency of timer
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	//Sample frequency = the input frequency of timer
	TIM_TimeBaseInitStructure.TIM_Period = PWM_Period -1; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
//********************************************************
//the initial of TIM4 for 1ms 
//********************************************************
void TIM3_1ms(void) 
{
	TIM_TimeBaseInitTypeDef   	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef						NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&NVIC_InitStructure);
}

//********************************************************
//Enable PWM pin PA3
//********************************************************
void PWM_Enable(void)
{
	GPIO_InitTypeDef 						GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);

}


