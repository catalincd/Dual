#pragma once
#include "../Screen.h"
#include "../../Utils/Utils.h"
#include "../../FileManager/FileManager.h"

extern FileManager* g_FileManager;



class UnitScreen : public Screen
{
protected:
    lv_obj_t* img;
    lv_obj_t* obj;
    lv_obj_t* unit;
    lv_meter_scale_t* scale;
    lv_meter_indicator_t* arc;
    lv_meter_indicator_t* needle;
public:
    UnitScreen(int _pin) : Screen(_pin) {};

    void Start(ScreenStartData data) override
    {
        lv_color_t BIMMER_ORANGE = lv_color_hex(0xff8000);
        lv_color_t BLACK = lv_color_hex(0x000);
        lv_color_t WHITE = lv_color_hex(0xffffff);

        static lv_style_t glow_style;
        lv_style_init(&glow_style);
        lv_style_set_bg_color(&glow_style, BLACK);
        lv_style_set_bg_opa(&glow_style, LV_OPA_100);
        lv_style_set_line_color(&glow_style, BIMMER_ORANGE);
        lv_style_set_text_color(&glow_style, BIMMER_ORANGE);
        lv_style_set_text_font(&glow_style, &lv_font_montserrat_18);
        lv_style_set_shadow_color(&glow_style, BIMMER_ORANGE);
        lv_style_set_shadow_ofs_x(&glow_style, 0);
        lv_style_set_shadow_ofs_y(&glow_style, 0);
        lv_style_set_shadow_spread(&glow_style, 8);
        lv_style_set_line_rounded(&glow_style, 1);
        lv_style_set_radius(&glow_style, 999);

        obj = lv_meter_create(screen);
        lv_obj_set_size(obj, 244, 244);
        lv_obj_center(obj);
        lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

        lv_obj_add_style(obj, &glow_style, LV_PART_MAIN);
        lv_obj_add_style(obj, &glow_style, LV_PART_INDICATOR);
        lv_obj_add_style(obj, &glow_style, LV_PART_ITEMS);

        lv_obj_set_style_bg_color(obj, BLACK, 0);

        img = lv_img_create(screen);
        lv_img_set_src(img, g_FileManager->LoadImage(("/" + data.image + ".bmp").c_str()));
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 80);

        // Create a scale
        scale = lv_meter_add_scale(obj);
        lv_meter_set_scale_range(obj, scale, data.lower, data.upper, 240, 150);
        lv_meter_set_scale_ticks(obj, scale, 9, 3, 15, BIMMER_ORANGE);
        lv_meter_set_scale_major_ticks(obj, scale, 2, 6, 20, BIMMER_ORANGE, 25);

        // Add an arc indicator
        arc = lv_meter_add_arc(obj, scale, 20, BIMMER_ORANGE, 0);
        lv_meter_set_indicator_start_value(obj, arc, data.lower); // to do divide to diff
        lv_meter_set_indicator_end_value(obj, arc, data.upper);

        unit = lv_label_create(screen);                               // Create label on the active screen
        lv_label_set_text(unit, data.unit.c_str());            // Set text
        lv_obj_set_style_text_color(unit, BIMMER_ORANGE, LV_PART_MAIN); // Set text color to black
        lv_obj_set_style_text_font(unit, &lv_font_montserrat_48, LV_PART_MAIN);
        lv_obj_align(unit, LV_ALIGN_CENTER, 0, 0);
    }

    void Draw(ScreenUpdateData data) override
    {
	    lv_meter_set_indicator_end_value(obj, arc, data.value);
    }
};
