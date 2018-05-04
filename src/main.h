#pragma once
#include <Arduino.h>
#include <MFRC522.h>

#include "SetupHelper.h"
#include "CardReader.h"
#include "Network.h"

String lastUid = "";

AsyncWebServer server(80);
Preferences preferences;

HardwareSerial Serial1(1);
MFRC522 mfrc522;

SetupHelper setuphelper;
CardReader cardreader;
Network network;