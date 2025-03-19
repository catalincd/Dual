#pragma once
#include "../Controller.h"

#include <TFT_eSPI.h>

#include "Screen.h"
#include "Screens/ValueScreen.h"
#include "Screens/UnitScreen.h"

#define PIN_SCREEN_1 16
#define PIN_SCREEN_2 17

/*
Maybe Upper and Lower should be a List in the future
*/

class ScreenManager : public Controller
{
    Screen* Lower;
    Screen* Upper;
public:
    ScreenManager() : Controller("ScreenManager") 
    {
        
    }

    void InitTFT();
    bool Start() override;
    bool Update() override;
};
