
#include "lowpassfilter.h"

float y_prev = 0;

float alpha = 0.9;

float lowpassfilter(float x)
{
	float y = alpha * y_prev + (1 - alpha) *x;
	
	y_prev = y;
	
	return y;
}

