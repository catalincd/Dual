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


static Widget CoolantTemp{0, "coolant", "0105", "°C", 0, 120, [](const std::string& response) {
	return strtol(response.substr(response.find("05") + 2, 2).c_str(), NULL, 16) - 40;
}};

static Widget BatteryVoltage{1, "battery", "0142", "dV", 80, 160, [](const std::string& response) {
	return strtol(response.substr(response.find("42") + 2, 2).c_str(), NULL, 16);
}};

static Widget EngineLoad{2, "load", "0104", "%", 0, 100, [](const std::string& response) {
	return strtol(response.substr(response.find("04") + 2, 2).c_str(), NULL, 16);
}};

static Widget AllWidgets[] = {
	CoolantTemp, 
	BatteryVoltage,
	EngineLoad
};