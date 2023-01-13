
//********************************************************
//the dealy method named query the timer,
//we utilize the timer but disable interrupt,
//solved the problem that use interrupt frequently 
//and ensure the accuracy of delay
//********************************************************

#include "delay.h"

//********************************************************
//delay n_us
//********************************************************

void delay_us (u32 n_us)
{
	unsigned long temp;
	
	SysTick -> LOAD = n_us*9;
	// 9 for 72Mhz maximum, LOAD is register of automatic reloading
	
	SysTick -> VAL = 0x00;
	// VAL is register of current value, clear counter
	
	SysTick -> CTRL = 0x01;
	// CTRL is register of control and status, enable SysTick Timer
	
	do temp = SysTick -> CTRL;
	
	while ((temp&0x01) && ! (temp&(1<<16)));
	
	SysTick -> CTRL = 0x00;
	SysTick -> VAL = 0x00;
}

//********************************************************
//delay n_ms
//the Maximum_delay_time=0xFFFFFF/9MHz=1864ms
//********************************************************
void delay_ms (u16 n_ms)
{
	unsigned long temp;
	
	SysTick -> LOAD = (u32)n_ms*9*1000;
	// *1000 because the ms large 1000 times than us
	
	SysTick -> VAL = 0x00;
	SysTick -> CTRL = 0x01;
	
	do temp = SysTick -> CTRL;
	
	while ((temp&0x01) && ! (temp&(1<<16)));
	
	SysTick -> CTRL = 0x00;
	SysTick -> VAL = 0x00;
}

/********************************************************/
//Counting from 0xffffff to 0 circularly
/********************************************************/
void systick_count(void)
{
	SysTick -> LOAD = 0xffffff - 1;
	// set reloading register
	
	SysTick -> VAL = 0x00;
	SysTick -> CTRL = 0x01;
	//enable systick timer
}
