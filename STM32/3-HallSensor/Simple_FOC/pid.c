
#include "pid.h"

float P_velocity;
float I_velocity;
float ramp_output_velocity;
float integral_prev_velocity;
float error_prev_velocity;
float output_prev_velocity;

unsigned long timestamp_velocity;


float P_angle;
float D_angle;
float error_prev_angle;

unsigned long timestamp_angle;

void PID_Init (void)
{
	P_velocity 									= 0.1;
	I_velocity 									= 2;
	ramp_output_velocity 				= 100;
	//output derivative limit [V/s]
	
	integral_prev_velocity			= 0;
	error_prev_velocity					= 0;
	output_prev_velocity				= 0;
	timestamp_velocity					= SysTick->VAL;
	
	
	P_angle											= 10;
	D_angle											= 0.5;
	error_prev_angle						= 0;
	timestamp_angle							= SysTick->VAL;
}

float PID_velocity(float error_velocity)
{
	unsigned long now_us;
	
	float Ts;
	float proportional, integral, output;
	float output_rate;
	
	now_us = SysTick->VAL;
	
	if (now_us < timestamp_velocity)
		Ts = (float) (timestamp_velocity - now_us)/9*1e-6;
	else 
		Ts = (float) (0xffffff - now_us + timestamp_velocity)/9*1e-6; 
	
	timestamp_velocity = now_us;
	
	if (Ts == 0 || Ts > 0.5)
		Ts = 1e-3;
	
	proportional = P_velocity * error_velocity;
	//u(s) = (P + I/s + Ds)e(s)
	//Discrete implementations
	//proportional part
	//u_p = P * e(k)
	
	integral = integral_prev_velocity + I_velocity * Ts * 0.5 * (error_velocity + error_prev_velocity);
	// Tustin transform of the integral part
	// u_ik = u_ik_prev  + I*Ts/2*(ek + ek_prev)
	
	integral = _constrain(integral, - voltage_limit, voltage_limit);
	//limit the output values for antiwindup
	
	output = proportional + integral;
	
	output = _constrain(output, - voltage_limit, voltage_limit);
	
	output_rate = (output - output_prev_velocity) / Ts;
	
	if(output_rate > ramp_output_velocity) 
		output = output_prev_velocity + ramp_output_velocity * Ts;
	
	else if (output_rate < -ramp_output_velocity)
		output = output_prev_velocity - ramp_output_velocity * Ts;
	
	integral_prev_velocity = integral;
	output_prev_velocity = output;
	error_prev_velocity = error_velocity;
	
	return output;
}

float PID_angle(float error_angle)
{
	unsigned long now_us;
	
	float Ts;
	float proportional, derivative, output;
	
	now_us = SysTick->VAL;
	
	if (now_us < timestamp_angle)
		Ts = (float) (timestamp_angle - now_us)/9*1e-6;
	else 
		Ts = (float) (0xffffff - now_us + timestamp_angle)/9*1e-6; 
	
	timestamp_angle = now_us;
	
	if (Ts == 0 || Ts > 0.5)
		Ts = 1e-3;
	
	proportional = P_angle * error_angle;
	//u(s) = (P + I/s + Ds)e(s)
	//Discrete implementations
	//proportional part
	//u_p = P * e(k)
	
	derivative = D_angle * (error_angle - error_prev_angle) / Ts;
	//u_dk = D(ek - ek_prev) / Ts
	
	
	output = proportional + derivative;
	
	output = _constrain(output, - voltage_limit, voltage_limit);
	
	error_prev_angle = error_angle;
	
	return output;
	
}

