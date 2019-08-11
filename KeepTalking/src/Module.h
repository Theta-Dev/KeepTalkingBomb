#pragma once
#include <Arduino.h>

#define N_MODULE 12
#define N_MODULE_SLOTS 8

#define TIMER_ID 0
#define PWD_ID 1
#define MORSE_ID 2
#define PIN_ID 3
#define WIRE_ID 4
#define CWIRE_ID 5
#define KEYPAD_ID 6
#define MEMORY_ID 7
#define ONFIRST_ID 8
#define SIMON_ID 9
#define MAZE_ID 10
#define BUTTON_ID 11

#define MODULE_BLINKCTR_MAX 20
#define MODULE_BLINKCTR_TIM 100

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
    uint8_t slotID = 0;

    uint64_t blinkTime = 0;
    uint8_t blinkState = 0;
    uint8_t blinkCtr = 0;

    uint8_t melodyIndex = 0;
    uint8_t melodyEnd = 0;
    uint64_t melodyTime = 0;

    virtual void menu() = 0;
    virtual void reset() = 0;
    virtual void setup() = 0;
    virtual void run() = 0;

    void updateStatus() {
        // Blinking
        uint64_t now = millis();
        if(now - blinkTime >= MODULE_BLINKCTR_TIM) {
            blinkTime = now;
            blinkCtr++;
            if(blinkCtr >= MODULE_BLINKCTR_MAX) {
                blinkCtr = 0;
                blinkState = 0;
            }
        }

        // Melody
        if(melodyEnd > 0 && now >= melodyTime) {
            tone(PIN_BUZZER, mod_melody[0][melodyIndex], mod_melody[1][melodyIndex]);
            melodyIndex++;
            if(melodyIndex > melodyEnd) melodyEnd = 0;
            else melodyTime = now + 1.2*mod_melody[1][melodyIndex];
        }

        if(slotID == 0) return;

        // Module in setup (orange)
        if(state == 1) pixel.setPixelColor(statusPixel[slotID], 180, 75, 0);
        // Module in operation (blue)
        else if(state == 2) {
            if(blinkState == 1) pixel.setPixelColor(statusPixel[slotID], 255, 0, 0);
            else pixel.setPixelColor(statusPixel[slotID], 0, 0, 255);
        }
        // Module defused (green)
        else if(state == 3) pixel.setPixelColor(statusPixel[slotID], 0, 255, 0);
    }

    void defused() {
        state = 3;
        melodyIndex = 0;
        melodyEnd = 1;
    }

    void add_strike() {
        strikes++;
        blinkState = 1;
        blinkCtr = 0;
        melodyIndex = 2;
        melodyEnd = 3;
    }
};