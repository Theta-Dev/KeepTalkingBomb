#pragma once

#define MAX_STRIKES 2

class MTimer : public Module
{
private:
    void displayTime(uint64_t t)
    {   
        uint16_t secs = t / 1000;
        uint8_t minute = floor(secs / 60);
        uint8_t second = secs % 60;

        timerDigits[0] = floor(minute / 10);
        timerDigits[1] = minute % 10;
        timerDigits[2] = floor(second / 10);
        timerDigits[3] = second % 10;
        
        if(t >= 60000) {
            for(int i=0; i<4; i++) max.setDigit(0, i, timerDigits[i], i==1);
        }
        else {
            for(int i=0; i<2; i++) max.setDigit(0, i, timerDigits[i+2], i==0);

            uint32_t hsecs = (t - 1000*secs) / 10;
            max.setDigit(0, 2, floor(hsecs / 10), 0);
            max.setDigit(0, 3, hsecs % 10, 0);
        }
    }

public:
    void reset()
    {
        startTime = 0;
        strikes = 0;
    }

    void menu()
    {
        displayTime(setTime);
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
            if(hcMode) {
                for(int i=0; i<2; i++) max.setLed(0, 5, 6+i, blinkCtr<10);
                if(strikes > 0) state = 5;
            }
            else {
                for(int i=0; i<2; i++) max.setLed(0, 5, 6+i, strikes>i);
                if(strikes > MAX_STRIKES) state = 5;
            }
        }
        else {
            displayTime(0);
            state = 5;
        }
        
    }
};