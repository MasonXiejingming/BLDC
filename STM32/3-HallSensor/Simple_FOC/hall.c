
#include "hall.h"

long cpr;
float full_rotation_offset;
long angle_data_prev;
unsigned long velocity_calc_timestamp;
float angle_prev;

void hallconfig(int hall_u, int hall_v, int hall_w)
{
	hall_u = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
	hall_v = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
	hall_w = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
	
	full_rotation_offset = 0;
	velocity_calc_timestamp = 0;
}



float GetSensorAngle(void)
{
	float angle_data, d_angle;
	
	d_angle = angle_data - angle_data_prev;
	
	if (fabs(d_angle) > (0.8)*cpr) 
		full_rotation_offset += d_angle >0 ? -_2PI : _2PI;
	
	angle_data_prev = angle_data;
	
	return (full_rotation_offset + (angle_data / (float)cpr)* _2PI);
}


float GetSensorVelocity(void)
{
	unsigned long now_us;
	float Ts, angle_current, velocity;
	
	now_us = SysTick -> VAL;
	if (now_us<velocity_calc_timestamp)
		Ts = (float) (velocity_calc_timestamp - now_us)/9*1e-6;
	
	else
		Ts = (float)(0xFFFFFF - now_us + velocity_calc_timestamp)/9*1e-6;
	
	if(Ts == 0 || Ts > 0.5)
		Ts = 1e-3;
	
	angle_current = GetSensorAngle();
	
	velocity = (angle_current - angle_prev) / Ts;
	
	angle_prev = angle_current;
	velocity_calc_timestamp = now_us;
	return velocity;
}
