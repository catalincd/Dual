#pragma once
#include "Arduino.h"
#include "lvgl.h"
#include <map>

#define WIDTH 240
#define HEIGHT 240

struct ScreenStartData
{
    int lower;
    int upper;
    std::string unit;
    std::string image;
};

struct ScreenUpdateData
{
    int value;
};


class Screen
{
protected:
    int pin = -1;
    lv_disp_drv_t disp_drv; 
    lv_disp_t* p_disp;
    lv_disp_draw_buf_t d_buf;
    lv_color_t c_buf[WIDTH * HEIGHT / 10]; 
    lv_obj_t* screen;
public:
    Screen(int _pin);
    
    void Init(int _pin);
    void Rotate(int rotation);

    virtual void Start(ScreenStartData data) {}
    virtual void Draw(ScreenUpdateData data) {}

    
};


