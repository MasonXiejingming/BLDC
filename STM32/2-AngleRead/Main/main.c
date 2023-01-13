
#include "stm32f10x.h"
#include "stdlib.h"
#include "headerfiles.h"

float ang1;

int main(void)
{
//	unsigned int count_i=0;
	usart1_init(115200);
	TIM2_PWM();
	TIM3_1ms();
	Hall_Init();
	hallconfig();
	
	delay_ms(1000);
	
	printf("Motor is ready. \r\n");

	systick_count();
	// we can't call delay_us and delay_ms anymore
	
	while(1)
	{ 
		
		ang1 = getAngle();      
		
		printf("M1:%.4f\r\n",ang1);  
   
		delay_ms(100);
	}
	
}






