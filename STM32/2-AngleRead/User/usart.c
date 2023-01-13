
#include "usart.h"

//********************************************************
//Support the function of printf 
//********************************************************

#if 1

#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	while((USART1 -> SR&0x40) == 0);
	USART1 -> DR = (u8)ch;
	return ch;
}
#endif 

unsigned char USART_RX_BUFFER[USART_REC_LEN];
// receiver buffer 
// bit15: mark of received successfully
// bit14: 0x0D
// bit13~0: the number of byte

unsigned short USART_RX_STA = 0;
// receive status

void usart1_init(unsigned long bound)
{
	GPIO_InitTypeDef 						GPIO_InitStructure;
	USART_InitTypeDef						USART_InitStructure;
	NVIC_InitTypeDef						NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	//USART1_TX, PA_9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1_RX, PA_10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//configuration of usart1 initiated
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	//configuration of usart1 NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	//enable the function interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//enable usart1
	USART_Cmd(USART1, ENABLE);
}

//********************************************************
//process of interrupt with USART1
//********************************************************
//void USART1_IRQHandler(void)   
//{	
//	unsigned char Read;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		Read = USART_ReceiveData(USART1);
//		//read the receiving data
//		
//		//the receive doesn't finish
//		if((USART_RX_STA&0x8000) == 0)
//		{
//			//when it has received 0x0D
//			if(USART_RX_STA&0x4000)
//			{
//				//when receive wrong, again
//				if(Read!=0x0a) 
//					USART_RX_STA = 0;
//				else 
//				{
//					USART_RX_STA |= 0x8000;
//					//done the receive
//					
//					USART_RX_BUFFER[USART_RX_STA&0x3FFF] = '\0';
//					//the last one bit is added with 0, 
//					//it's better to make a decision. 
//				}
//			}
//			
//			// not yet received 0x0D
//			else
//			{
//				if(Read==0x0D)
//					USART_RX_STA |= 0x4000;
//				else
//				{
//					USART_RX_BUFFER[USART_RX_STA&0x3FFF] = Read;
//					USART_RX_STA++;
//					
//					if(USART_RX_STA > (USART_REC_LEN-1))
//						USART_RX_STA = 0;
//						//receive wrong, again
//				}
//			}
//		}
//	}
//}




