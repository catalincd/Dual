#pragma once
#include <Arduino.h>
#include "widgets.h"

#define ASSERT(condition, message) \
	if (!(condition)) { \
		Serial.print("Assertion failed: "); \
		Serial.println(message); \
	}
	
#define ASSERT_EQUAL(val1, val2, message) \
	if ((val1) != (val2)) { \
		Serial.print("Assertion failed: "); \
		Serial.println(message); \
		Serial.print("Expected: "); \
		Serial.print(val2); \
		Serial.print(", Got: "); \
		Serial.println(val1); \
	}
	

void RunWidgetTests()
{
	ASSERT_EQUAL((int)CoolantTemp.process("410588\r\n\r\n>"), 96, "FAILED COOLANT TEST");
	ASSERT_EQUAL(BatteryVoltage.process("13.1V\r\n\r\n>"), 131.00f, "FAILED BATTERY TEST");
	ASSERT_EQUAL((int)EngineLoad.process("410480\r\n\r\n>"), 50, "FAILED LOAD TEST");
}