#pragma once

#define ONFIRST_NBUTTON 6
#define ONFIRST_NSTAGE 3

const char onfirst_word00[] PROGMEM = "";
const char onfirst_word01[] PROGMEM = "BLANK";
const char onfirst_word02[] PROGMEM = "C";
const char onfirst_word03[] PROGMEM = "CEE";
const char onfirst_word04[] PROGMEM = "DISPLAY";
const char onfirst_word05[] PROGMEM = "DONE";
const char onfirst_word06[] PROGMEM = "FIRST";
const char onfirst_word07[] PROGMEM = "HOLD";
const char onfirst_word08[] PROGMEM = "HOLD ON";
const char onfirst_word09[] PROGMEM = "LEAD";
const char onfirst_word10[] PROGMEM = "LED";
const char onfirst_word11[] PROGMEM = "LEED";
const char onfirst_word12[] PROGMEM = "LEFT";
const char onfirst_word13[] PROGMEM = "LIKE";
const char onfirst_word14[] PROGMEM = "MIDDLE";
const char onfirst_word15[] PROGMEM = "NEXT";
const char onfirst_word16[] PROGMEM = "NO";
const char onfirst_word17[] PROGMEM = "NOTHING";
const char onfirst_word18[] PROGMEM = "OKAY";
const char onfirst_word19[] PROGMEM = "PRESS";
const char onfirst_word20[] PROGMEM = "READ";
const char onfirst_word21[] PROGMEM = "READY";
const char onfirst_word22[] PROGMEM = "RED";
const char onfirst_word23[] PROGMEM = "REED";
const char onfirst_word24[] PROGMEM = "RIGHT";
const char onfirst_word25[] PROGMEM = "SAYS";
const char onfirst_word26[] PROGMEM = "SEE";
const char onfirst_word27[] PROGMEM = "SURE";
const char onfirst_word28[] PROGMEM = "THEIR";
const char onfirst_word29[] PROGMEM = "THERE";
const char onfirst_word30[] PROGMEM = "THEY ARE";
const char onfirst_word31[] PROGMEM = "THEY'RE";
const char onfirst_word32[] PROGMEM = "U";
const char onfirst_word33[] PROGMEM = "UH HUH";
const char onfirst_word34[] PROGMEM = "UH UH";
const char onfirst_word35[] PROGMEM = "UHHH";
const char onfirst_word36[] PROGMEM = "UR";
const char onfirst_word37[] PROGMEM = "WAIT";
const char onfirst_word38[] PROGMEM = "WHAT";
const char onfirst_word39[] PROGMEM = "WHAT?";
const char onfirst_word40[] PROGMEM = "YES";
const char onfirst_word41[] PROGMEM = "YOU";
const char onfirst_word42[] PROGMEM = "YOU ARE";
const char onfirst_word43[] PROGMEM = "YOUR";
const char onfirst_word44[] PROGMEM = "YOU'RE";

const char* const pgm_onfirst_table[] PROGMEM = {
    onfirst_word00, onfirst_word01, onfirst_word02, onfirst_word03, onfirst_word04, onfirst_word05, onfirst_word06, onfirst_word07, onfirst_word08, onfirst_word09,
    onfirst_word10, onfirst_word11, onfirst_word12, onfirst_word13, onfirst_word14, onfirst_word15, onfirst_word16, onfirst_word17, onfirst_word18, onfirst_word19,
    onfirst_word20, onfirst_word21, onfirst_word22, onfirst_word23, onfirst_word24, onfirst_word25, onfirst_word26, onfirst_word27, onfirst_word28, onfirst_word29,
    onfirst_word30, onfirst_word31, onfirst_word32, onfirst_word33, onfirst_word34, onfirst_word35, onfirst_word36, onfirst_word37, onfirst_word38, onfirst_word39,
    onfirst_word40, onfirst_word41, onfirst_word42, onfirst_word43, onfirst_word44
};

#define ONFIRST_NDISPLAY 28
const uint8_t pgm_onfirst_display[] PROGMEM = {40,  6,  4, 18, 25, 17,  0,  1, 16, 10,  9, 20, 22, 23, 11,  8, 41, 42, 43, 44, 36, 29, 31, 28, 30, 26,  2,  3};
const uint8_t pgm_onfirst_dplabel[] PROGMEM = { 1,  3,  5,  3,  5,  1,  2,  4,  5,  1,  5,  4,  4,  2,  2,  5,  4,  5,  4,  4,  0,  5,  2,  4,  1,  5,  3,  5};

#define ONFIRST_NLABEL 28
#define ONFIRST_TWIDTH 14
const uint8_t pgm_onfirst_label[] PROGMEM = {21, 6, 16, 1, 17, 40, 38, 35, 12, 24, 14, 18, 37, 19, 41, 42, 43, 44, 36, 32, 33, 34, 39, 5, 15, 7, 27, 13};
/*
const uint8_t pgm_onfirst_lbtable[] PROGMEM = {
    40, 18, 38, 14, 12, 19, 24, 1, 21, 16, 6, 35, 17, 37,
    12, 18, 40, 14, 16, 24, 17, 35, 37, 21, 1, 38, 19, 6,
    1, 35, 37, 6, 38, 21, 24, 40, 17, 12, 19, 18, 16, 14,
    37, 24, 18, 14, 1, 19, 21, 17, 16, 38, 12, 35, 40, 6,
    35, 24, 18, 14, 40, 1, 16, 19, 12, 38, 37, 6, 17, 21,
    18, 24, 35, 14, 6, 38, 19, 21, 17, 40, 12, 1, 16, 37,
    35, 38, 12, 17, 21, 1, 14, 16, 18, 6, 37, 40, 19, 24,
    21, 17, 12, 38, 18, 40, 24, 16, 19, 1, 35, 14, 37, 6,
    24, 12, 6, 16, 14, 40, 1, 38, 35, 37, 19, 21, 18, 17,
    40, 17, 21, 19, 16, 37, 38, 24, 14, 12, 35, 1, 18, 6,
    1, 21, 18, 38, 17, 19, 16, 37, 12, 14, 24, 6, 35, 40,
    14, 16, 6, 40, 35, 17, 37, 18, 12, 21, 1, 19, 38, 24,
    35, 16, 1, 18, 40, 12, 6, 19, 38, 37, 17, 21, 24, 14,
    24, 14, 40, 21, 19, 18, 17, 35, 1, 12, 6, 38, 16, 37,
    27, 42, 43, 44, 15, 33, 36, 7, 39, 41, 34, 13, 5, 32,
    43, 15, 13, 33, 39, 5, 34, 7, 41, 32, 44, 27, 36, 42,
    34, 42, 33, 43, 15, 36, 27, 32, 44, 41, 39, 7, 13, 5,
    41, 44, 36, 15, 34, 42, 32, 43, 39, 33, 27, 5, 13, 7,
    5, 32, 36, 33, 39, 27, 43, 7, 44, 13, 15, 34, 42, 41,
    33, 27, 15, 39, 44, 36, 34, 5, 32, 41, 13, 7, 42, 43,
    33, 43, 42, 41, 5, 7, 34, 15, 27, 13, 44, 36, 32, 39,
    36, 32, 42, 44, 15, 34, 5, 41, 33, 13, 43, 27, 7, 39,
    41, 7, 44, 43, 32, 5, 34, 13, 42, 33, 36, 15, 39, 27,
    27, 33, 15, 39, 43, 36, 44, 7, 13, 41, 32, 42, 34, 5,
    39, 33, 34, 43, 7, 27, 15, 13, 5, 42, 36, 44, 32, 41,
    42, 32, 5, 34, 41, 36, 27, 39, 44, 15, 7, 33, 43, 13,
    42, 5, 13, 44, 41, 7, 33, 36, 27, 32, 39, 15, 43, 34,
    44, 15, 32, 36, 7, 5, 34, 39, 33, 41, 13, 27, 42, 43
};*/
const uint8_t pgm_onfirst_lbtable[] PROGMEM = {
    5, 11, 6, 10, 8, 13, 9, 3, 0, 2, 1, 7, 4, 12,
    8, 11, 5, 10, 2, 9, 4, 7, 12, 0, 3, 6, 13, 1,
    3, 7, 12, 1, 6, 0, 9, 5, 4, 8, 13, 11, 2, 10,
    12, 9, 11, 10, 3, 13, 0, 4, 2, 6, 8, 7, 5, 1,
    7, 9, 11, 10, 5, 3, 2, 13, 8, 6, 12, 1, 4, 0,
    11, 9, 7, 10, 1, 6, 13, 0, 4, 5, 8, 3, 2, 12,
    7, 6, 8, 4, 0, 3, 10, 2, 11, 1, 12, 5, 13, 9,
    0, 4, 8, 6, 11, 5, 9, 2, 13, 3, 7, 10, 12, 1,
    9, 8, 1, 2, 10, 5, 3, 6, 7, 12, 13, 0, 11, 4,
    5, 4, 0, 13, 2, 12, 6, 9, 10, 8, 7, 3, 11, 1,
    3, 0, 11, 6, 4, 13, 2, 12, 8, 10, 9, 1, 7, 5,
    10, 2, 1, 5, 7, 4, 12, 11, 8, 0, 3, 13, 6, 9,
    7, 2, 3, 11, 5, 8, 1, 13, 6, 12, 4, 0, 9, 10,
    9, 10, 5, 0, 13, 11, 4, 7, 3, 8, 1, 6, 2, 12,
    26, 15, 16, 17, 24, 20, 18, 25, 22, 14, 21, 27, 23, 19,
    16, 24, 27, 20, 22, 23, 21, 25, 14, 19, 17, 26, 18, 15,
    21, 15, 20, 16, 24, 18, 26, 19, 17, 14, 22, 25, 27, 23,
    14, 17, 18, 24, 21, 15, 19, 16, 22, 20, 26, 23, 27, 25,
    23, 19, 18, 20, 22, 26, 16, 25, 17, 27, 24, 21, 15, 14,
    20, 26, 24, 22, 17, 18, 21, 23, 19, 14, 27, 25, 15, 16,
    20, 16, 15, 14, 23, 25, 21, 24, 26, 27, 17, 18, 19, 22,
    18, 19, 15, 17, 24, 21, 23, 14, 20, 27, 16, 26, 25, 22,
    14, 25, 17, 16, 19, 23, 21, 27, 15, 20, 18, 24, 22, 26,
    26, 20, 24, 22, 16, 18, 17, 25, 27, 14, 19, 15, 21, 23,
    22, 20, 21, 16, 25, 26, 24, 27, 23, 15, 18, 17, 19, 14,
    15, 19, 23, 21, 14, 18, 26, 22, 17, 24, 25, 20, 16, 27,
    15, 23, 27, 17, 14, 25, 20, 18, 26, 19, 22, 24, 16, 21,
    17, 24, 19, 18, 25, 23, 21, 22, 20, 14, 27, 26, 15, 16
};


class MOnFirst : public Module
{
private:
    uint8_t display;
    uint8_t labels[ONFIRST_NBUTTON];
    uint8_t stage;
    uint8_t solution;

    char buffer[8];
    bool act;

    void readWord(uint8_t n) {
        strcpy_P(buffer, (char*)pgm_read_word(&(pgm_onfirst_table[n])));
    }
    void readDisplay(uint8_t n) {
        readWord(pgm_read_byte_near(pgm_onfirst_display+n));
    }
    void readLabel(uint8_t n) {
        readWord(pgm_read_byte_near(pgm_onfirst_label+n));
    }

    void updateLeds() {
        for(uint8_t i=0; i<4; i++) max.setLed(MAX_LEDS, LED_KEYPAD_R, LED_KEYPAD_C+i, i<stage);
    }

    void resetStage()
    {
        act = 1;
        
        // Generate random display
        display = random(ONFIRST_NDISPLAY);

        // Generate random labels
        for(uint8_t i=0; i<ONFIRST_NBUTTON; i++) {
            uint8_t rnd = random(ONFIRST_NLABEL);

            // Check for duplicates
            bool tr = 1;
            while(tr) {
                tr = 0;
                for(uint8_t j=0; j<i; j++) {
                    if(labels[j] == rnd) {
                        tr = 1;
                        rnd++;
                        if(rnd >= ONFIRST_NLABEL) rnd = 0;
                        break;
                    }
                }
            }
            labels[i] = rnd;
        }

        // Determine solution
        uint8_t l = pgm_read_byte_near(pgm_onfirst_dplabel + display);
        
        for(uint8_t i=0; i<ONFIRST_TWIDTH; i++) {
            for(uint8_t j=0; j<ONFIRST_NBUTTON; j++) {
                if(labels[j] == pgm_read_byte_near(pgm_onfirst_lbtable + ONFIRST_TWIDTH*labels[l] + i)) {
                    solution = j;
                    return;
                }
            }
        }
    }

public:
    MOnFirst() {
        slotID = 4;
    }

    void menu()
    {
        if(inputClicked(BTN_KEYPAD+2) || inputClicked(BTN_KEYPAD+5)) {
			toggleModule(ONFIRST_ID);
			menu_act = -1;
		}
    }

    void reset()
    {   
        stage = 0;

        resetStage();
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {
        // Handle buttons
        char in = inputClicked(BTN_KEYPAD, ONFIRST_NBUTTON);

        if(in > -1) {
            if(in == solution) {
                stage++;
                if(stage >= ONFIRST_NSTAGE) {
                    updateLeds();
                    defused();
                    return;
                }
                else click();
            }
            else add_strike();

            resetStage();
        }
        
        // Update display
        if(act) {
            dpKpd.clearDisplay();
            dpKpd.setTextColor(WHITE);
            dpKpd.setTextSize(2);

            dpKpd.setCursor(0, 0);
            readDisplay(display);
            dpKpd.print(buffer);

            dpKpd.setTextSize(1);
            for(uint8_t i=0; i<ONFIRST_NBUTTON; i++) {
                dpKpd.setCursor(i<3 ? 0:80, 25 + (i%3)*16);
                readLabel(labels[i]);
                dpKpd.print(buffer);
            }

            dpKpd.drawFastHLine(0, 19, 128, WHITE);
            dpKpd.drawFastVLine(64, 19, 45, WHITE);
            dpKpd.display();

            updateLeds();
            act = 0;
        }
    }
};