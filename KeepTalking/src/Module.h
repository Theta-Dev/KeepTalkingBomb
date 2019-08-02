#pragma once
#include <Arduino.h>

#define MODULE_BLINKTIME 2000
#define MODULE_MELODY_DEFUSED 0
#define MODULE_MELODY_STRIKE 1

/*
Defused: 0-1
Strike: 2-3
*/

uint16_t mod_melody[2][4] = {
    {500, 700, 300, 200},
    {200, 100, 200, 300}
};

class Module
{
public:
    uint8_t state = 0;
    uint8_t statusPixel = 255;
    uint64_t blinkTime = 0;
    uint8_t blinkState = 0;

    uint8_t melodyIndex = 0;
    uint8_t melodyEnd = 0;
    uint64_t melodyTime = 0;

    virtual void reset() = 0;
    virtual void setup() = 0;
    virtual void run() = 0;

    void updateStatus() {
        if(statusPixel == 255) return;

        uint64_t now = millis();

        // Module in setup (orange)
        if(state == 1) pixel.setPixelColor(statusPixel, 180, 75, 0);
        // Module in operation (blue)
        else if(state == 2) {
            if(blinkState == 1) {
                if(now - blinkTime >= MODULE_BLINKTIME) blinkState = 0;
                pixel.setPixelColor(statusPixel, 255, 0, 0);
            }
            else pixel.setPixelColor(statusPixel, 0, 0, 255);
        }
        // Module defused (green)
        else if(state == 3) pixel.setPixelColor(statusPixel, 0, 255, 0);
        // Module disabled (off)
        else pixel.setPixelColor(statusPixel, 0);

        // Melody
        if(melodyEnd > 0 && now >= melodyTime) {
            tone(PIN_BUZZER, mod_melody[0][melodyIndex], mod_melody[1][melodyIndex]);
            melodyIndex++;
            if(melodyIndex > melodyEnd) melodyEnd = 0;
            else melodyTime = now + 1.2*mod_melody[1][melodyIndex];
        }
    }

    void defused() {
        state = 3;
        melodyIndex = 0;
        melodyEnd = 1;
    }

    void add_strike() {
        strikes++;
        blinkState = 1;
        blinkTime = millis();
        melodyIndex = 2;
        melodyEnd = 3;
    }

    void click() {
        tone(PIN_BUZZER, 1000, 10);
    }
};