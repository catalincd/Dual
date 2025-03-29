#pragma once
#include "../Controller.h"
#include "../widgets.h"
#include "../ButtonManager/ButtonManager.h"

#include <TFT_eSPI.h>

#include "Screen.h"
#include "Screens/ValueScreen.h"
#include "Screens/UnitScreen.h"

#define PIN_SCREEN_1 16
#define PIN_SCREEN_2 17


/*
Maybe Upper and Lower should be a List in the future
*/
static int Rotation;

class ScreenManager : public Controller
{
    Screen* Lower;
    Screen* Upper;
    
    int LowerWidget = 0; // TO DO: Load from mem
    int UpperWidget = 1; // TO DO: Load from mem
public:
    ScreenManager() : Controller("ScreenManager") 
    {
        Rotation = 0;
    }

    void InitTFT();
    void PrintTFT(std::string message);
    bool Start() override;
    bool Update() override;
};
