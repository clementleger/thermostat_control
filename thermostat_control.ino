#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <epdlib.h>
#include <epdpaint.h>
#include "wifi_params.h"
#include "images/images.h"


#ifndef WIFI_SSID
#error "Missing wifi parameters !"
#endif

#define COLORED     0
#define UNCOLORED   1

#define BUSY_PIN D0
#define DC_PIN D2
#define RST_PIN D1

Epd epd(SS, DC_PIN, BUSY_PIN, RST_PIN);

unsigned char image[15000];
Paint paint(image, 400, 300);    //width should be the multiple of 8 

#define CALAOS_API "http://home:5454/api.php"

#define CALAOS_USER		"user"
#define CALAOS_PASSWORD		"pass"

#define HEATER_SETPOINT "intern_4"
#define OUTDOOR_TEMP        "io_43"
#define OUTDOOR_HUMIDITY    "io_42"

#define INDOOR_TEMP     "io_2"
#define INDOOR_HUMIDITY     "io_3"

#define NUMBER_SPACING 2

struct image {
	unsigned char *data;
	unsigned int width;
	unsigned int height;
};

const struct image numberToImage[] = {
	{(unsigned char *) number_0_data, NUMBER_0_WIDTH, NUMBER_0_HEIGHT},
	{(unsigned char *) number_1_data, NUMBER_1_WIDTH, NUMBER_1_HEIGHT},
	{(unsigned char *) number_2_data, NUMBER_2_WIDTH, NUMBER_2_HEIGHT},
	{(unsigned char *) number_3_data, NUMBER_3_WIDTH, NUMBER_3_HEIGHT},
	{(unsigned char *) number_4_data, NUMBER_4_WIDTH, NUMBER_4_HEIGHT},
	{(unsigned char *) number_5_data, NUMBER_5_WIDTH, NUMBER_5_HEIGHT},
	{(unsigned char *) number_6_data, NUMBER_6_WIDTH, NUMBER_6_HEIGHT},
	{(unsigned char *) number_7_data, NUMBER_7_WIDTH, NUMBER_7_HEIGHT},
	{(unsigned char *) number_8_data, NUMBER_8_WIDTH, NUMBER_8_HEIGHT},
	{(unsigned char *) number_9_data, NUMBER_9_WIDTH, NUMBER_9_HEIGHT},
};
const struct image smallNumberToImage[] = {
	{(unsigned char *) number_small_0_data, NUMBER_SMALL_0_WIDTH, NUMBER_SMALL_0_HEIGHT},
	{(unsigned char *) number_small_1_data, NUMBER_SMALL_1_WIDTH, NUMBER_SMALL_1_HEIGHT},
	{(unsigned char *) number_small_2_data, NUMBER_SMALL_2_WIDTH, NUMBER_SMALL_2_HEIGHT},
	{(unsigned char *) number_small_3_data, NUMBER_SMALL_3_WIDTH, NUMBER_SMALL_3_HEIGHT},
	{(unsigned char *) number_small_4_data, NUMBER_SMALL_4_WIDTH, NUMBER_SMALL_4_HEIGHT},
	{(unsigned char *) number_small_5_data, NUMBER_SMALL_5_WIDTH, NUMBER_SMALL_5_HEIGHT},
	{(unsigned char *) number_small_6_data, NUMBER_SMALL_6_WIDTH, NUMBER_SMALL_6_HEIGHT},
	{(unsigned char *) number_small_7_data, NUMBER_SMALL_7_WIDTH, NUMBER_SMALL_7_HEIGHT},
	{(unsigned char *) number_small_8_data, NUMBER_SMALL_8_WIDTH, NUMBER_SMALL_8_HEIGHT},
	{(unsigned char *) number_small_9_data, NUMBER_SMALL_9_WIDTH, NUMBER_SMALL_9_HEIGHT},
};

StaticJsonDocument<200> jsonBuffer;
HTTPClient http;

void setup()
{
	Serial.begin(115200);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while(!WiFi.isConnected() ){
		Serial.println("Waiting for connection");
		delay(1000);
	}
	Serial.println("Connected !");

	if (epd.Init(EPD_PLL_100HZ, 2000000) != 0) {
		Serial.println("e-Paper init failed");
		while(1);
	}

	/* This clears the SRAM of the e-paper display */
	epd.ClearFrame();
	epd.DisplayFrame();
	Serial.println("Clear frame");
}

int executeRequest(String request, String &ret)
{
	http.begin(CALAOS_API);
	http.addHeader("Content-Type", "application/json");
	int httpCode = http.POST(request);
	if(httpCode <= 0) {
		Serial.print("HTTPS connection failed: ");
		Serial.println(httpCode);
		http.end();
		return 1;
	}

	String payload = http.getString();
	ret = payload;
	return 0;
}

int calaosGetState(String item)
{
	String output;
	String payload;

	JsonObject root = jsonBuffer.to<JsonObject>();
	root["cn_user"] = CALAOS_USER;
	root["cn_pass"] = CALAOS_PASSWORD;
	root["action"] = "get_state";

	JsonArray data = root.createNestedArray("items");
	data.add(item);

	serializeJson(root, output);

	int ret = executeRequest(output, payload);

	DeserializationError error = deserializeJson(jsonBuffer, payload);
	  
	http.end();
	if (error) {
		Serial.println("deserializeJson() failed");
		return 1;
	}

	return 0;
}


int calaosGetFloatValue(String item, float *value)
{
	int ret = calaosGetState(item);

	JsonObject resp = jsonBuffer.as<JsonObject>();
	*value = resp[item].as<float>();
	jsonBuffer.clear();

	return ret;
}


int calaosSetState(String item, String item_value)
{
	String output;
	String payload;

	JsonObject root = jsonBuffer.to<JsonObject>();
	root["cn_user"] = CALAOS_USER;
	root["cn_pass"] = CALAOS_PASSWORD;
	root["action"] = "set_state";
	root["id"] = item;
	root["value"] = item_value;

	serializeJson(root, output);

	int ret = executeRequest(output, payload);

	DeserializationError error = deserializeJson(jsonBuffer, payload);
	
	http.end();
	if (error) {
		Serial.println("deserializeJson() failed");
		return 1;
	}
	JsonObject resp = jsonBuffer.as<JsonObject>();
	bool status = (resp["success"] != "true");
	jsonBuffer.clear();

	return status;
}

int setTempSetpoint(float value)
{
	return calaosSetState(HEATER_SETPOINT, String(value, 0));
}


static void drawTemp(int x, int y, float temp)
{
	/* We supportse there is no more than 2 digits */
	int tens_d = (int) (temp / 10) % 10;
	int ones_d = (int) temp % 10;
	int tenths_d = (int) (temp * 10) % 10;
	const struct image *tens = &numberToImage[tens_d];
	const struct image *ones = &numberToImage[ones_d];
	const struct image *tenths = &numberToImage[tenths_d];
	unsigned int current_x = x;

	paint.CopyProgmemImage(current_x, y,
		(unsigned char *)thermometer_data,
		THERMOMETER_WIDTH, THERMOMETER_HEIGHT);

	current_x += THERMOMETER_WIDTH + 10;
	paint.CopyProgmemImage(current_x, y,
		tens->data,
		tens->width, tens->height);

	current_x += NUMBER_SPACING + tens->width;
	paint.CopyProgmemImage(current_x, y,
		ones->data,
		ones->width, ones->height);

	current_x += NUMBER_SPACING + ones->width;
	paint.CopyProgmemImage(current_x, y + ones->height - DOT_HEIGHT,
		(unsigned char *) dot_data, DOT_WIDTH, DOT_HEIGHT);

	current_x += NUMBER_SPACING + DOT_WIDTH;
	paint.CopyProgmemImage(current_x, y,
		tenths->data,
		tenths->width, tenths->height);

	
}

void drawHumidity(int x, int y, int unsigned hum)
{
	int tens_d = (int) (hum / 10) % 10;
	int ones_d = (int) hum % 10;
	unsigned int current_x = x;
	const struct image *tens = &smallNumberToImage[tens_d];
	const struct image *ones = &smallNumberToImage[ones_d];
	String str_hum = String(hum);
	str_hum += "%";

	paint.CopyProgmemImage(current_x, y,
		(unsigned char *) humidity_data,
		HUMIDITY_WIDTH, HUMIDITY_HEIGHT);

	current_x += HUMIDITY_WIDTH + 8;
	paint.CopyProgmemImage(current_x, y,
		tens->data,
		tens->width, tens->height);

	current_x += NUMBER_SPACING + tens->width;
	paint.CopyProgmemImage(current_x, y,
		ones->data,
		ones->width, ones->height);

	current_x += NUMBER_SPACING + ones->width;
	paint.CopyProgmemImage(current_x, y,
		(unsigned char *) percent_data,
		PERCENT_WIDTH, PERCENT_HEIGHT);


}

#define DISP_X_START 20
#define DISP_Y_START 30
#define HUMIDITY_Y_OFFSET 20
#define HUMIDITY_X_OFFSET 20
#define SETPOINT_Y_OFFSET 20
#define SETPOINT_X_OFFSET 20

void loop()
{
	float v, in_temp = 0, in_humid = 0;

	//~ calaosGetState(OUTDOOR_TEMP, out_temp);
	//~ calaosGetState(OUTDOOR_HUMIDITY, out_humid);
	calaosGetFloatValue(INDOOR_TEMP, &in_temp);
	calaosGetFloatValue(INDOOR_HUMIDITY, &in_humid);
	calaosGetFloatValue(HEATER_SETPOINT, &v);

	paint.Clear(UNCOLORED);
	drawTemp(DISP_X_START, DISP_Y_START, in_temp);
	drawHumidity(DISP_X_START + HUMIDITY_X_OFFSET, DISP_Y_START + NUMBER_0_HEIGHT + HUMIDITY_Y_OFFSET, in_humid);

	//~ String s =  "Consigne: " + String(v, 0) + "C";
	//~ paint.DrawStringAt(0, 30, s.c_str(), &Font24, COLORED);

	//~ s =  "Interieur Temp: " + in_temp + "C";
	//~ paint.DrawStringAt(0, 60, s.c_str(), &Font16, COLORED);
	//~ s =  "Humidite: " + in_humid + "%";
	//~ paint.DrawStringAt(0, 85, s.c_str(), &Font16, COLORED);

	//~ s =  "Exterieur Temp: " + out_temp + "C";
	//~ paint.DrawStringAt(0, 80, s.c_str(), &Font16, COLORED);
	//~ s =  "Exterieur Humidite: " + out_humid + "%";
	//~ paint.DrawStringAt(0, 105, s.c_str(), &Font16, COLORED);

	epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();
	delay(20000);
  
}
