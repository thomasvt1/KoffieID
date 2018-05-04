#include "CardReader.h"

void CardReader::loop()
{
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    byte readCard[4];

    String rfidUid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        rfidUid += String(mfrc522.uid.uidByte[i], HEX);
    }

    if (!rfidUid.equals(lastUid)) //Check if this is a new cup, to prevent accidental double actions.
    {
        Serial.print(F("Card scanned - "));
        String in = network.readWebsite(rfidUid);
        

        lastUid = rfidUid;

        if (in.length() == 0)
        {
            Serial.println(F("API returned empty response"));
            return;
        }

        int x = in.toInt();
        Serial.println(in);

        if (x < 1 || x > 35)     //Check if the given pin# is within the range of our ESP32
        {
            Serial.println(F("API tried to call pin thatis not in range!"));
        }
        else if (arrayIncludeElement(disallowedPins, x))    //Check if the array is in the blacklist - for example pins used by the RFID reader.
        {
            Serial.println(F("API tried to call pin that is in blacklist!"));
        }
        else
        {
            pinMode(x, OUTPUT);
            digitalWrite(x, HIGH);
            delay(2000);
            digitalWrite(x, LOW);
        }
    }
    else
        Serial.println(F("This tag has already been scanned!"));


    mfrc522.PICC_HaltA();
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