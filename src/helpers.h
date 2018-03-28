#pragma once
#include <EEPROM.h>

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