#include "BluetoothManager.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

bool BluetoothManager::Start()
{
    if(BLUETOOTH_DEBUG)
    {
        connected = true;
        return true;
    }

	SerialBT.begin("ESP32", true);
	SerialBT.setPin("1234");

	if(!Connect())
	{
		Serial.println("BT connection failed");
        connected = false;
	}
    else
    {
        Serial.println("BT connection success");
        connected = true;
        
    }
}

bool BluetoothManager::IsConnected() const
{
    return connected;
}

bool BluetoothManager::Connect()
{
    int availableTries = BLUETOOTH_TRIES;
	while (!SerialBT.connect(SLAVE_MAC_ADDRESS) && availableTries)
	{
		Serial.println("BT failed... retrying");
		availableTries--;
		delay(BLUETOOTH_DELAY);
	}
	return availableTries;
}

std::string BluetoothManager::SendCommand(std::string command)
{
    SerialBT.println(command.c_str());
    delay(BLUETOOTH_RESPONSE_DELAY);
    std::string response = "";
    if(SerialBT.available())
    {
        response = std::string(SerialBT.readStringUntil('>').c_str());
    }
    return response;
}

bool BluetoothManager::Update()
{

    return true;
}