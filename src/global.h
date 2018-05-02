#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>

AsyncWebServer server(80);
Preferences preferences;