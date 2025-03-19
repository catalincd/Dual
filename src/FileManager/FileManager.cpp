#include "FileManager.h"


bool FileManager::Start()
{
    if (!SPIFFS.begin(true)) {
		Serial.println("SPIFFS Mount Failed");
		return false;
	}
	Serial.println("SPIFFS Mount Successful");
    return true;
}


lv_img_dsc_t* FileManager::LoadImage(std::string filename) // make dict and free auto
{
	auto searched = loadedImages.find(filename);
	if(searched != loadedImages.end())
		return searched->second;
	
	const char* path = filename.c_str();
	Serial.printf("Loading image: %s\n", path);
	
	fs::File f = SPIFFS.open(path, "r");
	if (!f) {
		Serial.println("Failed to open file for reading");
		return NULL;
	}

	size_t file_size = f.size();
	uint8_t *img_data = (uint8_t*)malloc(file_size);
	if (img_data == NULL) {
		Serial.println("Failed to allocate memory for image");
		f.close();
		return NULL;
	}
	

	f.read(img_data, file_size);
	f.close();
	
	memcpy(&BMPHeader, img_data, sizeof(BMPHeader));
	lv_img_dsc_t* img_dsc = new lv_img_dsc_t;
	
	Serial.printf("Loaded image: %d x %d\n", img_dsc->header.w, img_dsc->header.h);

	img_dsc->header.always_zero = 0;
	img_dsc->header.w = BMPHeader.width;
	img_dsc->header.h = (BMPHeader.height > 0? BMPHeader.height : -BMPHeader.height);
	img_dsc->header.cf = 4;
	img_dsc->data = img_data + BMPHeader.offset;

	loadedImages[filename] = img_dsc;
	
	return img_dsc;
}