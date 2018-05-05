#include "CardReader.h"

void CardReader::loop()
{
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    String rfidUid = String();
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        rfidUid += String(mfrc522.uid.uidByte[i], HEX);
    }

    mfrc522.PICC_HaltA(); // Done with RFID scanning.

    if (rfidUid.equals(lastUid)) //Check if this is a new cup, to prevent accidental double actions.
    {
        Serial.println(F("This tag has already been scanned!"));
        return;
    }

    Serial.print(F("Card scanned - "));
    Serial.println(rfidUid);
    String in = network.readWebsite(rfidUid);

    if (in.length() == 0)
    {
        Serial.println(F("API returned empty response"));
        return;
    }

    lastUid = rfidUid;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(in);

    if (json.containsKey("pin"))
    {
        Serial.println("json contains pin!");

        int pin = json["pin"];

        if (pin < 1 || pin > 35)     //Check if the given pin# is within the range of our ESP32
        {
            Serial.println(F("API tried to call pin thatis not in range!"));
        }
        else if (arrayIncludeElement(disallowedPins, pin))    //Check if the array is in the blacklist - for example pins used by the RFID reader.
        {
            Serial.println(F("API tried to call pin that is in blacklist!"));
        }
        else
        {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
            delay(2000);
            digitalWrite(pin, LOW);
        }
    }
    else if (json.containsKey("serial"))
    {
        JsonArray& commands = json["serial"];
        for (String cmd : commands){
            Serial.println(cmd);
            Serial1.println(cmd);
            delay(100);
        }
        
    }
}

boolean CardReader::arrayIncludeElement(int array[], int element)
{
    int max = sizeof(&array) - 1;

    for (int i = 0; i < max; i++)
    {
        if (array[i] == element)
            return true;
    }
    return false;
}