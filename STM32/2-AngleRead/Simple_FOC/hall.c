
#include "hall.h"


const int8_t ELECTRIC_SECTORS[8] = {-1, 0, 4, 5, 2, 1, 3, -1};

long cpr = 0;
float full_rotation_offset = 0;
long angle_data_prev = 0;
unsigned long velocity_calc_timestamp = 0;
float angle_prev = 0;
#define velocity_max (float)1000;
#define pole_pairs  13

int HU;
int HV; 
int HW;
int hall_state;
int new_hall_state;
int electric_sector;
int new_electric_sector;
long electric_rotations;
long total_interrupt;
unsigned long pulse_timestamp;
long pulse_diff;
Type_Direction direction;

void hallconfig()
{
	
	unsigned long now_us;
	int new_hall_state;
	int new_electric_sector;
	
	static Type_Direction old_direction ;
	
	electric_rotations = 0;
	now_us = SysTick -> VAL;
	
	HU = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	HV = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	HW = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
	cpr= pole_pairs *6;
	
	new_hall_state = HU + (HV<<1) + (HW<<2);
	
	if (new_hall_state == hall_state)
		return;
	
	new_electric_sector = ELECTRIC_SECTORS[new_hall_state];
	
	
	
	
	if (new_electric_sector - electric_sector >3)
	{
		direction = CCW;
		electric_rotations += direction;
	}
	
	else if (new_electric_sector - electric_sector >(-3))
	{
		direction = CW;
		electric_rotations += direction;
	}
	
	else
		direction = (new_electric_sector > electric_sector)? CW : CCW;
	
	electric_sector = new_electric_sector;
	
	if (direction == old_direction) 
		pulse_diff = now_us - pulse_timestamp;

	else 
		pulse_diff = 0;
	
	pulse_timestamp = now_us;
	total_interrupt ++;
	old_direction = direction;
	
}	

float getAngle()
{
	float angle;
	angle = ((float)(electric_rotations * 6 + electric_sector) / (float)cpr) * _2PI ;
	
	return (angle);
	
}


//float getVelocity()
//{
//	unsigned long now_us;
//	float vel;
//	now_us = SysTick -> VAL;
//	
//	if (pulse_diff == 0 || ((long)now_us - pulse_timestamp) > pulse_diff)
//		return 0;
//	
//	else
//	{
//		vel = direction*(_2PI / (float)cpr) / (pulse_diff / (float)1000000);
//		if (vel < -velocity_max || vel > velocity_max)
//			vel = (float)0;
//		
//		return vel;
//	}
//	

//}



