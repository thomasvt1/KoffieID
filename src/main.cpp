#include <Arduino.h>
#include "global.h"
#include "main.h"

void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    delay(100);

    preferences.begin("KoffieID", false); // Start preference Library, false sets readOnly.

    // Start WiFi setup and stop doing anything else if WiFi is not setup.
    if (!setuphelper.isNetworkSetup())
    {
        setuphelper.startWiFiSetup();
        return;
    }

    /*
    *   Allow user to press reset within 3 seconds of boot to enable setup mode mode.
    *   This allows to user to change WiFi settings without compromising security.
    */
    if (!preferences.getBool("setup"))
        setuphelper.startDelay();
    else
        setuphelper.enableSetupMode();

    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(preferences.getString("ssid"));

    
    // Start WiFi connecting process with setup connection details. 
    WiFi.begin(preferences.getString("ssid").c_str(), preferences.getString("wpa2").c_str());

    unsigned char tick = 0;
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

void loop()
{
    cardreader.loop();
}