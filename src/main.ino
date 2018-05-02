#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>     // RC522 Module uses SPI protocol
#include <MFRC522.h> // Library for Mifare RC522 Devices
#include <HTTPClient.h>

#include "helpers.h"
#include "global.h"

const char *host = "thomasvt.xyz";

void startWiFiSetup();
void startSoftAP();
void setupWebServer();

MFRC522 mfrc522; // Create MFRC522 instance


HardwareSerial Serial1(1);

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    delay(100);

    preferences.begin("KoffieID", false); // Start preference Library, false sets readOnly.

    // Start WiFi setup and stop doing anything else if WiFi is not setup.
    if (preferences.getString("ssid").equals("") || preferences.getString("wpa2").equals(""))
    {
        startWiFiSetup();
        return;
    }

    // Enable a way to enable setup mode after setup.
    if (!preferences.getBool("setup"))
    {
        Serial.println(F("\nSetup mode will be enabled if the ESP restarts within 5 seconds!"));
        preferences.putBool("setup", true);
        delay(5000);
        preferences.putBool("setup", false);
        Serial.println(F("Setup mode timeout has passed! - Starting normal!"));
    }

    // If setup mode is enabled.
    if (preferences.getBool("setup"))
    {
        Serial.println(F("\n!!! SETUP MODE ENABLED !!!"));
        WiFi.mode(WIFI_AP_STA);
        startSoftAP();
        setupWebServer();
        Serial.println(F("!!! SETUP MODE ENABLED !!!\n"));
    }

    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(preferences.getString("ssid"));

    
    // Start WiFi connecting process with setup connection details. 
    WiFi.begin(preferences.getString("ssid").c_str(), preferences.getString("wpa2").c_str());

    int tick = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (tick == 60)
            Serial.println(F("\nConnecting to network seems to be a problem?"));

        delay(500);
        Serial.print(".");
        tick++;
    }

    if (preferences.getBool("setup"))
    {
        Serial.println(F("Succesfully connected to network. Next reboot setup mode will be disabled!"));
        preferences.putBool("setup", false);
    }
    else
        WiFi.mode(WIFI_STA);

    Serial.println();
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());

    SPI.begin();                       // Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

String readWebsite(String UID)
{
    HTTPClient http;

    String url = "https://";
    url += host;
    url += "/api/koffieid.php?uid=";
    url += UID;

    http.begin(url);           //Specify the URL
    int httpCode = http.GET(); //Make the request

    if (httpCode > 0)
        return http.getString();
    else
        Serial.println(F("Error on HTTP request"));

    http.end(); //Free the resources
    return "";
}

String lastUid = "";
void loop()
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

    int disallowedPins[] = {5, 18, 19, 22, 23};

    if (!rfidUid.equals(lastUid))
    {
        Serial.print(F("Card scanned - "));
        String in = readWebsite(rfidUid);

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
    {
        Serial.println(F("This tag has already been scanned!"));
    }
    mfrc522.PICC_HaltA();
}