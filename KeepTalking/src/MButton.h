#pragma once

#define BUTTON_NUMRGB 2
#define BUTTON_NUMCOLOR 5
#define BUTTON_NUMLABEL 3
#define BUTTON_TIME 1000
#define BUTTON_BRIGHTNESS 1.8

#define BUTTON_DETONATE 0
#define BUTTON_ABORT    1
#define BUTTON_HOLD     2


class MButton : public Module
{
private:
    uint8_t buttonColor;
    uint8_t lightColor;
    uint8_t buttonLabel;
    uint8_t solution;

    uint64_t pressTime;
    uint8_t buttonState;

    void setButtonColor(uint8_t c)
    {
        for(int i=0; i<BUTTON_NUMRGB; i++)
            pixel.setPixelColor(RGB_BUTTON+i, colors[3*c]*pixelB*BUTTON_BRIGHTNESS, colors[3*c+1]*pixelB*BUTTON_BRIGHTNESS, colors[3*c+2]*pixelB*BUTTON_BRIGHTNESS);
    }

public:
    MButton() {
        slotID = 7;
    }

    void menu()
    {
        if(inputClicked(BTN_BUTTON)) toggleModule(BUTTON_ID);
    }
    
    void reset()
    {
        buttonState = 0;

        // Generate random colors/text
        buttonColor = random(1, BUTTON_NUMCOLOR);
        lightColor = random(1, BUTTON_NUMCOLOR);
        buttonLabel = random(BUTTON_NUMLABEL);

        if(lightColor == buttonColor) {
            lightColor++;
            if(lightColor >= BUTTON_NUMCOLOR) lightColor = 1;
        }

        // Determine the solution
        if(buttonColor==COLOR_BLUE && buttonLabel==BUTTON_ABORT) solution = 1;
        else if(batteryLevel>1 && buttonLabel==BUTTON_DETONATE) solution = 0;
        else if(buttonColor==COLOR_WHITE && bitRead(indicators, INDICATOR_CAR)) solution = 1;
        else if(batteryLevel>2 && bitRead(indicators, INDICATOR_FRK)) solution = 0;
        else if(buttonColor==COLOR_YELLOW) solution = 1;
        else if(buttonColor==COLOR_RED && buttonLabel==BUTTON_HOLD) solution = 0;
        else solution = 1;

        if(solution == 1) {
            if(lightColor == COLOR_BLUE) solution = 4;
            else if(lightColor == COLOR_YELLOW) solution = 5;
        }
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {       
        uint64_t now = millis();
        
        // Update LEDs
        setButtonColor(buttonColor);
        digitalWrite(PIN_LED_BUTTON+buttonLabel, 1);

        // Button logic
        if(buttonState == 0 && inputPressed(BTN_BUTTON)) {
            pressTime = now;
            buttonState = 1;
            click();
        }
        else if(buttonState == 1) {
            // Button pressed
            if(inputPressed(BTN_BUTTON)) {
                if(now-pressTime >= BUTTON_TIME) setButtonColor(lightColor);
            }
            else {
                // Button released
                buttonState = 0;

                if(now-pressTime < BUTTON_TIME) {
                    // Short press
                    if(solution == 0) defused();
                    else add_strike();
                }
                else {
                    // Long press
                    if(solution > 0) {
                        bool ok = false;
                        for(int i=0; i<4; i++) {
                            if(timerDigits[i] == solution) {
                                ok = true;
                                break;
                            }
                        }
                        if(ok) defused();
                        else add_strike();
                    }
                    else add_strike();
                }
            }
        }
    }

};