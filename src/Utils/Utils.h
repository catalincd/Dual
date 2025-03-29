#pragma once

#include <Arduino.h>

float intrapolate(float a, float b, float value);
float interpolate(float a, float b, float bias);
void print_partition_table();

const float PIb = 1.57079f;