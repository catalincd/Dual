#include "Flushers.h"
#include "Screen.h"


Screen::Screen(int _pin)
{
    pin = _pin;
    Init(pin);
}

void Screen::Init(int _pin)
{
    Serial.println("INIT SCREEN");
    lv_disp_draw_buf_init(&d_buf, c_buf, NULL, WIDTH);     
	lv_disp_drv_init(&disp_drv);       
	disp_drv.flush_cb = GetFlusherCallBack(_pin);    
	disp_drv.draw_buf = &d_buf;      
	disp_drv.hor_res = WIDTH;
	disp_drv.ver_res = HEIGHT;
	disp_drv.sw_rotate = 1;
	disp_drv.antialiasing = 1;
	p_disp = lv_disp_drv_register(&disp_drv);
	screen = lv_disp_get_scr_act(p_disp);
}

void Screen::Rotate(int rotation)
{
	disp_drv.rotated = rotation;
	lv_obj_invalidate(screen);
}

