#include "network.h"

Network::Network()
{
    http.setReuse(true);
    http.setUserAgent("KoffieID");
}

void Network::firstCheckWiFi()
{
    unsigned char tick = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (tick == 60)
            Serial.println(F("\nConnecting to network seems to be a problem?"));

        delay(500);
        Serial.print(".");
        tick++;
    }
}

void Network::checkWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;
    
    Serial.println("WiFi problem detected! - Trying to reconnect!");
    //TODO: No WiFi reconnect working?
}

String Network::readWebsite(String UID)
{
    String url = "https://";
    url += preferences.getString("domain");
    url += "/api/koffieid.php?uid=";
    url += UID;

    http.begin(url);           // Specify the URL
    int httpCode = http.GET(); // Make the request

    if (httpCode > 0)
        return http.getString();
    else
        Serial.println(F("Error on HTTP request"));

    http.end(); //Free the resources
    return "";
}