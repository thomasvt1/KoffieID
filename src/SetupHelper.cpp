#include "SetupHelper.h"


// Return true if network settings have been set.
boolean SetupHelper::isNetworkSetup()
{
    return !(preferences.getString("ssid").equals("") && preferences.getString("wpa2").equals(""));
}

void SetupHelper::startDelay()
{
    Serial.println(F("\nSetup mode will be enabled if the ESP restarts within 3 seconds!"));
    preferences.putBool("setup", true);
    delay(3000);
    preferences.putBool("setup", false);
    Serial.println(F("Setup mode timeout has passed! - Starting normal!"));
}

void SetupHelper::enableSetupMode()
{
    Serial.println(F("\n!!! SETUP MODE ENABLED !!!"));
    WiFi.mode(WIFI_AP_STA);
    startSoftAP();
    setupWebServer();
    Serial.println(F("!!! SETUP MODE ENABLED !!!\n"));
}

void SetupHelper::startSoftAP()
{
    WiFi.softAP(ACCESS_POINT_NAME, ACCESS_POINT_PASSWORD);

    IPAddress myIP = WiFi.softAPIP(); //Get IP address
    Serial.print("HotSpot SSID: ");
    Serial.println(ACCESS_POINT_NAME);
    Serial.print("HotSpot PASSWD: ");
    Serial.println(ACCESS_POINT_PASSWORD);
    Serial.print("HotSpot IP: ");
    Serial.println(myIP);
}

void SetupHelper::setupWebServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", PAGE_NetworkConfiguration);
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        int args = request->args();

        if (args != 0)
        {
            request->send(200, "text/html", PAGE_WaitAndReload);

            String ssid = request->arg("ssid");
            String password = request->arg("password");

            preferences.putString("ssid", ssid);
            preferences.putString("wpa2", password);

            delay(500);

            ESP.restart();
        }
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/css", PAGE_Style_css);
    });

    server.begin();
}

void SetupHelper::startWiFiSetup()
{
    WiFi.mode(WIFI_AP);
    startSoftAP();
    setupWebServer();

    String connectMessage = "Please connect to $ssid WiFi - password:$password";
    connectMessage.replace("$ssid", ACCESS_POINT_NAME);
    connectMessage.replace("$password", ACCESS_POINT_PASSWORD);

    Serial.println(connectMessage);
}