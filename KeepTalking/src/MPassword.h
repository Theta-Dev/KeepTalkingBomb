#pragma once
#include "Module.h"

#define PWD_PIXEL 1
#define PWD_BUTTON_UP 12
#define PWD_BUTTON_DOWN 17
#define PWD_BUTTON_OK 11

#define PWD_NUM_CHARS 5
#define PWD_NUM_POSITIONS 6

#define PASSWORDS_LENGTH 35
const char PROGMEM pgm_passwords[] = {
    'A','B','O','U','T',
    'A','F','T','E','R',
    'A','G','A','I','N',
    'B','E','L','O','W',
    'C','O','U','L','D',
    'E','V','E','R','Y',
    'F','I','R','S','T',
    'F','O','U','N','D',
    'G','R','E','A','T',
    'H','O','U','S','E',
    'L','A','R','G','E',
    'L','E','A','R','N',
    'N','E','V','E','R',
    'O','T','H','E','R',
    'P','L','A','C','E',
    'P','L','A','N','T',
    'P','O','I','N','T',
    'R','I','G','H','T',
    'S','M','A','L','L',
    'S','O','U','N','D',
    'S','P','E','L','L',
    'S','T','I','L','L',
    'S','T','U','D','Y',
    'T','H','E','I','R',
    'T','H','E','R','E',
    'T','H','E','S','E',
    'T','H','I','N','G',
    'T','H','I','N','K',
    'T','H','R','E','E',
    'W','A','T','E','R',
    'W','H','E','R','E',
    'W','H','I','C','H',
    'W','O','R','L','D',
    'W','O','U','L','D',
    'W','R','I','T','E'
};

#define WRONGCHARS_LENGTH 12
const char PROGMEM pgm_wrongChars[] = {
    'D','I','J','K','M','Q','U','V','X','Y','Z','D',
    'C','D','J','K','N','Q','S','U','W','X','Y','Z',
    'B','C','D','F','J','K','M','N','P','Z','S','W',
    'B','F','J','K','M','P','Q','V','W','X','Y','Z',
    'A','B','C','F','I','J','M','O','P','V','S','U'
};


class MPassword : public Module
{
private:
    char characters[PWD_NUM_CHARS][PWD_NUM_POSITIONS];
    uint8_t solution;
    uint8_t selectors[PWD_NUM_CHARS];

    bool act = true;
    uint8_t arrows[2];

public:
    MPassword() {
        statusPixel = PWD_PIXEL;
    }

    bool menu()
    {
        return true;
    }

    void reset()
    {
        // Empty character array
        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            for(uint8_t i=0; i<PWD_NUM_POSITIONS; i++) characters[n][i] = 0;
            selectors[n] = 0;
        }

        // Insert random word
        solution = random(PASSWORDS_LENGTH);

        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            //insertCharacter(n, pgm_read_byte_near(pgm_passwords + PWD_NUM_CHARS*solution + n));
            characters[n][0] = pgm_read_byte_near(pgm_passwords + PWD_NUM_CHARS*solution + n);
        }

        // Insert 2 right characters per digit
        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            for(uint8_t i=1; i<3; i++) {
                uint8_t rnd = random(PASSWORDS_LENGTH);
                char c;

                // Check if character is already present
                bool match = true;
                while(match) {
                    c = pgm_read_byte_near(pgm_passwords + PWD_NUM_CHARS*rnd + n);

                    for(uint8_t j=0; j<i; j++) {
                        match = characters[n][j] == c;
                        if(match) {
                            if(rnd < PASSWORDS_LENGTH-1) rnd++;
                            else rnd = 0;
                            break;
                        }
                    }
                }
                characters[n][i] = c;
            }
        }

        
        // Uncomment for testing wrong match check
        /*
        characters[0][1] = 'G';
        characters[1][1] = 'W';
        characters[2][1] = 'E';
        characters[3][1] = 'A';
        characters[4][1] = 'T';
        characters[1][2] = 'R';
        */
        /*
        solution = 14;
        characters[0][0] = 'P';
        characters[1][0] = 'L';
        characters[2][0] = 'A';
        characters[3][0] = 'C';
        characters[4][0] = 'E';
        characters[0][1] = 'T';
        characters[1][1] = 'H';
        characters[2][1] = 'R';
        characters[3][1] = 'E';
        characters[4][1] = 'X';
        */
        
        // Check for wrong matches
        for(uint8_t i=0; i<PASSWORDS_LENGTH; i++) {
            if(i == solution) continue;
            
            bool match;
            uint8_t pos = 0;

            for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
                for(uint8_t p=0; p<3; p++) {
                    match = pgm_read_byte_near(pgm_passwords + PWD_NUM_CHARS*i + n) == characters[n][p];
                    if(match) {
                        if(n == 1) pos = p;
                        break;
                    }
                }
                if(!match) break;
            }

            // Correct wrong match by replacing one character
            if(match) {
                characters[1][pos] = pgm_read_byte_near(pgm_wrongChars + WRONGCHARS_LENGTH + random(WRONGCHARS_LENGTH));
            }
        }


        // Insert 3 wrong characters per digit
        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            for(uint8_t i=3; i<PWD_NUM_POSITIONS; i++) {
                uint8_t rnd = random(WRONGCHARS_LENGTH);
                //uint8_t rnd = 10;
                char c;

                // Check if character is already present
                bool match = true;
                while(match) {
                    c = pgm_read_byte_near(pgm_wrongChars + WRONGCHARS_LENGTH*n + rnd);

                    for(uint8_t j=0; j<i; j++) {
                        match = characters[n][j] == c;
                        if(match) {
                            if(rnd < WRONGCHARS_LENGTH-1) rnd++;
                            else rnd = 0;
                            break;
                        }
                    }
                }
                characters[n][i] = c;
            }
        }
        
        // Shuffle characters
        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            for(uint8_t i=0; i<PWD_NUM_POSITIONS; i++) {
                uint8_t rnd = random(PWD_NUM_POSITIONS);
                
                char tmp = characters[n][i];
                characters[n][i] = characters[n][rnd];
                characters[n][rnd] = tmp;
            }
        }
    }

    void setup()
    {
        state = 2;
    }
    
    void run()
    {
        // Handle button input
        int in = inputClicked(PWD_BUTTON_UP, PWD_NUM_CHARS);
        if(in > -1) {
            if(selectors[in] >= PWD_NUM_POSITIONS-1) selectors[in] = 0;
            else selectors[in]++;
            act = true;
            click();
        }
        in = inputClicked(PWD_BUTTON_DOWN, PWD_NUM_CHARS);
        if(in > -1) {
            if(selectors[in] < 1) selectors[in] = PWD_NUM_POSITIONS-1;
            else selectors[in]--;
            act = true;
            click();
        }
        if(inputClicked(PWD_BUTTON_OK)) {
            bool ok = true;
            for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
                if(pgm_read_byte_near(pgm_passwords + PWD_NUM_CHARS*solution + n) != characters[n][selectors[n]]) {
                    ok = false;
                    break;
                }
            }

            if(ok) defused();
            else add_strike();
        }

        for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
            for(uint8_t i=0; i<2; i++) {
                bool v = inputPressed(PWD_BUTTON_UP + i*PWD_NUM_CHARS + n);
                if(v != bitRead(arrows[i], n)) {
                    bitWrite(arrows[i], n, v);
                    act = true;
                }
            }
        }

        if(act)
        {
            // Display selected characters
            dpPwd.clearDisplay();

            for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
                dpPwd.drawChar(28*n, 20, characters[n][selectors[n]], WHITE, BLACK, 3);
            }

            // Display arrows
            for(uint8_t n=0; n<PWD_NUM_CHARS; n++) {
                if(bitRead(arrows[0], n)) dpPwd.fillTriangle(28*n, 7, 28*n + 14, 7, 28*n + 7, 0, WHITE);
                else dpPwd.drawTriangle(28*n, 7, 28*n + 14, 7, 28*n + 7, 0, WHITE);

                if(bitRead(arrows[1], n)) dpPwd.fillTriangle(28*n, 56, 28*n + 14, 56, 28*n + 7, 63, WHITE);
                dpPwd.drawTriangle(28*n, 56, 28*n + 14, 56, 28*n + 7, 63, WHITE);
            }

            dpPwd.display();
            act = false;
        }
    }
};