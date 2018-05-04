#pragma once
#include <Arduino.h>
#include "global.h"
#include <MFRC522.h>

class CardReader
{
    public:
        void loop();
        
    private:
        String lastUid = "";
        boolean arrayIncludeElement(int array[], int element);
        int disallowedPins[5] = {5, 18, 19, 22, 23};
};