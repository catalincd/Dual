#pragma once
#include <Arduino.h>
#include <string>

class Controller
{
    std::string name = "Controller";
public:
    Controller(std::string _name)
    {
        name = _name;
        // Serial.printf("INIT %s", name.c_str());
    }

    virtual bool Start() { return true; }
    virtual bool Update() { return true; }
    std::string GetName() { return name; }
};