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

#include "helpers.h"
#include "global.h"

#include "page_admin.h"
#include "page_style.css.h"

const char *host = "google.com";

void readWebsite();
void handleRoot();
void startWiFiSetup();

void setup()
{
    EEPROM.begin(512);
    Serial.begin(9600);
    delay(10);

    /*
    for (int i = 0 ; i < 512 ; i++) {
    EEPROM.write(i, 0);
    }
    EEPROM.commit();
    */

    if (!ReadConfig())
    {

        startWiFiSetup();
        return;
    }

    const int RST_PIN = 22; // Reset pin
    const int SS_PIN = 21;  // Slave select pin

    MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

    // We start by connecting to a WiFi network

    Serial.print("Connecting to ");
    Serial.println(config.ssid);

    WiFi.begin(config.ssid.c_str(), config.password.c_str());

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void readWebsite()
{
    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/input/";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("end of session");
}

void loop()
{
}
