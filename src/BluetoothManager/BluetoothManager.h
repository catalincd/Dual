#pragma once
#include <Arduino.h>
#include <BluetoothSerial.h>

#include "../Controller.h"

static uint8_t SLAVE_MAC_ADDRESS[] = {0x1C, 0xA1, 0x35, 0x69, 0x8D, 0xC5};

class BluetoothManager : public Controller
{
public:
    BluetoothManager() : Controller("BluetoothManager") 
    {
        
    }

    //bool Start() override;
    //bool Update() override;
    
};