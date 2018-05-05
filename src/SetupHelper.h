#pragma once
#include <Arduino.h>

#include <Preferences.h>
#include <ESPAsyncWebServer.h>

#include "global.h"

#include "page_admin.h"
#include "page_style.css.h"

#define ACCESS_POINT_NAME "KoffieID"
#define ACCESS_POINT_PASSWORD "KoffieConfig"


class SetupHelper
{
    public:
        void startSoftAP();
        void startWiFiSetup();
        void setupWebServer();
        void startDelay();
        void enableSetupMode();
        boolean isNetworkSetup();
};