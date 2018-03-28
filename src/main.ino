/*
 *  This sketch sends data via HTTP GET requests.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <SPI.h>     // RC522 Module uses SPI protocol
#include <MFRC522.h> // Library for Mifare RC522 Devices
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#include "helpers.h"
#include "global.h"

const char *host = "thomasvt.xyz";

void startWiFiSetup();
void startSoftAP();
void setupWebServer();

MFRC522 mfrc522; // Create MFRC522 instance

void setup()
{
    EEPROM.begin(512);
    Serial.begin(9600);
    delay(10);

    if (!ReadConfig())
    {
        startWiFiSetup();
        return;
    }

    // We start by connecting to a WiFi network

    Serial.print("Connecting to ");
    Serial.println(config.ssid);

    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    int tick = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (tick == 60)
        {
            Serial.println();
            Serial.println("No network has been found in 30 seconds! - Enabling setup aswell!");
            WiFi.mode(WIFI_AP_STA);
            startSoftAP();
            setupWebServer();
        }

        delay(500);
        Serial.print(".");
        tick++;
    }

    WiFi.mode(WIFI_STA);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
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
        Serial.println("Error on HTTP request");

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

    if (!rfidUid.equals(lastUid))
    {
        Serial.println(F("Card scanned"));
        Serial.println(readWebsite(rfidUid));

        lastUid = rfidUid;
    }

    mfrc522.PICC_HaltA();
}