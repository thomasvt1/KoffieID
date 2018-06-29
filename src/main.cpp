#include <Arduino.h>
#include "global.h"
#include "local.h"
#include "main.h"


void setup()
{
    Serial.begin(9600);
    Serial1.begin(115200);
    delay(100);

    preferences.begin("KoffieID", false); // Start preference Library, false sets readOnly so config can be written.

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
    if (preferences.getBool("setup"))
        setuphelper.enableSetupMode();
    else
        setuphelper.startDelay();
        

    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(preferences.getString("ssid"));
    
    // Start WiFi connecting process with setup connection details. 
    if (preferences.getString("ssid", "").startsWith("eduroam"))
    {
        Serial.println(F("Special eduroam setup loaded..."));
        String EAP_IDENTITY = preferences.getString("ssid");
        EAP_IDENTITY.replace("eduroam/", "");
        EAP_IDENTITY += "@hr.nl";
        Serial.println(EAP_IDENTITY);
        String EAP_PASSWORD = preferences.getString("wpa2");

        const char * IDENTITY = EAP_IDENTITY.c_str();
        const char * PASSWORD = EAP_PASSWORD.c_str();

        delay(500);

        WiFi.disconnect(true);
        esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)IDENTITY, EAP_IDENTITY.length()); //provide identity
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)IDENTITY, EAP_IDENTITY.length()); //provide username
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)PASSWORD, EAP_PASSWORD.length()); //provide password
        esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config to default (fixed for 2018 and Arduino 1.8.5+)
        esp_wifi_sta_wpa2_ent_enable(&config); //set config to enable function (fixed for 2018 and Arduino 1.8.5+)

        WiFi.setHostname("KoffieID"); //set Hostname for your device - not neccesary

        WiFi.begin("eduroam");

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("eduroam connected!");
    }
    else
        WiFi.begin(preferences.getString("ssid").c_str(), preferences.getString("wpa2").c_str());

    WiFi.setAutoReconnect(true);

    network.firstCheckWiFi();

    if (preferences.getBool("setup"))
    {
        Serial.println(F("Succesfully connected to network. Next reboot setup mode will be disabled!"));
        preferences.putBool("setup", false);
    }
    else if (!preferences.getString("ssid", "").startsWith("eduroam"))
        WiFi.mode(WIFI_STA);

    Serial.println();
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());

    SPI.begin();                       // Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

void loop()
{
    if (!preferences.getBool("setup"))
        network.checkWiFi();
    cardreader.loop();
}