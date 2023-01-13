#ifndef HALL_H
#define HALL_H

#include "timer.h"
#include "delay.h"
#include "utils.h"

extern long cpr;
extern float full_rotation_offset;
extern long angle_data_prev;
extern unsigned long velocity_calc_timestamp;
extern float angle_prev;


void hallconfig(int hall_u, int hall_v, int hall_w);

float GetSensorAngle(void);
float GetSensorVelocity(void);

#endif

