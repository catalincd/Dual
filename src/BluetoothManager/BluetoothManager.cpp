#include "BluetoothManager.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
extern ScreenManager* g_ScreenManager;

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
        connected = InitOBD();
    }
}

bool BluetoothManager::InitOBD() // TO DO : check for responses and retry, maybe only that request
{
    g_ScreenManager->PrintTFT(SendCommand("ATZ"));          delay(BLUETOOTH_INIT_DELAY);
    g_ScreenManager->PrintTFT(SendCommand("ATE0"));         delay(BLUETOOTH_INIT_DELAY);
    g_ScreenManager->PrintTFT(SendCommand("ATL1"));         delay(BLUETOOTH_INIT_DELAY);
    g_ScreenManager->PrintTFT(SendCommand("ATS0"));         delay(BLUETOOTH_INIT_DELAY);
    g_ScreenManager->PrintTFT(SendCommand("ATSP0"));        delay(BLUETOOTH_INIT_DELAY);
    g_ScreenManager->PrintTFT(SendCommand("0100"));         delay(BLUETOOTH_INIT_DELAY);
    //g_ScreenManager->PrintTFT(SendCommand("ATDP"));         delay(BLUETOOTH_INIT_DELAY);

    return true;
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
    Serial.printf("SENDING COMMAND: %s\n", command.c_str());

    SerialBT.println(command.c_str());
    delay(BLUETOOTH_RESPONSE_DELAY);
    std::string response = "";
    if(SerialBT.available())
    {
        response = std::string(SerialBT.readStringUntil('>').c_str());
    }
    Serial.printf("RECEIVED: %s\n", response.c_str());
    return response;
}

bool BluetoothManager::Update()
{

    return true;
}