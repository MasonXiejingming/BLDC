
#include "bldc.h"


void move(float new_target)
{
	
	shaft_velocity = shaftVelocity();
	
	switch (controller)
	{
		case Torque:
			if (torque_controller == dc_voltage)
				voltage.q = new_target;
			else
				current_sp = new_target;
			break;
			
		case Velocity:
			shaft_velocity_sp = new_target;
			current_sp = PID_velocity(shaft_velocity_sp - shaft_velocity);
			//calculate the torque command
			
			if (torque_controller == dc_voltage)
			{
				voltage.q = current_sp;
				voltage.d = 0;
			}
			break;
			
		case Angle:
			shaft_angle_sp = new_target;
			shaft_velocity_sp = PID_angle(shaft_angle_sp - shaft_angle);
			current_sp = PID_velocity(shaft_velocity_sp - shaft_velocity);
			if(torque_controller == dc_voltage)
			{
				voltage.q = current_sp;
				voltage.d = 0;
			}
			break;
			
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

int alignSensor(void)
{
	long i;
	float angle;
	float middle_angle, end_angle;
	float moved;
	
	printf("Motor: Align sensor.\r\n");
	
	//find natural direction;
	//move one electrical revolution forward;
	for (i=0; i<=500; i++)
	{
		angle = (float) (_3PI_2 + _2PI * i / 500);
		setPhaseVoltage(voltage_sensor_align, 0, angle);
		delay_ms(2);
	}
	middle_angle = GetSensorAngle();
	
	for (i=500; i>=0; i--)
	{
		angle = (float) (_3PI_2 + _2PI * i / 500);
		setPhaseVoltage(voltage_sensor_align, 0, angle);
		delay_ms(2);
	}
	end_angle = GetSensorAngle();
	
	setPhaseVoltage(0,0,0);
	delay_ms(200);
	
	printf("middle_angle=%.4f\r\n", middle_angle);
	printf("end_angle=%.4f\r\n", end_angle);
		
	moved = fabs(middle_angle - end_angle);
	if ((middle_angle == end_angle) || (moved < 0.02))
	{
		printf ("Motor: Failed to notice movement loop222. \r\n");
		MotorDisable;
		return 0;
	}
	
	else if (middle_angle < end_angle)
	{
		printf ("Motor: sensor_direction = CCW. \r\n");
		sensor_direction = CCW;
	}
	
	else if (middle_angle > end_angle)
	{
		printf ("Motor: sensor_direction = CW. \r\n");
		sensor_direction = CW;
	}
	
	// check the pole_pairs
	printf ("Motor: pole_pairs check: ");
	if (fabs(moved*pole_pairs - _2PI) > 0.5)
		//0.5 is an arbitrary number, it could be lower or higher
	{
		printf ("fail to estimate pole_pairs");
		pole_pairs = _2PI/moved +0.5;
		//approximating
		printf ("%d\r\n,", pole_pairs);
	}
	else
		printf ("OK! \r\n");
	
	//calculate the angle of zero bias
	setPhaseVoltage(voltage_sensor_align, 0, _3PI_2);
	delay_ms(700);
	zero_electric_angle = _normalizeAngle(_electricalAngle(sensor_direction*GetSensorAngle(), pole_pairs));
	delay_ms(20);
  printf ("Motor: Zero electic angle: %.4f\r\n" , zero_electric_angle);
	
	setPhaseVoltage(0,0,0);
	delay_ms(200);
	
	return 1;
}

void loopFOC(void)
{
	if (controller == Velocity_Openloop || controller == Angle_Openloop) 
		return ; 
	
	shaft_angle = shaftAngle();
	electrical_angle = electricalAngle();
	
	switch(torque_controller)
	{
		case dc_voltage:
			break;
		case dc_current:
			break;
		case foc_current:
			break;
		default:
			printf("Motor: no torque controll selected.");
			break;
	}
	
	setPhaseVoltage(voltage.q, voltage.d, electrical_angle);
}

void Motor_init(void)
{
	printf("Motor: Init\r\n");
	
	//new_voltage_limit = current_limit * phase_resistance;
	//voltage_limit = new_voltage_limit < voltage_limit ? new_voltage_limit : voltage_limit;
	if (voltage_sensor_align > voltage_limit)
		voltage_sensor_align = voltage_limit;
	
	pole_pairs = 6;
	sensor_direction = UNKNOWN;
	
	MotorEnable;
	printf("Motor: enable driver. \r\n");
	
}

void Motor_FOC_init(void)
{
	alignSensor();
	delay_ms(5);
	shaft_velocity = shaftVelocity();
	delay_ms(5);
	shaft_angle = shaftAngle();
	if(controller == Angle)
		target = shaft_angle;
	delay_ms(200);
}

