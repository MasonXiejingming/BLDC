

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

//********************************************************
//initiate handles for hallsensor
//********************************************************
void Hall_Init(void)
{
	GPIO_InitTypeDef 						GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   	TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef						TIM_ICInitStructure;
	TIM_OCInitTypeDef         	TIM_OCInitStructure;
	NVIC_InitTypeDef						NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;
	TIM_ICInitStructure.TIM_ICFilter = 0x04;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0x1;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init (&NVIC_InitStructure);

	TIM_SelectHallSensor(TIM4, ENABLE);
	//enable hallsensor
	
	TIM_SelectInputTrigger(TIM4, TIM_TS_TI1F_ED);
	//TI1F_ED: falling and rising edges of input are used
	TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_OC2Ref);
	//trigger output
	
	
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
	
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	//add CC2 handle to reduce delay
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	
	TIM_ITConfig(TIM4, TIM_IT_Trigger, ENABLE);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void TIM_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
	//	HALL_ChangePhase();
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_Trigger) != RESET)
	{
	//	HALL_ChangePhase();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);
	}
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
	//	Count++;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

