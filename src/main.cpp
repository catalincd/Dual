#include <Arduino.h>
#include <BluetoothSerial.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "SPIFFS.h"
#include "FS.h"
#include "widgets.h"
#include "tests.h"
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
/*
#include <functional>

*/

#pragma pack(push, 1)
/*

*/


#define DEBUG_DISABLE_BT 0

#define WIDTH 240
#define HEIGHT 240

#define PIN_SCREEN_1 16
#define PIN_SCREEN_2 17

#define PIN_BUTTON_1 25
#define PIN_BUTTON_2 26

bool usedWidges[WIDGETS_NUM] = {1, 1, 0};
bool switched_1;
bool switched_2;

TFT_eSPI tft = TFT_eSPI();
/////////// TFT
void InitTFT()
{
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

void PrintScreen1(std::string message)
{
	digitalWrite(PIN_SCREEN_1, LOW);
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(80, 80);
	tft.setTextColor(TFT_WHITE);
	tft.print(message.c_str());
	digitalWrite(PIN_SCREEN_1, HIGH);
}

void PrintScreen2(std::string message)
{
	digitalWrite(PIN_SCREEN_2, LOW);
	tft.fillScreen(TFT_BLACK);
	tft.setCursor(80, 80);
	tft.setTextColor(TFT_WHITE);
	tft.print(message.c_str());
	digitalWrite(PIN_SCREEN_2, HIGH);
}
/////////// BLUETOOTH


uint8_t SLAVE_MAC_ADDRESS[] = {0x1C, 0xA1, 0x35, 0x69, 0x8D, 0xC5};
//uint8_t SLAVE_MAC_ADDRESS[] = {0xD4, 0xD7, 0xCF, 0x6D, 0x71, 0x65};

BluetoothSerial SerialBT;
bool BT_CONNECTED = false;
const int BT_TRIES = 5;
const int INIT_TRIES = 5;
const int INIT_DELAY_FACTOR = 500;
const int BT_DELAY = 2000;

std::string SendCommand(std::string command)
{
	if(DEBUG_DISABLE_BT) return (command + "82" + "7E");
		
	SerialBT.println(String(command.c_str()));
	std::vector<char> incoming;
	while(1)
	{
		if(SerialBT.available())
		{
			char incomingByte = SerialBT.read();
			incoming.push_back(incomingByte);
			if(incomingByte == '>')
			{
				break;
			}
		}
	}
	
	return std::string(incoming.begin(), incoming.end());
}



std::string SendInitCommand(std::string command)
{
	PrintScreen1(command);
	std::string response = SendCommand(command); 
	PrintScreen2(response);
	Serial.println(String(response.c_str())); 
	delay(1000);
	return response;
}

bool ExecuteInit(const int currentDelay)
{
	std::vector<std::pair<std::string, std::string>> commands = {
		{"ATZ", "ELM"}, 
		{"ATE0", "OK"}, 
		{"ATL1", "OK"}, 
		{"ATS0", "OK"}, 
		{"ATSP0", "OK"}, 
		{"0100", ""},
		{"ATDP", ""}
	};

	for(int i=0;i<commands.size();i++)
	{
		std::string response = SendInitCommand(commands[i].first);
		if(response.find(commands[i].second) == std::string::npos)
			return false;
			
		delay(currentDelay);
	}
	
	return true;
}

void BluetoothConnect()
{
	SerialBT.begin("ESP32", true);
	SerialBT.setPin("1234");

	if(DEBUG_DISABLE_BT)
	{
		delay(BT_DELAY);
		BT_CONNECTED = false;
		return;
	}
		
	PrintScreen1("BT CONNECTING");
	int availableTries = BT_TRIES;
	while (!SerialBT.connect(SLAVE_MAC_ADDRESS))
	{
		Serial.printf("TRIED BT");
		availableTries--;
		if(!availableTries) 
		{
			PrintScreen1("BT FAILED");
			delay(2000);
			return;
		}
		delay(BT_DELAY);
	}
		
	ExecuteInit(1000);
	
	BT_CONNECTED = true;
}


////// BUTTON MANAGER

bool m_leftPressed;
bool m_rightPressed;

bool m_leftHit;
bool m_rightHit;

void ButtonsInit()
{
	pinMode(PIN_BUTTON_1, INPUT_PULLUP);
	pinMode(PIN_BUTTON_2, INPUT_PULLUP);
}

void ButtonsUpdate()
{
	bool leftLow = (digitalRead(PIN_BUTTON_1) == LOW);
	bool rightLow = (digitalRead(PIN_BUTTON_2) == LOW);

	m_leftHit = leftLow && !m_leftPressed;
	m_rightHit = rightLow && !m_rightPressed;
	
	if(m_leftHit) Serial.printf("HIT LEFT!\n");
	if(m_rightHit) Serial.printf("HIT RIGHT!\n");
	
	m_leftPressed = leftLow;
	m_rightPressed = rightLow;
}


/////// WIDGETS

int left_widget_id = 0;
int right_widget_id = 1;
Widget* left_widget = &(AllWidgets[left_widget_id]);
Widget* right_widget = &(AllWidgets[right_widget_id]);

int GetNextWidget(int currentWidget)
{
	for(int i=0;i<WIDGETS_NUM;i++)
	{
		if(!usedWidges[i])
		{
			return i;
		}
	}
	return -1;
}

void UpdateWidgets()
{
	if(m_leftHit)
	{
		int temp = left_widget_id;
		left_widget_id = GetNextWidget(left_widget_id);
		
		usedWidges[temp] = false;
		usedWidges[left_widget_id] = true;
		left_widget = &(AllWidgets[left_widget_id]);
		
		switched_1 = true;
	}
	
	if(m_rightHit)
	{
		int temp = right_widget_id;
		right_widget_id = GetNextWidget(right_widget_id);
		
		usedWidges[temp] = false;
		usedWidges[right_widget_id] = true;
		right_widget = &(AllWidgets[right_widget_id]);
		
		switched_2 = true;
	}
}


////// FILE MANAGER

void InitFileManager()
{
	if (!SPIFFS.begin(true)) {
		Serial.println("SPIFFS Mount Failed");
		return;
	}
	Serial.println("SPIFFS Mount Successful");
}

std::map<std::string, lv_img_dsc_t*> loadedImages;
lv_img_dsc_t* LoadImage(std::string filename) // make dict and free auto
{
	auto searched = loadedImages.find(filename);
	if(searched != loadedImages.end())
		return searched->second;
	
	const char* path = filename.c_str();
	Serial.printf("Loading image: %s\n", path);
	
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


////// SCREEENS

float intrapolate(float a, float b, float value)
{
	return (value - a) / (b - a);
}

float interpolate(float a, float b, float bias)
{
	return a + ((b - a) * bias);
}

const float PIb = 1.57079f;




// SCREEN 1 DATA
lv_disp_drv_t disp_drv_1; 
lv_disp_t* p_disp_1;
lv_disp_draw_buf_t d_buf_1;
lv_color_t c_buf_1[WIDTH * HEIGHT / 10]; 
lv_obj_t* screen_1;
lv_obj_t* img_1;
lv_obj_t* obj_1;
lv_obj_t* unit_1;
lv_obj_t* value_1;
int last_value_update_1 = 0;
lv_meter_scale_t* scale_1;
lv_meter_indicator_t* arc_1;
lv_meter_indicator_t* needle_1;



void flush_scr1(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	uint16_t w = area->x2 - area->x1 + 1;
	uint16_t h = area->y2 - area->y1 + 1;

	//int pin = disp == &disp_drv_1? PIN_SCREEN_1 : PIN_SCREEN_2;
	digitalWrite(PIN_SCREEN_1, LOW);
	
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors((uint16_t *)&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
	
	digitalWrite(PIN_SCREEN_1, HIGH);
}

// SCREEN 2 DATA
lv_disp_drv_t disp_drv_2; 
lv_disp_t* p_disp_2;
lv_disp_draw_buf_t d_buf_2;
lv_color_t c_buf_2[WIDTH * HEIGHT / 10]; 
lv_obj_t* screen_2;
lv_obj_t* img_2;
lv_obj_t* obj_2;
lv_obj_t* unit_2;
lv_meter_scale_t* scale_2;
lv_meter_indicator_t* arc_2;
lv_meter_indicator_t* needle_2;

void flush_scr2(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
	uint16_t w = area->x2 - area->x1 + 1;
	uint16_t h = area->y2 - area->y1 + 1;

	digitalWrite(PIN_SCREEN_2, LOW);
	
	tft.startWrite();
	tft.setAddrWindow(area->x1, area->y1, w, h);
	tft.pushColors((uint16_t *)&color_p->full, w * h, true);
	tft.endWrite();

	lv_disp_flush_ready(disp);
	
	digitalWrite(PIN_SCREEN_2, HIGH);
}



void InitScreen1()
{
	lv_disp_draw_buf_init(&d_buf_1, c_buf_1, NULL, 240);     
	lv_disp_drv_init(&disp_drv_1);       
	disp_drv_1.flush_cb = flush_scr1;    
	disp_drv_1.draw_buf = &d_buf_1;      
	disp_drv_1.hor_res = WIDTH;
	disp_drv_1.ver_res = HEIGHT;
	p_disp_1 = lv_disp_drv_register(&disp_drv_1);
	
	screen_1 = lv_disp_get_scr_act(p_disp_1);
	
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
	
	obj_1 = lv_meter_create(screen_1);
	lv_obj_set_size(obj_1, 244, 244);
	lv_obj_center(obj_1);
	lv_obj_align(obj_1, LV_ALIGN_CENTER, 0, 0);
	
	lv_obj_add_style(obj_1, &glow_style, LV_PART_MAIN);
	lv_obj_add_style(obj_1, &glow_style, LV_PART_INDICATOR);
	lv_obj_add_style(obj_1, &glow_style, LV_PART_ITEMS);
	
	lv_obj_set_style_bg_color(obj_1, BLACK, 0);
	
	// Create a scale
	scale_1 = lv_meter_add_scale(obj_1);
	lv_meter_set_scale_range(obj_1, scale_1, left_widget->lower, left_widget->upper, 240, 150);
	lv_meter_set_scale_ticks(obj_1, scale_1, 9, 3, 15, BIMMER_ORANGE);
	lv_meter_set_scale_major_ticks(obj_1, scale_1, 2, 6, 20, BIMMER_ORANGE, 22);

	// Add an arc indicator
	arc_1 = lv_meter_add_arc(obj_1, scale_1, 20, BIMMER_ORANGE, 0);
	lv_meter_set_indicator_start_value(obj_1, arc_1, left_widget->lower); // to do divide to diff
	lv_meter_set_indicator_end_value(obj_1, arc_1, left_widget->upper);

	
	unit_1 = lv_label_create(screen_1);  // Create label on the active screen
	lv_label_set_text(unit_1, left_widget->unit.c_str());          // Set text
	lv_obj_set_style_text_color(unit_1, BIMMER_ORANGE, LV_PART_MAIN);  // Set text color to black
	lv_obj_set_style_text_font(unit_1, &lv_font_montserrat_24, LV_PART_MAIN);
	lv_obj_align(unit_1, LV_ALIGN_BOTTOM_RIGHT, -50, -25);
	
	value_1 = lv_label_create(screen_1);
	lv_label_set_text(value_1, "0.0");          
	lv_obj_set_style_text_color(value_1, BIMMER_ORANGE, LV_PART_MAIN);  
	lv_obj_set_style_text_font(value_1, &lv_font_montserrat_32, LV_PART_MAIN);
	lv_obj_align(value_1, LV_ALIGN_CENTER, 0, 0);
	
	img_1 = lv_img_create(screen_1);	
	lv_img_set_src(img_1, LoadImage(("/" + left_widget->image + ".bmp").c_str()));
	lv_obj_align(img_1, LV_ALIGN_CENTER, 0, 80);
}

std::string Precision(const float value, const int precision = 1)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << value;
	return oss.str();
}

void UpdateScreen1()
{
	if(switched_1)
	{
		lv_label_set_text(unit_1, left_widget->unit.c_str());
		lv_img_set_src(img_1, LoadImage(("/" + left_widget->image + ".bmp").c_str()));
		lv_meter_set_scale_range(obj_1, scale_1, left_widget->lower, left_widget->upper, 240, 150);
		lv_meter_set_indicator_start_value(obj_1, arc_1, left_widget->lower);
		lv_meter_set_indicator_end_value(obj_1, arc_1, left_widget->upper);
		switched_1 = false;
	}
	
	float value = 0;
	
	if(BT_CONNECTED)
	{
		std::string response = SendCommand(left_widget->code);
		value = left_widget->process(response);
	}
	else
	{
		unsigned long inteval = 6000;
		float modulo = millis() % inteval;
		float bias = (modulo / float(inteval) * 2.0f - 1.0f);
		value = interpolate(left_widget->lower, left_widget->upper, cos(bias * PIb));
	}
	
	if(millis() - last_value_update_1 > 1000)
	{
		char text[4];
		itoa(value, text, 10);
		lv_label_set_text(value_1, text);   
		last_value_update_1 = millis(); 
	}
	lv_meter_set_indicator_end_value(obj_1, arc_1, ceil(value));
}




void InitScreen2()
{
	lv_disp_draw_buf_init(&d_buf_2, c_buf_2, NULL, 240);     
	lv_disp_drv_init(&disp_drv_2);       
	disp_drv_2.flush_cb = flush_scr2;    
	disp_drv_2.draw_buf = &d_buf_2;      
	disp_drv_2.hor_res = WIDTH;
	disp_drv_2.ver_res = HEIGHT;
	p_disp_2 = lv_disp_drv_register(&disp_drv_2);
	
	screen_2 = lv_disp_get_scr_act(p_disp_2);
	
	lv_color_t BIMMER_ORANGE = lv_color_hex(0xff8000);
	lv_color_t BLACK = lv_color_hex(0x000);
	lv_color_t WHITE = lv_color_hex(0xffffff);
	

	static lv_style_t glow_style;
	lv_style_init(&glow_style);
	lv_style_set_bg_color(&glow_style, BLACK);
	lv_style_set_bg_opa(&glow_style, LV_OPA_100);
	lv_style_set_line_color(&glow_style, BIMMER_ORANGE);
	lv_style_set_text_color(&glow_style, BIMMER_ORANGE);
	lv_style_set_text_font(&glow_style, &lv_font_montserrat_24);
	lv_style_set_shadow_color(&glow_style, BIMMER_ORANGE);
	lv_style_set_shadow_ofs_x(&glow_style, 0);
	lv_style_set_shadow_ofs_y(&glow_style, 0);
	lv_style_set_shadow_spread(&glow_style, 8);
	lv_style_set_line_rounded(&glow_style, 1);
	lv_style_set_radius(&glow_style, 999);
	
	obj_2 = lv_meter_create(screen_2);
	lv_obj_set_size(obj_2, 244, 244);
	lv_obj_center(obj_2);
	lv_obj_align(obj_2, LV_ALIGN_CENTER, 0, 0);
	
	lv_obj_add_style(obj_2, &glow_style, LV_PART_MAIN);
	lv_obj_add_style(obj_2, &glow_style, LV_PART_INDICATOR);
	lv_obj_add_style(obj_2, &glow_style, LV_PART_ITEMS);
	
	lv_obj_set_style_bg_color(obj_2, BLACK, 0);
	
	img_2 = lv_img_create(screen_2);	
	lv_img_set_src(img_2, LoadImage(("/" + right_widget->image + ".bmp").c_str()));
	lv_obj_align(img_2, LV_ALIGN_CENTER, 0, 80);

	// Create a scale
	scale_2 = lv_meter_add_scale(obj_2);
	lv_meter_set_scale_range(obj_2, scale_2, right_widget->lower, right_widget->upper, 240, 150);
	lv_meter_set_scale_ticks(obj_2, scale_2, 9, 3, 15, BIMMER_ORANGE);
	lv_meter_set_scale_major_ticks(obj_2, scale_2, 2, 6, 20, BIMMER_ORANGE, 25);

	// Add an arc indicator
	arc_2 = lv_meter_add_arc(obj_2, scale_2, 20, BIMMER_ORANGE, 0);
	lv_meter_set_indicator_start_value(obj_2, arc_2, right_widget->lower); // to do divide to diff
	lv_meter_set_indicator_end_value(obj_2, arc_2, right_widget->upper);

	
	unit_2 = lv_label_create(screen_2);  // Create label on the active screen
	lv_label_set_text(unit_2, right_widget->unit.c_str());          // Set text
	lv_obj_set_style_text_color(unit_2, BIMMER_ORANGE, LV_PART_MAIN);  // Set text color to black
	lv_obj_set_style_text_font(unit_2, &lv_font_montserrat_48, LV_PART_MAIN);
	lv_obj_align(unit_2, LV_ALIGN_CENTER, 0, 0);
}

void UpdateScreen2()
{
	if(switched_2)
	{
		lv_label_set_text(unit_2, right_widget->unit.c_str());
		lv_img_set_src(img_2, LoadImage(("/" + right_widget->image + ".bmp").c_str()));
		lv_meter_set_scale_range(obj_2, scale_2, right_widget->lower, right_widget->upper, 240, 150);
		lv_meter_set_indicator_start_value(obj_2, arc_2, right_widget->lower);
		lv_meter_set_indicator_end_value(obj_2, arc_2, right_widget->upper);
		switched_2 = false;
	}
	
	float value = 0;
	
	if(BT_CONNECTED)
	{
		std::string response = SendCommand(right_widget->code);
		value = right_widget->process(response);
	}
	else
	{
		unsigned long inteval = 6000;
		float modulo = millis() % inteval;
		float bias = (modulo / float(inteval) * 2.0f - 1.0f);
		value = interpolate(right_widget->lower, right_widget->upper, cos(bias * PIb));
	}

	lv_meter_set_indicator_end_value(obj_2, arc_2, ceil(value));
}

void InitScreenManager()
{	
	lv_init();
	InitScreen1();
	InitScreen2();
}



void print_partition_table() {
	esp_partition_iterator_t iter = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
	Serial.printf("# Name,   Type, SubType, Offset,  Size, Flags\n");
	while (iter != NULL) {
		const esp_partition_t *partition = esp_partition_get(iter);
		Serial.printf("%s, %d, %d, 0x%08x, 0x%08x,\n",
				 partition->label, partition->type, partition->subtype, partition->address, partition->size);
		iter = esp_partition_next(iter);
	}
	esp_partition_iterator_release(iter);
	Serial.printf("DONE DATA\n");

}


void setup() {
	Serial.begin(9600);
	
	RunWidgetTests();
	
	InitFileManager();
	Serial.printf("File Manager Init Done\n");
	ButtonsInit();
	Serial.printf("Buttons Init Done\n");
	InitTFT();
	Serial.printf("TFT Init Done\n");
	BluetoothConnect();
	Serial.printf("Bluetooth Init Done\n");
	InitScreenManager();
	Serial.printf("Screen Init Done\n");
}



void loop() {
	ButtonsUpdate();
	UpdateWidgets();
	
	UpdateScreen1();
	UpdateScreen2();
	
	lv_task_handler();
	lv_timer_handler();
	delay(1);
	lv_tick_inc(1);
}