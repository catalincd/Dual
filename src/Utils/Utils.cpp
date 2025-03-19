#include "Utils.h"

float intrapolate(float a, float b, float value)
{
	return (value - a) / (b - a);
}

float interpolate(float a, float b, float bias)
{
	return a + ((b - a) * bias);
}

