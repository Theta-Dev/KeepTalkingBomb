#pragma once
#include <Arduino.h>
#include "Input.h"
#include "Output.h"

#define SEGCHARS_LENGTH 11
const uint8_t PROGMEM pgm_segChars[] =
{
    B01110111,  // A
    B01001111,  // E
    B00111110,  // U
    B00011111,  // b
    B01001110,  // C
    B00111101,  // d
    B01000111,  // F
    B00110111,  // H
    B00111100,  // J
    B00001110,  // L
    B01100111   // B
};

uint64_t startTime = 0;
uint64_t setTime = 300000;
uint8_t strikes;
bool hcMode = 0;
uint8_t bombType = 0;

uint8_t serialNumber[8];
bool serialEven;
bool serialVowel;
uint8_t timerDigits[4];
uint8_t batteryLevel;
uint8_t indicators;

#define NBOMBTYPES 4
#define BOMBCUSTOM 3

#define INDICATOR_PAR  0
#define INDICATOR_FRK  1
#define INDICATOR_CAR  2


// Modules
#include "Module.h"
Module* modules[N_MODULE];
bool module_slots_en[N_MODULE_SLOTS];

bool enableModule(uint8_t id)
{
    if(module_slots_en[modules[id]->slotID]) return false;

    modules[id]->state = 1;
    module_slots_en[modules[id]->slotID] = 1;
    return true;
}

void setModule(uint8_t id, bool st)
{
    if(modules[id]->state == st) return;
    else if(!st) {
        modules[id]->state = 0;
        module_slots_en[modules[id]->slotID] = 0;
    }
    else {
        for(uint8_t i=0; i<N_MODULE; i++) {
            if(module_slots_en[modules[i]->slotID] == 1 && modules[id]->slotID == modules[i]->slotID) {
                modules[i]->state = 0;
                module_slots_en[modules[i]->slotID] = 0;
            }
        }
        modules[id]->state = 1;
        module_slots_en[modules[id]->slotID] = 1;
    }
}

void toggleModule(uint8_t id)
{
    if(bombType == BOMBCUSTOM) {
        setModule(id, modules[id]->state == 0);
        click();
    }
}

void disableModules() {
    for(uint8_t i=0; i<N_MODULE_SLOTS; i++) module_slots_en[i] = 0;
    for(uint8_t i=0; i<N_MODULE; i++) modules[i]->state = (i==TIMER_ID);
}

void statusPixelReset() {
    for(uint8_t i=0; i<N_MODULE_SLOTS; i++) pixel.setPixelColor(statusPixel[i], 0);
}

#include "Menu.h"

#include "MTimer.h"
#include "MPassword.h"
#include "MMorse.h"
#include "MPIN.h"
#include "MWire.h"
#include "MCWire.h"
#include "MKeypad.h"
#include "MMemory.h"
#include "MOnFirst.h"
#include "MSimon.h"
#include "MMaze.h"
#include "MButton.h"


void gameBegin()
{
    pinMode(A14, INPUT);
    pinMode(A15, INPUT);
    randomSeed(analogRead(A14) * analogRead(A15));

    outputBegin();

    // Initialize modules
    modules[TIMER_ID] = new MTimer();
    modules[PWD_ID] = new MPassword();
    modules[MORSE_ID] = new MMorse();
    modules[PIN_ID] = new MPIN();
    modules[WIRE_ID] = new MWire();
    modules[CWIRE_ID] = new MCWire();
    modules[KEYPAD_ID] = new MKeypad();
    modules[MEMORY_ID] = new MMemory();
    modules[ONFIRST_ID] = new MOnFirst();
    modules[SIMON_ID] = new MSimon();
    modules[MAZE_ID] = new MMaze();
    modules[BUTTON_ID] = new MButton();
}


void gameMenu()
{
    disableModules();
    menuReset();
    
    while(1) {
        inputUpdate();
        statusPixelReset();

        if(menuMain()) break;

        for(uint8_t i=0; i<N_MODULE; i++) {
            modules[i]->menu();
            if(bombType == BOMBCUSTOM) modules[i]->updateStatus();
        }
        pixel.show();
    }
}

void gameReset()
{
    outputReset();

    // Set serial number
    serialEven = false;
    serialVowel = false;
    
    for(uint8_t i=0; i<4; i++) {
        serialNumber[i] = random(10, SEGCHARS_LENGTH+10);
        if(serialNumber[i] <= 12) serialVowel = true;
        max.setRow(2, 7-i, pgm_read_byte_near(pgm_segChars + serialNumber[i]-SEGCHARS_LENGTH+1));
    }
    for(uint8_t i=4; i<8; i++) {
        serialNumber[i] = random(10);
        serialEven = serialNumber[i] % 2 == 0;
        max.setDigit(2, 7-i, serialNumber[i], 0);
    }

    // Set indicators
    batteryLevel = random(1, 4);
    max.setLed(0, 6, 6, batteryLevel>0);
    max.setLed(0, 6, 7, batteryLevel>1);
    max.setLed(0, 7, 7, batteryLevel>2);

    indicators = random(256);
    for(int i=0; i<3; i++) max.setLed(0, 7, 4+i, bitRead(indicators, i));

    // Enable modules
    if(bombType != BOMBCUSTOM) {
        disableModules();

        for(uint8_t i=0; i<pgm_read_byte_near(pgm_bomb_types_n + bombType); i++) {
            uint8_t rnd = random(1, N_MODULE);
            while(1) {
                if(enableModule(rnd)) break;
                else {
                    rnd++;
                    if(rnd >= N_MODULE) rnd = 1;
                }
            }
        }
    }


    // Reset all modules
    for(uint8_t i=0; i<N_MODULE; i++) {
        if(modules[i]->state == 1) modules[i]->reset();
    }
}

void gameSetup()
{
    bool state;
    
    do {
        inputUpdate();
        statusPixelReset();
        state = true;
        
        for(uint8_t i=0; i<N_MODULE; i++)
        {
            if(modules[i]->state == 1) {
                modules[i]->setup();
                state = false;
            }
            modules[i]->updateStatus();
        }
        pixel.show();

    } while(!state);
}

void gameRun()
{
    bool state;
    
    do {
        inputUpdate();
        statusPixelReset();
        
        state = true;
        for(uint8_t i=0; i<N_MODULE; i++)
        {          
            if(modules[i]->state == 2) {
                modules[i]->run();
                state = false;
            }
            else if(modules[i]->state == 4) modules[i]->run();
            else if(modules[i]->state == 5) return;

            modules[i]->updateStatus();
        }
        pixel.show();

    } while(!state);
}