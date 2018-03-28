#include <Arduino.h>

#include "helpers.h"
#include "global.h"

#include "page_admin.h"
#include "page_style.css.h"

#define ACCESS_POINT_NAME "KoffieID"
#define ACCESS_POINT_PASSWORD "KoffieConfig"

void startSoftAP()
{
    WiFi.softAP(ACCESS_POINT_NAME, ACCESS_POINT_PASSWORD);

    IPAddress myIP = WiFi.softAPIP(); //Get IP address
    Serial.print("HotSpot IP: ");
    Serial.println(myIP);
}

void setupWebServer()
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

            config.ssid = ssid;
            config.password = password;

            WriteConfig();
        }
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/css", PAGE_Style_css);
    });

    server.begin();
}

void startWiFiSetup()
{

    WiFi.mode(WIFI_AP);
    startSoftAP();

    setupWebServer();

    String connectMessage = "Please connect to $ssid WiFi - password:$password";
    connectMessage.replace("$ssid", ACCESS_POINT_NAME);
    connectMessage.replace("$password", ACCESS_POINT_PASSWORD);

    Serial.println(connectMessage);
}