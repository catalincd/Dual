#include "ButtonManager.h"

bool ButtonManager::Start()
{
    int index = 0;
    for(int i=0;i<PINS.size();i++)
    {
        pinMode(PINS[i].pin, INPUT_PULLUP);
        INDEX[PINS[i].pin] = i;
    }

    return true;
}

bool ButtonManager::Update()
{
    for(int i=0;i<PINS.size();i++)
    {
        bool lowNow = (digitalRead(PINS[i].pin) == LOW);
        if(lowNow != PINS[i].pressed)
        {
            PINS[i].pressed = lowNow;
            PINS[i].prevEvent = PINS[i].lastEvent;
            PINS[i].lastEvent = millis();
            Serial.printf("EVENT: %d\n", PINS[i].pin);
        }
    }
    return true;
}

bool ButtonManager::Pressed(int id)
{
    return PINS[id].pressed;
}

bool ButtonManager::Released(int id)
{
    const ButtonState state = PINS[id];
    return (!(state.pressed) && (millis() - state.lastEvent < 5));
}

bool ButtonManager::LongReleased(int id)
{
    const ButtonState state = PINS[id];
    return (!(state.pressed) && (millis() - state.lastEvent < 5) && (state.lastEvent - state.prevEvent > 1000));
}