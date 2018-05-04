#pragma once
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <MFRC522.h>
#include "Network.h"

extern AsyncWebServer server;
extern Preferences preferences;
extern Network network;
extern MFRC522 mfrc522;