#pragma once
#include "Module.h"

#define SIMON_PIXEL 17
#define SIMON_LENGTH 5
#define SIMON_BUTTON 38
#define SIMON_NBUTTON 4

#define SIMON_TIME_LIGHT 1000
#define SIMON_TIME_DELAY 500
#define SIMON_TIME_PAUSE 5000

#define SIMON_RED 0
#define SIMON_BLUE 1
#define SIMON_GREEN 2
#define SIMON_YELLOW 3

const uint8_t PROGMEM pgm_simon[] = {
    // No vowel
    SIMON_BLUE, SIMON_YELLOW, SIMON_GREEN, SIMON_RED,
    SIMON_RED, SIMON_BLUE, SIMON_YELLOW, SIMON_GREEN,
    SIMON_YELLOW, SIMON_GREEN, SIMON_BLUE, SIMON_RED,
    // Vowel
    SIMON_BLUE, SIMON_RED, SIMON_YELLOW, SIMON_GREEN,
    SIMON_YELLOW, SIMON_GREEN, SIMON_BLUE, SIMON_RED,
    SIMON_GREEN, SIMON_RED, SIMON_YELLOW, SIMON_BLUE
};
/*
const uint8_t simon_map[2][3][4] = {
  { // No Vowel
    {SIMON_BLUE, SIMON_RED, SIMON_GREEN, SIMON_YELLOW}, // No Strikes
    {SIMON_RED, SIMON_GREEN, SIMON_YELLOW, SIMON_BLUE}, // One Strike
    {SIMON_YELLOW, SIMON_RED, SIMON_BLUE, SIMON_GREEN}, // Two Strikes
  },
  { // Vowel
    {SIMON_BLUE, SIMON_GREEN, SIMON_YELLOW, SIMON_RED}, // No Strikes
    {SIMON_YELLOW, SIMON_RED, SIMON_BLUE, SIMON_GREEN}, // One Strike
    {SIMON_GREEN, SIMON_BLUE, SIMON_YELLOW, SIMON_RED}, // Two Strikes
  },
};*/


class MSimon : public Module
{
private:
    uint8_t sequence[SIMON_LENGTH];
    uint8_t seqPos;     // Index for sequence input
    uint8_t seqProg;    // Index for sequence progress

    uint8_t seqState;
    uint8_t seqIndex;   // Index for sequence display
    uint64_t seqTime;

public:
    MSimon() {
        statusPixel = SIMON_PIXEL;
    }
    
    void reset()
    {
        // Init variables
        seqPos = 0;
        seqProg = 0;
        seqState = 0;

        // Generate random sequence
        for(int i=0; i<SIMON_LENGTH; i++) sequence[i] = random(SIMON_NBUTTON);
    }

    void setup()
    {
        state = 2;
    }

    uint8_t getSolution()
    {
        return pgm_read_byte_near(pgm_simon + SIMON_NBUTTON*3*serialVowel + SIMON_NBUTTON*strikes + sequence[seqPos]);
        //return simon_map[serialVowel][strikes][seqPos];
    }

    void setLED(uint8_t c)
    {
        for(int i=0; i<SIMON_NBUTTON; i++) max.setLed(0, 7, i, i==c);
    }

    void run()
    {
        uint64_t now = millis();

        // Show sequence
        if(seqState == 0) {
            seqState = 1;
            seqTime = now;
            setLED(sequence[seqIndex]);
        }
        else if(seqState == 1) {
            if(now-seqTime >= SIMON_TIME_LIGHT) {
                seqState = 2;
                seqTime = now;
                setLED(5);
            }
        }
        else if(seqState == 2) {
            if(now-seqTime >= SIMON_TIME_DELAY) {
                seqIndex++;
                if(seqIndex > seqProg) {
                    seqIndex = 0;
                    seqState = 3;
                }
                else seqState = 0;
            }
        }
        else if(seqState == 3 && now-seqTime >= SIMON_TIME_PAUSE) {
            setLED(5);
            seqState = 0;
        }
        else if(seqState == 4 && now-seqTime >= SIMON_TIME_LIGHT) {
            setLED(5);
            seqState = 0;
        }

        // Read buttons
        int bt = inputClicked(SIMON_BUTTON, SIMON_NBUTTON);

        if(bt > -1) {
            // Check if button is correct
            if(bt == getSolution()) {
                seqPos++;                
                if(seqPos > seqProg) {
                    // Add 1 to the sequence
                    seqPos = 0;
                    seqIndex = 0;
                    seqProg++;
                    seqState = 4;
                }
                else {
                    seqIndex = 0;
                    seqState = 3;
                }
                tone(22, 400+100*bt, 200);
                if(seqProg >= SIMON_LENGTH) {
                    setLED(5);
                    defused();
                }
            }
            else {
                seqProg = 0;
                seqPos = 0;
                seqIndex = 0;
                seqState = 4;
                add_strike();
            }
            seqTime = now;
        }
    }
};