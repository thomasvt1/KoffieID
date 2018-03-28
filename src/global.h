#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

struct strConfig {
	String ssid;
	String password;

}   config;

boolean ReadConfig()
{
	Serial.println("Reading Configuration");
	if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
	{
		Serial.println("Configuration Found!");

		config.ssid = ReadStringFromEEPROM(64);
		config.password = ReadStringFromEEPROM(96);

		return true;
		
	}
	else
	{
		Serial.println("Configuration NOT FOUND");
		return false;
	}
}

void WriteConfig()
{
	Serial.println("Writing Config");
	EEPROM.write(0,'C');
	EEPROM.write(1,'F');
	EEPROM.write(2,'G');

	WriteStringToEEPROM(64, config.ssid);
	WriteStringToEEPROM(96, config.password);

	EEPROM.commit();
	ESP.restart();
}