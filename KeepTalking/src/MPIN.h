#pragma once

#define PIN_NBUTTON 10
#define PIN_LENGTH 4
#define PIN_XMAX 8

const uint8_t pgm_pin_ytable[] PROGMEM = {
    91, 59, 16, 3,
    63, 7,  88, 39,
    72, 51, 73, 36,
    71, 54, 58, 25,
    30, 94, 14, 93,
    87, 35, 90, 45,
    56, 60, 17, 67,
    33, 85, 64, 38
};

class MPIN : public Module
{
private:
    //uint8_t pin[PIN_LENGTH];
    uint8_t solution[PIN_LENGTH];
    uint8_t xValues[PIN_LENGTH];
    uint8_t pinCursor;

    uint8_t buttonState;
    uint64_t buttonTim;

    void updateLeds() {
        for(uint8_t i=0; i<4; i++) max.setLed(MAX_LEDS, LED_PIN_R, LED_PIN_C+i, i<pinCursor);

        for(uint8_t i=0; i<3; i++) max.setLed(MAX_LEDS, LED_PINX_R, LED_PINX_C+i, pinCursor<PIN_LENGTH ? bitRead(xValues[pinCursor], 2-i) : 0);
    }

public:
    MPIN() {
        slotID = 4;
    }

    void menu()
    {
        if(inputClicked(BTN_PIN, PIN_NBUTTON+1) > -1) toggleModule(PIN_ID);
    }

    void reset()
    {
        pinCursor = 0;
        buttonState = 0;
        
        uint8_t sds = 0;
        for(uint8_t i=4; i<8; i++) sds += serialNumber[i];

        for(uint8_t i=0; i<PIN_LENGTH; i++) {
            uint8_t x = random(PIN_XMAX);
            uint8_t y = pgm_read_byte_near(pgm_pin_ytable + PIN_LENGTH*x + i);
            
            xValues[i] = x;

            if(x%2 == 0 && y%2 == 0)
                solution[i] = abs(y + x*serialNumber[5] - bitRead(indicators, INDICATOR_FRK)) % 10;
            else if(batteryLevel > 2 && y%2 == 1)
                solution[i] = abs(serialNumber[4]*serialNumber[5] + y - x) % 10;
            else if(sds > 12)
                solution[i] = abs(x*batteryLevel + y - serialNumber[4]) % 10;
            else solution[i] = abs(y + sds*bitRead(indicators, INDICATOR_CAR) - batteryLevel) % 10;
        }

        updateLeds();

        /*
        // Debug info
        dpKpd.clearDisplay();
        dpKpd.setTextColor(WHITE);
        dpKpd.setCursor(0,0);
        for(uint8_t i=0; i<PIN_LENGTH; i++) dpKpd.print(solution[i]);
        dpKpd.display();*/
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {
        // Handle button input
        char in = inputClicked(BTN_PIN, PIN_NBUTTON);

        if(in > -1) {
            in++;
            if(in == 10) in = 0;

            if(solution[pinCursor] == in) {
                pinCursor++;

                if(pinCursor < PIN_LENGTH) click();
                else defused();
            }
            else add_strike();
            updateLeds();
        }
    }
};