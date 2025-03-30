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
    return true;
}

bool BluetoothManager::InitOBD()
{
    g_ScreenManager->PrintTFT("INIT"); 

    std::vector<std::pair<std::string, std::string>> commands = {
		{"ATZ", "327"}, 
		{"ATE0", "OK"}, 
		{"ATL1", "OK"}, 
		{"ATS0", "OK"}, 
		{"ATSP0", "OK"}, 
		{"0100", ""},
		{"ATDP", ""}
	};

	for(int i=0;i<commands.size();i++)
	{
		if(!SendCommandChecked(commands[i].first, commands[i].second));
			return false;
			
		delay(BLUETOOTH_INIT_DELAY);
	}

    return true;
}

bool BluetoothManager::IsConnected()
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

std::string BluetoothManager::SendCommand(std::string command, uint32_t resp_delay)
{
    if(BLUETOOTH_DEBUG) return "4104A0\r\n\r\n>";

    Serial.printf("SENDING COMMAND: %s\n", command.c_str());

    SerialBT.println(command.c_str());
    delay(resp_delay);
    std::string response = "";
    int start = millis();
    while(true)
    {
        if(millis() - start > 200) break;
        if(SerialBT.available())
        {
            char c = SerialBT.read();
            response += c;
            if(c == '>') break;
        }
    }

    Serial.printf("RECEIVED: %s\n", response.c_str());
    return response;
}

bool BluetoothManager::SendCommandChecked(std::string command, std::string expected, uint32_t resp_delay)
{
    int availableTries = BLUETOOTH_TRIES;
	while (availableTries)
	{
		std::string response = SendCommand(command, resp_delay);
        if(response.find(expected) != std::string::npos)
            return true;
        availableTries--;
        delay(5);
	}
	return availableTries;
}

bool BluetoothManager::Update()
{

    return true;
}