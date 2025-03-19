#pragma once
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "lvgl.h"

#define PIN_1 16
#define PIN_2 17
#define PIN_3 99

extern TFT_eSPI tft;

void flush_scr1(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	uint16_t w = area->x2 - area->x1 + 1;
	uint16_t h = area->y2 - area->y1 + 1;
	digitalWrite(PIN_1, LOW);
	
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors((uint16_t *)&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
	digitalWrite(PIN_1, HIGH);
}

void flush_scr2(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	uint16_t w = area->x2 - area->x1 + 1;
	uint16_t h = area->y2 - area->y1 + 1;
	digitalWrite(PIN_2, LOW);
	
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors((uint16_t *)&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
	digitalWrite(PIN_2, HIGH);
}

void flush_scr3(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	
}


void (*GetFlusherCallBack(int PIN))(_lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if(PIN == 16) return flush_scr1;
    if(PIN == 17) return flush_scr2;
    return flush_scr3;
}
