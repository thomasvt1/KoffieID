#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

#define HOST "ip.thomasvt.xyz"

class Network
{
    public:
        Network();
        String readWebsite(String UID);
    private:
        HTTPClient http;
};