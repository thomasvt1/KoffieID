#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

class Network
{
    public:
        String readWebsite(String UID);
    private:
        HTTPClient http;
        const char *host = "ip.thomasvt.xyz";

};

