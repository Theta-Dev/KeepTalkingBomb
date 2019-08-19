#pragma once

#define MORSE_BASEFREQ 500
#define MORSE_TIME 300

#define MORSE_WORDLEN 6
#define MORSE_ROWLEN 7
#define MORSE_LENGTH 16

/*
Syntax:
- one byte per character
- two bits per sign (dot: 10, dash: 11)
- leading/trailing zeroes are ignored
*/

const uint8_t PROGMEM pgm_morse[] = {
    /*shell */ 5, B101010, B10101010, B10, B10111010, B10111010, 0,
    /*halls */ 15, B10101010, B1011, B10111010, B10111010, B101010, 0,
    /*slick */ 22, B101010, B10111010, B1010, B11101110, B111011, 0,
    /*trick */ 32, B11, B101110, B1010, B11101110, B111011, 0,
    /*boxes */ 35, B11101010, B111111, B11101011, B10, B101010, 0,
    /*leaks */ 42, B10111010, B10, B1011, B111011, B101010, 0,
    /*strobe*/ 45, B101010, B11, B101110, B111111, B11101010, B10,
    /*bistro*/ 52, B11101010, B1010, B101010, B11, B101110, B111111,
    /*flick */ 55, B10101110, B10111010, B1010, B11101110, B111011, 0,
    /*bombs */ 65, B11101010, B111111, B1111, B11101010, B101010, 0,
    /*break */ 72, B11101010, B101110, B10, B1011, B111011, 0,
    /*brick */ 75, B11101010, B101110, B1010, B11101110, B111011, 0,
    /*steak */ 82, B101010, B11, B10, B1011, B111011, 0,
    /*sting */ 92, B101010, B11, B1010, B1110, B111110, 0,
    /*vector*/ 95, B10101011, B10, B11101110, B11, B111111, B101110,
    /*beats */ 100, B11101010, B10, B1011, B11, B101010, 0
};

class MMorse : public Module
{
private:
    uint8_t selection;
    uint8_t solution;

    uint64_t morseTime;
    uint8_t morseState;
    uint8_t morseChar;
    uint8_t morseIndex;

    bool act;

    uint8_t readChar() {
        return pgm_read_byte_near(pgm_morse + MORSE_ROWLEN*solution + morseChar + 1);
    }

public:
    MMorse() {
        slotID = 2;
    }

    void menu()
    {
        
    }

    void reset()
    {
        selection = 0;
        solution = random(MORSE_LENGTH);

        morseTime = 0;
        morseState = 0;
        morseChar = 0;
        morseIndex = 0;
        act = 1;
    }

    void setup()
    {
        state = 2;
    }
    
    void run()
    {
        // Play back the morse signal
        uint64_t now = millis();

        if(morseState == 0 && now >= morseTime)
        {
            uint8_t character = readChar();

            if(bitRead(character, 7 - 2*morseIndex)) {
                bool signal = bitRead(character, 6 - 2*morseIndex);
                
                digitalWrite(PIN_MORSE, 1);
                
                if(signal) morseTime = now + MORSE_TIME*3;
                else morseTime = now + MORSE_TIME;

                morseState = 1;
            }

            // Incrementing index
            morseIndex++;

            if(morseIndex >= 4) {
                morseIndex = 0;
                morseState = 2;

                do {
                    morseChar++;
                    if(morseChar > MORSE_WORDLEN-1) {
                        morseChar = 0;
                        morseState = 3;
                    }
                } while(readChar() == 0);
            }
        }
        else if(now >= morseTime)
        {
            digitalWrite(PIN_MORSE, 0);

            if(morseState == 2) morseTime = now + MORSE_TIME*5;
            else if(morseState == 3) morseTime = now + MORSE_TIME*10;
            else morseTime = now + MORSE_TIME;

            morseState = 0;
        }

        // Handle buttons
        if(inputClicked(BTN_PWD_UP, PWD_NUM_CHARS) > -1) {
            selection++;
            if(selection >= MORSE_LENGTH) selection = 0;
            act = true;
            click();
        }
        if(inputClicked(BTN_PWD_DOWN, PWD_NUM_CHARS) > -1) {
            selection--;
            if(selection >= MORSE_LENGTH) selection = MORSE_LENGTH-1;
            act = true;
            click();
        }
        if(inputClicked(BTN_PWD_OK)) {
            if(selection == solution) defused();
            else add_strike();
        }

        // Display frequency
        if(act)
        {
            uint8_t freq = pgm_read_byte_near(pgm_morse + MORSE_ROWLEN*selection);
            
            dpPwd.clearDisplay();
            dpPwd.setTextColor(WHITE);
            dpPwd.setCursor(10, 30);
            dpPwd.setTextSize(2);

            dpPwd.print("3.");
            dpPwd.print(MORSE_BASEFREQ + freq);
            dpPwd.print(" MHz");

            // Display frequency bar
            dpPwd.drawLine(0, 7, 128, 7, WHITE);
            dpPwd.drawRect(map(freq, 0, 110, 0, 125), 0, 3, 15, WHITE);
            
            dpPwd.display();
            act = false;
        }
    }
};