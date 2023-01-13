
#include "bldc.h"


void move(float new_target)
{
	

	
	switch (controller)
	{
		case Velocity_Openloop:
			shaft_velocity_sp = new_target;
			voltage.q = velocityOpenloop(shaft_velocity_sp);
			voltage.d = 0;
			break;
		
		case Angle_Openloop:
			shaft_angle_sp = new_target;
			voltage.q = angleOpenloop(shaft_angle_sp);
			voltage.d = 0;
			break;
	}
}

void setPhaseVoltage(float Uq, float Ud, float angle_electric)
{
	float T0, T1, T2;
	float Ta, Tb, Tc;
	float Uout;
	u32 	sector;
	
	switch (modulation)
	{
		case SinePWM:
			
		case Trapezoid_120:
			
		case Trapezoid_150:
			
		case SpaceVectorPWM:
			
			//only if Ud and Uq set
			if(Ud)
			{
				Uout = _sqrt(Ud*Ud+Uq*Uq) / voltage_power_supply;
				angle_electric = _normalizeAngle(angle_electric + atan2(Uq, Ud));
			}
	
			//only Uq available - no need for atan2 and sqrt
			else 
			{
				Uout = Uq/voltage_power_supply;
				angle_electric = _normalizeAngle(angle_electric +_PI_2);
			}
			
			if (Uout>0.577) 
				Uout=0.577;
			if (Uout<-0.577) 
				Uout=-0.577;
			
			sector = (angle_electric / _PI_3) +1;
			T1 = _SQRT3 * _sin(sector * _PI_3 - angle_electric) * Uout;
			T2 = _SQRT3 * _sin(angle_electric - (sector - 1) * _PI_3) * Uout;
			T0 = 1 - T1 - T2;
			
			//calculate the duty cycles (times)
			switch(sector)
			{
				case 1:
					Ta = T1+T2+T0/2;
					Tb = T2+T0/2;
					Tc = T0/2;
					break;
		
				case 2:
					Ta = T1+T0/2;
					Tb = T1+T2+T0/2;
					Tc = T0/2;
					break;
		
				case 3:
					Ta = T0/2;
					Tb = T1+T2+T0/2;
					Tc = T2+T0/2;
					break;
		
				case 4:
					Ta = T0/2;
					Tb = T1+T0/2;
					Tc = T1+T2+T0/2;
					break;
		
				case 5:
					Ta = T2+T0/2;
					Tb = T0/2;
					Tc = T1+T2+T0/2;
					break;
		
				case 6:
					Ta = T1+T2+T0/2;
					Tb = T0/2;
					Tc = T1+T0/2;
					break;
			
				default:
					Ta = 0;
					Tb = 0;
					Tc = 0;
			}
	
			TIM_SetCompare1(TIM2, Ta*PWM_Period); 
			//is compare register, and corresponding the CCR1 in TIM_OC1Init
			
			TIM_SetCompare2(TIM2, Tb*PWM_Period);
			//is compare register, and corresponding the CCR2 in TIM_OC1Init
			
			TIM_SetCompare3(TIM2, Tc*PWM_Period);
			//is compare register, and corresponding the CCR3 in TIM_OC1Init
			
			break;
			
		}
}
float velocityOpenloop(float target_velocity)
{
	unsigned long now_us;
	float Ts, Uq;
	
	now_us = SysTick -> VAL;
	
	if (now_us < open_loop_timestamp)
		Ts = (float)(open_loop_timestamp - now_us)/9*1e-6;
		// 1e-6 is micro second, and corresponding to the MHz.
	
	else 
		Ts = (float)(0xFFFFFF - now_us + open_loop_timestamp)/9*1e-6;
	
	open_loop_timestamp = now_us;
	//save timestamp for the next call
	
	if(Ts == 0 || Ts > 0.5)
		Ts = 1e-3;
	
	shaft_angle = _normalizeAngle(shaft_angle + target_velocity*Ts);
	
	Uq = voltage_limit;
	
	setPhaseVoltage(Uq, 0, _electricalAngle(shaft_angle, pole_pairs));
	
	return Uq;
}

float angleOpenloop(float target_angle)
{
	unsigned long now_us;
	float Ts, Uq;
	
	now_us = SysTick -> VAL;
	
	if (now_us < open_loop_timestamp)
		Ts = (float)(open_loop_timestamp - now_us)/9*1e-6;
	else 
		Ts = (float)(0xFFFFFF - now_us + open_loop_timestamp)/9*1e-6;
	
	open_loop_timestamp = now_us;
	//save timestamp for the next call
	
	if(Ts == 0 || Ts > 0.5)
		Ts = 1e-3;
	
	if(fabs(target_angle - shaft_angle) > velocity_limit*Ts)
		shaft_angle += _sign(target_angle - shaft_angle) * velocity_limit*Ts;
	
	else 
		shaft_angle = target_angle;
	
	Uq = voltage_limit;
	
	setPhaseVoltage(Uq, 0, _electricalAngle(shaft_angle, pole_pairs));
	
	return Uq;
}


