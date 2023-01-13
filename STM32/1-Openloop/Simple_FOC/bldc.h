#ifndef BLDC_H
#define BLDC_H


#include "foc.h"
#include "stm32f10x.h"
#include "timer.h"
#include "delay.h"
#include "utils.h"
#include "stdio.h"
#include "usart.h"

void move(float new_target);
void setPhaseVoltage(float Uq, float Ud, float angle_electric);

float velocityOpenloop(float target_velocity);
float angleOpenloop(float target_angle);

#endif


