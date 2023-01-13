#ifndef FOC_H
#define FOC_H

#include "utils.h" 
#include "hall.h"
#include "lowpassfilter.h"

//********************************************************
//direction types of motor
//********************************************************
typedef enum
{
	CW 				=1,	
	//clockwise
	
	CCW				=-1,
	//counter clockwise
	
	UNKNOWN 	=0
}Type_Direction;

//********************************************************
//motion control types
//********************************************************
typedef enum
{
	Torque,
	Velocity,
	Angle,
	Velocity_Openloop,
	Angle_Openloop
}Type_MotionCtrl;

//********************************************************
//torque control types
//********************************************************
typedef enum
{
	dc_voltage,
	dc_current,
	foc_current
}Type_TorqueCtrl;

//********************************************************
//foc modulation types
//********************************************************
typedef enum
{
	SinePWM,
	SpaceVectorPWM,
	Trapezoid_120,
	Trapezoid_150
}Type_FOCModulation;

//********************************************************
//foc motor status
//********************************************************
typedef enum
{
	motor_uninitialized,
	motor_initializing,
	motor_uncalibrated,
	motor_calibrating,
	motor_read,
	motor_error,
	motor_calib_faied,
	motor_init_failed
}Type_FOCMotorStatus;


extern Type_Direction				direction;
extern Type_TorqueCtrl 			torque_controller;
extern Type_MotionCtrl 			controller;
extern Type_FOCModulation		modulation;
extern Type_FOCMotorStatus	motor_status;


//********************************************************
//state variables
//********************************************************									

extern float 				shaft_angle;
										//current motor angle 

extern float 				shaft_angle_sp;
										//current target angle

extern float 				shaft_velocity;
										//current motor velocity

extern float 				shaft_velocity_sp;
										//current target velocity
										
extern float 				current_sp;
										//target current
										
extern float 				electrical_angle;
										//current electrical angle
										
extern int					pole_pairs;
										//the number of pole_pairs
										
extern DQVoltage_s 	voltage;
										//current d and q voltages set to the motor
extern DQCurrent_s  current;
										//vurrent d and q current measured
										
//********************************************************
//sensor variables
//********************************************************										
extern float 				sensor_offset;
										//user defined sensor zero offset 
										
extern float 				zero_electric_angle;
										//absolute zero electic angle
										
extern int 					sensor_direction;
										//if sensor direction == CCW, 
										//then direction wiil be flipped to CW
										

//********************************************************
//limiting variables
//********************************************************
extern float 				voltage_limit;
extern float 				current_limit;
extern float 				velocity_limit;

extern float 				voltage_power_supply;
extern float 				voltage_sensor_align;
extern float 				velocity_index_search;
extern unsigned long open_loop_timestamp;


float electricalAngle(void);
float shaftVelocity(void);
float shaftAngle(void);

#endif

