
#include "stm32f10x.h"
#include "stdlib.h"
#include "headerfiles.h"


int main(void)
{
	usart1_init(115200);
	TIM2_PWM();
	TIM3_1ms();
	PWM_Enable();
	Hall_Init();
	
	delay_ms(1000);
	
	voltage_power_supply 	= 8;
	voltage_limit        	= 1;
	velocity_limit				= 20;
	controller 						= Angle;
	pole_pairs 						= 6;
	
	MotorEnable;
	target = 0;
	Motor_init();
	Motor_FOC_init();
	PID_Init();
	printf("Motor is ready. \r\n");
	
	systick_count();
	// we can't call delay_us and delay_ms anymore
	
	while(1)
	{
		move(target);
		loopFOC();
		commander_run();
	}	
}






