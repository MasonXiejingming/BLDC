#ifndef HALL_H
#define HALL_H

#include "timer.h"
#include "delay.h"
#include "utils.h"
#include "foc.h"

extern int HU;
extern int HV; 
extern int HW;

extern int hall_state;
// the current 3bit state of the hall sensors

extern int new_hall_state ;

extern int electric_sector;
// the current sector of the sensor. Each sector is 60deg electrical

extern int new_electric_sector;

extern long electric_rotations;
// the number of electric rotations

extern long total_interrupt;
// this is sometimes useful to identify interrupt issues (e.g. weak or no pullup resulting in 1000s of interrupts)

extern unsigned long pulse_timestamp;
//!< last impulse timestamp in us

extern long pulse_diff;

extern float velocity_max;
					
extern Type_Direction direction;


extern long cpr;
extern float full_rotation_offset;
extern long angle_data_prev;
extern unsigned long velocity_calc_timestamp;
extern float angle_prev;

extern void hallconfig(void);
extern void SensorInit(void);
extern void update(void);
extern void Sensorupdate(void);
extern float getAngle(void);


//extern float getVelocity();

#endif

