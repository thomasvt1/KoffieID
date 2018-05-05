#pragma once
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <MFRC522.h>
#include "Network.h"

extern HardwareSerial Serial1;
extern Preferences preferences;
extern AsyncWebServer server;
extern Network network;
extern MFRC522 mfrc522;