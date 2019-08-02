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

uint8_t serialNumber[8];
bool serialEven;
bool serialVowel;
uint8_t timerDigits[4];
uint8_t batteryLevel;
uint8_t indicators;

#define INDICATOR_PAR  0
#define INDICATOR_FRK  1
#define INDICATOR_CAR  2

// Modules
#include "MTimer.h"
#include "MPassword.h"
#include "MWire.h"
#include "MButton.h"
#include "MSimon.h"
#include "MKeypad.h"
#include "MMaze.h"

#define N_MODULE 7
Module* modules[N_MODULE];


void gameBegin()
{
    pinMode(A14, INPUT);
    pinMode(A15, INPUT);
    randomSeed(analogRead(A14) * analogRead(A15));

    outputBegin();

    // Initialize modules
    modules[0] = new MTimer();
    modules[1] = new MPassword();
    modules[2] = new MWire();
    modules[3] = new MButton();
    modules[4] = new MSimon();
    modules[5] = new MKeypad();
    modules[6] = new MMaze();
}

/*
void gameMenu()
{
    bool state;
    
    do {
        inputUpdate();
        
        state = true;
        for(uint8_t i=0; i<N_MODULE; i++) {
            state = state && modules[i]->menu();
        }

    } while(!state);
}*/

void gameReset()
{
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


    // Reset all modules
    for(uint8_t i=0; i<N_MODULE; i++) {
        // Enable all modules (tmp)
        if(i != 2) modules[i]->state = 1;

        if(modules[i]->state) modules[i]->reset();
    }
}

void gameSetup()
{
    bool state;
    
    do {
        inputUpdate();
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