#include "ScreenManager.h"
#define DELAY 5

extern ButtonManager g_ButtonManager;
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
    InitTFT();
    delay(300);
    lv_init();
    
    Lower = new ValueScreen(16);
    Lower->Start({
        AllWidgets[LowerWidget].lower,
        AllWidgets[LowerWidget].upper,
        AllWidgets[LowerWidget].unit,
        AllWidgets[LowerWidget].image
    });

    Upper = new UnitScreen(17);
    Upper->Start({
        AllWidgets[UpperWidget].lower,
        AllWidgets[UpperWidget].upper,
        AllWidgets[UpperWidget].unit,
        AllWidgets[UpperWidget].image
    });
    return true;
}

bool ScreenManager::Update()
{
    if(g_ButtonManager.Released(0))
    {
        if(g_ButtonManager.LongReleased(0))
        {
            Rotation++;
            Rotation %= 4;
            Upper->Rotate(Rotation);
            Lower->Rotate(Rotation);
        }
        else
        {
            LowerWidget++;
            LowerWidget%=WIDGETS_NUM;
            Lower->Start({
                AllWidgets[LowerWidget].lower,
                AllWidgets[LowerWidget].upper,
                AllWidgets[LowerWidget].unit,
                AllWidgets[LowerWidget].image
            });
        }
    }

    if(g_ButtonManager.Released(1))
    {
        if(g_ButtonManager.LongReleased(1))
        {
            // TO DO: SWITCH COLOR
        }
        else
        {
            UpperWidget++;
            UpperWidget%=WIDGETS_NUM;
            Upper->Start({
                AllWidgets[UpperWidget].lower,
                AllWidgets[UpperWidget].upper,
                AllWidgets[UpperWidget].unit,
                AllWidgets[UpperWidget].image
            });
        }
    }



    int data = (millis() / 20) % 100;
    int switced = data = ((millis() / 2000) % 2)? data : 100 - data;

    Lower->Draw({switced});

    Upper->Draw({switced});

    lv_task_handler();
	lv_timer_handler();
	delay(DELAY);
	lv_tick_inc(DELAY);

    return true;
}
