#pragma once
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <MFRC522.h>


extern Preferences preferences;
extern HardwareSerial Serial1;
extern AsyncWebServer server;
extern MFRC522 mfrc522;