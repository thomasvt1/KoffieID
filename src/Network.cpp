#include "network.h"


String Network::readWebsite(String UID)
{
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