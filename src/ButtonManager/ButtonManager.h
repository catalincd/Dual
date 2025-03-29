#pragma once
#include "../Controller.h"
#include <vector>

#define PIN_BUTTON_1 25
#define PIN_BUTTON_2 26


struct ButtonState
{
    int pin;
    bool pressed;
    unsigned long lastEvent;
    unsigned long prevEvent;
};

static std::vector<ButtonState> PINS {
    {25, false, 0, 0},
    {26, false, 0, 0}
};

class ButtonManager : public Controller
{
    int INDEX[64];
public:
    ButtonManager() : Controller("ButtonManager") { }
    bool Start() override;
    bool Update() override;

    bool Pressed(int pin);
    bool Released(int pin);
    bool LongReleased(int pin);
};