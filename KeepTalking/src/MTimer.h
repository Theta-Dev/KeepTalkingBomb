#pragma once
#include "Module.h"

#define TIM_BUTTON_UP 12
#define TIM_BUTTON_DOWN 17
#define TIM_BUTTON_OK 11
#define TIM_BUTTON_GO 0
#define TIM_NBUTTON 11
#define TIM_RGB 19
#define TIM_NUMRGB 2

#define TIM_NOPTIONS 4
#define TIM_NBOMBTYPES 4
#define TIM_BOMBCUSTOM 3

#define TIM_TIMEINC 30000
#define TIM_TIMEMAX 1800000
#define TIM_TIMEMIN 30000

#define MAX_STRIKES 2

const char PROGMEM pgm_bomb_types[TIM_NBOMBTYPES] = {'E', 'M', 'H', 'C'};

class MTimer : public Module
{
private:
    char act = 5;
    uint8_t module_pwd = 0;
    uint8_t menuState = 0;
    
    void displayTime(uint64_t t)
    {   
        //if(t >= 60000) {
            uint16_t secs = t / 1000;
            uint8_t minute = floor(secs / 60);
            uint8_t second = secs % 60;

            timerDigits[0] = floor(minute / 10);
            timerDigits[1] = minute % 10;
            timerDigits[2] = floor(second / 10);
            timerDigits[3] = second % 10;

            for(int i=0; i<4; i++) max.setDigit(0, i, timerDigits[i], i==1);
        /*}
        else {
            String zsecs = String((uint32_t) t/10);
            
            for(int i=3; i>=0; i--) {
                timerDigits[i] = zsecs.charAt(zsecs.length()-4+i) - 48;
                max.setDigit(0, i, timerDigits[i], i==0);
            }
        }*/
    }

public:
    void reset()
    {
        startTime = 0;
        strikes = 0;
    }

    bool menu()
    {
        if(menuState == 0)
        {
            // Handle buttons
            char inc = 1;
            int in = inputClicked(TIM_BUTTON_UP, TIM_NOPTIONS);
            if(in == -1) {
                inc = -1;
                in = inputClicked(TIM_BUTTON_DOWN, TIM_NOPTIONS);
            }
            if(act == -1) act = in;

            switch (in)
            {
            case 0: // Bomb type
                bombType += inc;
                if(bombType >= TIM_NBOMBTYPES) bombType = inc>0 ? 0:TIM_NBOMBTYPES-1;
                break;
            case 1: // Time
                setTime += (TIM_TIMEINC * inc);
                if(setTime > TIM_TIMEMAX || setTime < TIM_TIMEMIN) setTime = inc>0 ? TIM_TIMEMAX:TIM_TIMEMIN;
                break;
            case 2: // Hardcore mode
                hcMode = !hcMode;
                break;
            case 3: // Password/Morse
                if(bombType == TIM_BOMBCUSTOM) module_pwd += inc;
                if(module_pwd > 2) module_pwd = inc>0 ? 0:2;
                break;
            default:
                break;
            }

            if(inputClicked(TIM_BUTTON_OK)) {
                menuState = 1;
                act = 5;
                return false;
            }
            
            // Update display
            if(act > -1)
            {
                displayTime(setTime);
                
                dpPwd.clearDisplay();
                dpPwd.setTextColor(WHITE);
                dpPwd.setTextSize(1);
                
                // Bomb type
                dpPwd.drawChar(7, 15, pgm_read_byte_near(pgm_bomb_types + bombType), WHITE, BLACK, 2);
                dpPwd.drawCircle(12, 21, 11, WHITE);

                // Time
                dpPwd.drawLine(37, 21, 37, 12, WHITE);
                dpPwd.drawLine(37, 21, 42, 26, WHITE);
                dpPwd.drawCircle(37, 21, 11, WHITE);

                // Hardcore mode
                if(hcMode) dpPwd.fillCircle(62, 21, 11, WHITE);
                else dpPwd.drawCircle(62, 21, 11, WHITE);
                dpPwd.setCursor(57, 18);
                dpPwd.setTextColor(hcMode ? BLACK : WHITE);
                dpPwd.print("Hc");

                // Password/Morse
                if(bombType == TIM_BOMBCUSTOM) {
                    if(module_pwd) dpPwd.fillCircle(87, 21, 11, WHITE);
                    else dpPwd.drawCircle(87, 21, 11, WHITE);
                    dpPwd.setCursor(79, 18);
                    dpPwd.setTextColor(module_pwd ? BLACK : WHITE);
                    if(module_pwd == 1) dpPwd.print("Pwd");
                    else if(module_pwd == 2) dpPwd.print("Mor");
                }
                
                dpPwd.setCursor(0, 45);
                dpPwd.setTextColor(WHITE);

                switch(act)
                {
                case 0:
                    dpPwd.print("Bomb: ");
                    if(bombType == 0) dpPwd.print("Easy");
                    else if(bombType == 1) dpPwd.print("Medium");
                    else if(bombType == 2) dpPwd.print("Hard");
                    else dpPwd.print("Custom");
                    break;
                case 1:
                    dpPwd.print("Time: ");
                    for(uint8_t i=0; i<4; i++) {
                        dpPwd.print(timerDigits[i]);
                        if(i==1) dpPwd.print(':');
                    }
                    break;
                case 2:
                    dpPwd.print("Hardcore: ");
                    dpPwd.print(hcMode ? "ON":"OFF");
                    break;
                case 3:
                    if(bombType == TIM_BOMBCUSTOM) {
                        if(module_pwd == 1) dpPwd.print("Password");
                        else if(module_pwd == 2) dpPwd.print("Morse");
                        else dpPwd.print("Module off");
                    }
                    break;
                case 5:
                    dpPwd.print("Welcome to KTANE");
                    break;
                default:
                    break;
                }
                
                // Start
                /*
                dpPwd.setCursor(109, 18);
                dpPwd.print("GO");
                dpPwd.drawCircle(114, 21, 11, WHITE);
                */
                dpPwd.display();

                act = -1;
            }
        }
        else if(menuState == 1)
        {
            if(act > -1) {
                dpPwd.clearDisplay();
                dpPwd.setTextColor(WHITE);
                dpPwd.setCursor(0,0);
                dpPwd.setTextSize(2);
                dpPwd.print("Ready?");

                dpPwd.setCursor(0,30);
                dpPwd.setTextSize(1);
                dpPwd.println("Let the games begin!");
                dpPwd.println("Press the red button to start");

                dpPwd.display();
                act = -1;
            }

            for(uint8_t i=0; i<TIM_NUMRGB; i++) {
                pixel.setPixelColor(TIM_RGB+i, blinkCtr<10 ? 255:0, 0, 0);
            }

            if(inputClicked(TIM_BUTTON_OK, TIM_NBUTTON) > -1) {
                menuState = 0;
                act = 5;
            }
            if(inputClicked(TIM_BUTTON_GO)) menuState = 2;
        }
        
        return menuState==2;
    }

    void setup()
    {
        state = 4;
        displayTime(setTime);
    }

    void run()
    {
        if(startTime == 0) startTime = millis();
        
        uint64_t elapsedTime = millis() - startTime;

        if(elapsedTime < setTime) {
            displayTime(setTime - elapsedTime);

            // Display strike indicator
            for(int i=0; i<2; i++) max.setLed(0, 5, 6+i, strikes>i);

            if(strikes > MAX_STRIKES) state = 5;
        }
        else {
            displayTime(0);
            state = 5;
        }
        
    }
};