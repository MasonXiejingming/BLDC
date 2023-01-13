
#ifndef PID_H
#define PID_H

#include "utils.h"
#include "delay.h"
#include "foc.h"

float PID_velocity(float error_velocity);
float PID_angle(float error_angle);


void PID_Init (void);

#endif

