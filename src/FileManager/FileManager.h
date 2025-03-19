#pragma once

#include "../Controller.h"
#include "SPIFFS.h"
#include "FS.h"
#include <lvgl.h>
#include <map>

class FileManager : public Controller
{
    std::map<std::string, lv_img_dsc_t*> loadedImages;
public:
    FileManager() : Controller("FileManager") {}
    lv_img_dsc_t* LoadImage(std::string filename);

    bool Start() override;
};

#pragma pack(push, 1)
struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t dibHeaderSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColorCount;
} BMPHeader;
#pragma pack(pop)