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

float batVoltage = 0;

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
#define INDICATOR_FRK  2
#define INDICATOR_CAR  3


// Modules
#include "Module.h"
Module* modules[N_MODULE];

void setModule(uint8_t id, bool st)
{
    if(modules[id]->state == st) return;
    else if(st) {
        for(uint8_t i=0; i<N_MODULE; i++) {
            if(modules[i]->state > 0 && modules[id]->slotID == modules[i]->slotID) {
                modules[i]->state = 0;
            }
        }
    }
    modules[id]->state = st;
}

bool enableModule(uint8_t id)
{
    for(uint8_t i=0; i<N_MODULE; i++) {
        if(modules[i]->state > 0 && modules[id]->slotID == modules[i]->slotID) return false;
    }

    modules[id]->state = 1;
    return true;
}

void toggleModule(uint8_t id)
{
    if(bombType == BOMBCUSTOM) {
        setModule(id, modules[id]->state == 0);
        click();
    }
}

void disableModules() {
    for(uint8_t i=0; i<N_MODULE; i++) modules[i]->state = (i==TIMER_ID);
}

void statusPixelReset() {
    for(uint8_t i=0; i<N_MODULE_SLOTS; i++) pixel.setPixelColor(statusPixel[i], 0);
}

#include "Menu.h"

void mReset() {
    pixel.clear();
    pixel.show();
    inputReset();
    menuReset();
    dpPwd.clearDisplay();
    dpPwd.display();
    dpKpd.clearDisplay();
    dpKpd.display();
}

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
    pinMode(A0, INPUT);
    pinMode(A15, INPUT);
    randomSeed(analogRead(A15));

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
    inputReset();
    outputReset();

    batVoltage = analogRead(A0) / 102.4;
    
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
    // Set serial number
    serialEven = false;
    serialVowel = false;
    
    for(uint8_t i=0; i<4; i++) {
        serialNumber[i] = random(10, SEGCHARS_LENGTH+10);
        if(serialNumber[i] <= 12) serialVowel = true;
    }
    for(uint8_t i=4; i<8; i++) {
        serialNumber[i] = random(10);
        serialEven = serialNumber[i] % 2 == 0;
    }

    // Set indicators
    batteryLevel = random(1, 4);
    indicators = random(256);

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

uint8_t gameSetup()
{
    mReset();
    outputReset();
    uint8_t state;
    
    do {
        inputUpdate();
        state = 1;
        
        for(uint8_t i=0; i<N_MODULE; i++)
        {
            if(modules[i]->state == 1) {
                modules[i]->setup();
                state = 0;
            }
        }
        pixel.show();

        if(menuSetup()) return 2;
        modules[TIMER_ID]->setup();

    } while(state == 0);

    mReset();
    outputReset();

    do {
        inputUpdate();
        state = menuStart();
        pixel.show();
        modules[TIMER_ID]->setup();
    } while(state == 0);
    
    return state;
}

uint8_t gameRun()
{
    // Serial number
    for(uint8_t i=0; i<4; i++) max.setRow(MAX_7SEG, 7-i, pgm_read_byte_near(pgm_segChars + serialNumber[i]-SEGCHARS_LENGTH+1));
    for(uint8_t i=4; i<8; i++) max.setDigit(MAX_7SEG, 7-i, serialNumber[i], 0);
    
    // Battery level
    max.setLed(MAX_LEDS, LED_BAT1_R, LED_BAT1_C, batteryLevel>0);
    max.setLed(MAX_LEDS, LED_BAT2_R, LED_BAT2_C, batteryLevel>1);
    max.setLed(MAX_LEDS, LED_BAT3_R, LED_BAT3_C, batteryLevel>2);

    // Indicators
    for(uint8_t i=0; i<N_LED_OUT; i++) digitalWrite(PIN_LED_OUT+i, bitRead(indicators, i));
    
    mReset();
    uint8_t state;
    
    do {
        inputUpdate();
        statusPixelReset();
        
        state = 1;
        for(uint8_t i=0; i<N_MODULE; i++)
        {          
            if(modules[i]->state == 2) {
                modules[i]->run();
                state = 0;
            }
            else if(modules[i]->state == 4) modules[i]->run();
            else if(modules[i]->state == 5) return 2;

            modules[i]->updateStatus();
        }
        pixel.show();

    } while(state == 0);
    return state;
}

void gameFinal(uint8_t state)
{
    mReset();
    for(uint8_t i=0; i<N_MODULE; i++) {
        modules[i]->melodyEnd = 0;
        modules[i]->blinkCtr = 0;
        modules[i]->blinkState = 0;
    }

    if(state == 2) {
        digitalWrite(PIN_VIBRATOR, 1);
        delay(1000);
        digitalWrite(PIN_VIBRATOR, 0);
    }

    do {
        inputUpdate();

    } while(!menuFinal(state));
}