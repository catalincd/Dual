#pragma once
#include <Arduino.h>
#include <BluetoothSerial.h>
#define BLUETOOTH_DEBUG true
#define BLUETOOTH_TRIES 5
#define BLUETOOTH_DELAY 2000
#define BLUETOOTH_RESPONSE_DELAY 5
#define BLUETOOTH_INIT_DELAY 750
#include "../Controller.h"
#include "../ScreenManager/ScreenManager.h"


static uint8_t SLAVE_MAC_ADDRESS[] = {0x1C, 0xA1, 0x35, 0x69, 0x8D, 0xC5};

class BluetoothManager : public Controller
{
    bool connected = false;
public:
    BluetoothManager() : Controller("BluetoothManager") 
    {
        
    }

    bool Start() override;
    bool Update() override;
    std::string SendCommand(std::string command, uint32_t resp_delay = BLUETOOTH_RESPONSE_DELAY);
    bool SendCommandChecked(std::string command, std::string expected, uint32_t resp_delay = BLUETOOTH_RESPONSE_DELAY);
    bool Connect();
    bool IsConnected();
    bool InitOBD();
};