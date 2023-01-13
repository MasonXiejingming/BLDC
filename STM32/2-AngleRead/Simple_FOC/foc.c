
#include "foc.h"
#include "usart.h"


float 				shaft_angle;
float 				shaft_angle_sp;
float 				shaft_velocity;
float 				shaft_velocity_sp;
float 				current_sp;
float 				electrical_angle;
int						pole_pairs;
DQVoltage_s 	voltage;
DQCurrent_s   current;

Type_Direction				Direction;
Type_TorqueCtrl 			torque_controller;
Type_MotionCtrl 			controller;
Type_FOCModulation		modulation;
Type_FOCMotorStatus	  motor_status;


float 				sensor_offset=0;
float 				zero_electric_angle;
int 					sensor_direction;

float 				voltage_limit;
float 				current_limit;
float 				velocity_limit;

float 				voltage_power_supply;
float 				voltage_sensor_align;
unsigned long open_loop_timestamp;

float electricalAngle(void)
{
	return _normalizeAngle((shaft_angle+sensor_offset)*pole_pairs-zero_electric_angle);
}

//float shaftVelocity(void)
//{
//	return sensor_direction * lowpassfilter(GetSensorVelocity());
//}


//float shaftAngle(void)
//{
//	return sensor_direction * GetSensorAngle()- sensor_offset;
//}

