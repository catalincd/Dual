#pragma once

#include <string>
#include <functional>

#define WIDGETS_NUM 3

typedef struct Widget
{
	int id;
	std::string image;
	std::string code;
	std::string unit;
	int lower;
	int upper;	
	std::function<float(std::string)> process;
};


static Widget CoolantTemp{0, "coolant", "0105", "°C", 0, 120, [](const std::string response) {
	return strtol(response.substr(4, 2).c_str(), NULL, 16) - 40;
}};

static Widget BatteryVoltage{1, "battery", "ATRV", "dV", 80, 160, [](const std::string response) {
	return std::stof(response) * 10.0f;
}};

static Widget EngineLoad{2, "load", "0104", "%", 0, 100, [](const std::string response) {
	return (float)strtol(response.substr(4, 2).c_str(), NULL, 16) / 2.55f;
}};

static Widget AllWidgets[] = {
	CoolantTemp, 
	BatteryVoltage,
	EngineLoad
};




	