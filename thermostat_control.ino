#include <Encoder.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <epdlib.h>
#include <epdpaint.h>
#include "wifi_params.h"

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

int calaosGetState(String item, String &item_value)
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
	JsonObject resp = jsonBuffer.as<JsonObject>();
	item_value = resp[item].as<String>();
	jsonBuffer.clear();

	return 0;
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

int getTempSetpoint(float *value)
{
	String setpoint;
	int ret = calaosGetState(HEATER_SETPOINT, setpoint);
	*value = setpoint.toFloat();
	return ret;
}

void loop()
{
	float v;
	String out_temp = "0", out_humid = "0";
	String in_temp = "0", in_humid = "0";

	//~ calaosGetState(OUTDOOR_TEMP, out_temp);
	//~ calaosGetState(OUTDOOR_HUMIDITY, out_humid);
	calaosGetState(INDOOR_TEMP, in_temp);
	calaosGetState(INDOOR_HUMIDITY, in_humid);

	getTempSetpoint(&v);

	String s =  "Consigne: " + String(v, 0) + "C";
	paint.Clear(UNCOLORED);
	paint.DrawStringAt(0, 30, s.c_str(), &Font24, COLORED);

	s =  "Interieur Temp: " + in_temp + "C";
	paint.DrawStringAt(0, 60, s.c_str(), &Font16, COLORED);
	s =  "Interieur Humidite: " + in_humid + "%";
	paint.DrawStringAt(0, 85, s.c_str(), &Font16, COLORED);

	//~ s =  "Exterieur Temp: " + out_temp + "C";
	//~ paint.DrawStringAt(0, 80, s.c_str(), &Font16, COLORED);
	//~ s =  "Exterieur Humidite: " + out_humid + "%";
	//~ paint.DrawStringAt(0, 105, s.c_str(), &Font16, COLORED);

	epd.SetPartialWindow(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();
	Serial.println(v);
	delay(20000);
  
}
