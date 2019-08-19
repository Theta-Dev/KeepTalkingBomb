#pragma once

#define MEMORY_NBUTTON 4
#define MEMORY_NSTAGE 5

const uint8_t memory_buttons[] = {BTN_KEYPAD, BTN_KEYPAD+2, BTN_KEYPAD+3, BTN_KEYPAD+5};

class MMemory : public Module
{
private:
    uint8_t numbers[MEMORY_NSTAGE];

    uint8_t labels[MEMORY_NBUTTON];
    uint8_t labelPositions[MEMORY_NBUTTON];

    uint8_t pressedButtons[MEMORY_NSTAGE];
    uint8_t pressedLabels[MEMORY_NSTAGE];

    uint8_t stage;
    bool act;

    void shuffleButtons() {
        for(uint8_t i=0; i<MEMORY_NBUTTON; i++) {
            uint8_t rnd = random(MEMORY_NBUTTON);

            uint8_t tmp = labels[i];
            labels[i] = labels[rnd];
            labels[rnd] = tmp;
        }

        for(uint8_t i=0; i<MEMORY_NBUTTON; i++) labelPositions[labels[i]] = i;
    }

    char readButtons()
	{
		for(uint8_t i=0; i<MEMORY_NBUTTON; i++) {
			if(inputClicked(memory_buttons[i])) return i;
		}
		return -1;
	}

public:
    MMemory() {
        slotID = 4;
    }

    void menu()
    {
        if(inputClicked(BTN_KEYPAD+1) || inputClicked(BTN_KEYPAD+4)) {
			toggleModule(MEMORY_ID);
			menu_act = -1;
		}
    }

    void reset()
    {
        act = 1;
        stage = 0;
        
        // Generate labels
        for(uint8_t i=0; i<MEMORY_NBUTTON; i++) labels[i] = i;
        shuffleButtons();
        
        // Generate random numbers
        for(uint8_t i=0; i<MEMORY_NSTAGE; i++) {
            numbers[i] = random(MEMORY_NBUTTON);
        }
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {
        if(act) {
            // Determine solution
            switch(stage)
            {
                case 0: // Stage 1
                    switch(numbers[stage]) {
                        case 0: pressedButtons[stage] = 1; break;
                        case 1: pressedButtons[stage] = 1; break;
                        case 2: pressedButtons[stage] = 2; break;
                        case 3: pressedButtons[stage] = 3; break;
                    }
                    break;
                case 1: // Stage 2
                    switch(numbers[stage]) {
                        case 0: pressedButtons[stage] = labelPositions[3]; break;
                        case 1: pressedButtons[stage] = pressedButtons[0]; break;
                        case 2: pressedButtons[stage] = 0; break;
                        case 3: pressedButtons[stage] = pressedButtons[0]; break;
                    }
                    break;
                case 2: // Stage 3
                    switch(numbers[stage]) {
                        case 0: pressedButtons[stage] = labelPositions[pressedLabels[1]]; break;
                        case 1: pressedButtons[stage] = labelPositions[pressedLabels[0]]; break;
                        case 2: pressedButtons[stage] = 2; break;
                        case 3: pressedButtons[stage] = labelPositions[3]; break;
                    }
                    break;
                case 3: // Stage 4
                    switch(numbers[stage]) {
                        case 0: pressedButtons[stage] = pressedButtons[0]; break;
                        case 1: pressedButtons[stage] = 0; break;
                        case 2: pressedButtons[stage] = pressedButtons[1]; break;
                        case 3: pressedButtons[stage] = pressedButtons[1]; break;
                    }
                    break;
                case 4: // Stage 5
                    switch(numbers[stage]) {
                        case 0: pressedButtons[stage] = labelPositions[pressedLabels[0]]; break;
                        case 1: pressedButtons[stage] = labelPositions[pressedLabels[1]]; break;
                        case 2: pressedButtons[stage] = labelPositions[pressedLabels[3]]; break;
                        case 3: pressedButtons[stage] = labelPositions[pressedLabels[2]]; break;
                    }
                    break;
            }
            pressedLabels[stage] = labels[pressedButtons[stage]];

            // Update display
            dpKpd.clearDisplay();
            dpKpd.setTextColor(WHITE);

            dpKpd.setTextSize(4);
            dpKpd.setCursor(54,18);
            dpKpd.print(numbers[stage]+1);

            dpKpd.setTextSize(2);
            
            for(uint8_t i=0; i<MEMORY_NBUTTON; i++) {
                dpKpd.drawRect(i<2 ? 0:106, i%2==0 ? 0:38, 22, 26, WHITE);
                dpKpd.setCursor(i<2 ? 6:112, i%2==0 ? 6:44);
                dpKpd.print(labels[i]+1);
            }

            dpKpd.setTextSize(1);
            dpKpd.setCursor(40,0);
            dpKpd.print("Stage: ");
            dpKpd.print(stage+1);
            /*
            // Debug info
            dpKpd.setCursor(40,55);
            dpKpd.setTextColor(WHITE);
            dpKpd.print(pressedButtons[stage]+1);
            dpKpd.print("; ");
            dpKpd.print(pressedLabels[stage]+1);*/
            
            dpKpd.display();

            for(uint8_t i=0; i<4; i++) max.setLed(MAX_LEDS, LED_KEYPAD_R, LED_KEYPAD_C+i, i<stage);

            act = 0;
        }

        // Handle button input
        char in = readButtons();

        if(in > -1)
        {
            if(in == pressedButtons[stage]) {
                stage++;
                if(stage >= MEMORY_NSTAGE) {
                    defused();
                    return;
                }
            }
            else {
                add_strike();
                reset();
                return;
            }
            
            shuffleButtons();
            act = 1;
            click();
        }
    }
};