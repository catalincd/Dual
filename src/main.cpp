#include <Arduino.h>
#include <TFT_eSPI.h>
#include <vector>

#include "ScreenManager/ScreenManager.h"
#include "FileManager/FileManager.h"
#include "ButtonManager/ButtonManager.h"
#include "BluetoothManager/BluetoothManager.h"

#include "Controller.h"
#include "widgets.h"
#include "tests.h"



TFT_eSPI tft = TFT_eSPI();

ScreenManager* g_ScreenManager = new ScreenManager(); 
FileManager* g_FileManager = new FileManager(); 
ButtonManager* g_ButtonManager = new ButtonManager(); 
BluetoothManager* g_BluetoothManager = new BluetoothManager(); 

std::vector<Controller*> controllers = {
    g_FileManager,
    g_ButtonManager,
    g_ScreenManager,
    g_BluetoothManager
};


void setup() {
	Serial.begin(9600);

    delay(2500);
	
    Serial.printf("RUNNING SERIAL TESTS\n\n");
	RunWidgetTests();
	
    for(auto i : controllers)
    {
        Serial.printf("START %s\n", i->GetName().c_str());
        if(!(i->Start()))
        {
            Serial.printf("CONSTRUCTING %s FAILED\n", i->GetName().c_str());
            while(1);
        }
        Serial.printf("DONE\n\n");
    }

    Serial.printf("SETUP DONE\n");
}



void loop() {
    for(auto i : controllers)
    {
        if(!(i->Update()))
        {
            Serial.printf("UPDATING %s FAILED\n", i->GetName().c_str());
        }
    }
	/*
	UpdateWidgets();
    */
}