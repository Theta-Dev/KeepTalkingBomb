#pragma once

#define MAX_STRIKES 2

class MTimer : public Module
{
private:
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
            for(int i=0; i<2; i++) max.setLed(0, 5, 6+i, strikes>i);

            if(strikes > MAX_STRIKES) state = 5;
        }
        else {
            displayTime(0);
            state = 5;
        }
        
    }
};