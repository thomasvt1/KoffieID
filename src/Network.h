#pragma once
#include <Arduino.h>
#include <HTTPClient.h>
#include "global.h"


class Network
{
    public:
        Network();
        String readWebsite(String UID);
        void firstCheckWiFi();
        void checkWiFi();
    private:
        HTTPClient http;
};