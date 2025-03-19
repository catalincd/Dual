#include "ScreenManager.h"

extern TFT_eSPI tft;

void ScreenManager::InitTFT()
{
    Serial.printf("INIT TFT\n");
    pinMode(PIN_SCREEN_1, OUTPUT);
	pinMode(PIN_SCREEN_2, OUTPUT);
	
	digitalWrite(PIN_SCREEN_1, HIGH);
	digitalWrite(PIN_SCREEN_2, HIGH);
	
	digitalWrite(PIN_SCREEN_1, LOW);
	digitalWrite(PIN_SCREEN_2, LOW);
	
	tft.init();
	tft.setTextSize(2);
	tft.setRotation(2);
	tft.fillScreen(TFT_RED);
	
	digitalWrite(PIN_SCREEN_1, HIGH);
	digitalWrite(PIN_SCREEN_2, HIGH);
}

bool ScreenManager::Start()
{
    ScreenStartData lowerData = {};
    ScreenStartData upperData = {};
    
    InitTFT();
    delay(300);
    lv_init();
    
    Lower = new ValueScreen(16);
    Lower->Start(lowerData);

    Upper = new UnitScreen(17);
    Upper->Start(upperData);
    return true;
}

bool ScreenManager::Update()
{
    int data = (millis() / 20) % 100;

    Lower->Draw({data, 0, 100});

    Upper->Draw({data, 0, 100});

    lv_task_handler();
	lv_timer_handler();
	delay(1);
	lv_tick_inc(1);

    return true;
}
