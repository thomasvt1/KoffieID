#ifndef GLOBAL_H
#define GLOBAL_H
#include <ESPAsyncWebServer.h>

struct strConfig {
	String ssid;
	String password;

}   config;





void WriteStringToEEPROM(int beginaddress, String string)
{
	char  charBuf[string.length()+1];
	string.toCharArray(charBuf, string.length()+1);
	for (int t=  0; t<sizeof(charBuf);t++)
	{
			EEPROM.write(beginaddress + t,charBuf[t]);
	}
}

String ReadStringFromEEPROM(int beginaddress)
{
	byte counter=0;
	char rChar;
	String retString = "";
	while (1)
	{
		rChar = EEPROM.read(beginaddress + counter);
		if (rChar == 0) break;
		if (counter > 31) break;
		counter++;
		retString.concat(rChar);

	}
	return retString;
}


AsyncWebServer server(80);

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
		Serial.println("Configuration NOT FOUND!!!!");
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
#endif