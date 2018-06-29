#pragma once
#include <Arduino.h>
#include <MFRC522.h>
#include <WiFi.h>

#include "SetupHelper.h"
#include "CardReader.h"
#include "esp_wpa2.h"
#include "Network.h"


String lastUid = "";

AsyncWebServer server(80);
Preferences preferences;

HardwareSerial Serial1(1);
MFRC522 mfrc522;

SetupHelper setuphelper;
CardReader cardreader;
Network network;