#pragma once
#include <Arduino.h>

#define debounceTime 30
#define holdTime 50
#define clickMaxTime 1000

#define debounceTime_wire 500
#define holdTime_wire 500

class Button
{
private:
    uint8_t buttonPin;
    uint8_t state = 0;
    bool click = false;
    uint64_t startTime;

public:
    Button(uint8_t pin)
    {
        buttonPin = pin;
        pinMode(buttonPin, INPUT_PULLUP);
    }

    void update()
    {
        uint64_t now = millis();
        bool pressed = !digitalRead(buttonPin);
        
        if(state == 0) {
            if(pressed) {
                state = 1;
                startTime = now;
            }
        }
        if(state == 1) {
            if((now - startTime) >= debounceTime) {
                if(pressed) state = 2;
                else {
                    click = true;
                    startTime = now;
                    state = 3;
                }
            }
            else if(!pressed) state = 0;
        }
        if(state == 2) {
            if(!pressed) {
                if((now - startTime) < clickMaxTime) click = true;
                startTime = now;
                state = 3;
            }
        }
        if(state == 3) {
            if((now - startTime) < holdTime) {
                if(pressed) state = 4;
            }
            else state = 0;
        }
        if(state == 4) {
            if(!pressed) {
                startTime = now;
                state = 3;
            }
        }
    }

    bool clicked()
    {
        if(click) {
            click = false;
            return true;
        }
        return false;
    }

    bool pressed()
    {
        return state > 1;
    }
};