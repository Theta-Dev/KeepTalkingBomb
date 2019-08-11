#pragma once

#define MENU_BUTTON_UP 12
#define MENU_BUTTON_DOWN 17
#define MENU_BUTTON_OK 11
#define MENU_NBUTTON 10

#define MENU_NOPTIONS 4
#define MENU_TIMEINC 30000
#define MENU_TIMEMAX 1800000
#define MENU_TIMEMIN 30000

const char PROGMEM pgm_bomb_types[NBOMBTYPES] = {'E', 'M', 'H', 'C'};
const uint8_t PROGMEM pgm_bomb_types_n[3] = {3, 5, 7};

uint8_t menuState;
uint8_t module_pwd;
char menu_act;

void menuReset()
{
    menuState = 0;
    module_pwd = 0;
    menu_act = 5;
}

bool menuMain()
{
    if(menuState == 0)
    {
        // Update display
        if(menu_act > -2)
        {
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
            if(bombType == BOMBCUSTOM) {
                if(module_pwd) dpPwd.fillCircle(87, 21, 11, WHITE);
                else dpPwd.drawCircle(87, 21, 11, WHITE);
                dpPwd.setCursor(79, 18);
                dpPwd.setTextColor(module_pwd ? BLACK : WHITE);
                if(module_pwd == 1) dpPwd.print("Pwd");
                else if(module_pwd == 2) dpPwd.print("Mor");
            }

            dpPwd.setCursor(0, 45);
            dpPwd.setTextColor(WHITE);

            switch (menu_act)
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
                for(uint8_t i = 0; i < 4; i++) {
                    dpPwd.print(timerDigits[i]);
                    if (i == 1) dpPwd.print(':');
                }
                break;
            case 2:
                dpPwd.print("Hardcore: ");
                dpPwd.print(hcMode ? "ON" : "OFF");
                break;
            case 3:
                if(bombType == BOMBCUSTOM) {
                    if(module_pwd == 1) dpPwd.print("Password");
                    else if(module_pwd == 2) dpPwd.print("Morse");
                    else dpPwd.print("Module off");
                }
                break;
            case 5:
                dpPwd.print("Welcome to KTANE");
                break;
            case 6:
                dpPwd.print("No modules activated");
                break;
            default:
                break;
            }
            dpPwd.display();

            dpKpd.clearDisplay();
            if(bombType == BOMBCUSTOM)
            {
                dpKpd.setTextSize(2);
                dpKpd.setCursor(15,3);
                dpKpd.setTextColor(modules[KEYPAD_ID]->state == 0, modules[KEYPAD_ID]->state == 1);
                dpKpd.print("Keypad");
                dpKpd.setCursor(15,24);
                dpKpd.setTextColor(modules[MEMORY_ID]->state == 0, modules[MEMORY_ID]->state == 1);
                dpKpd.print("Memory");
                dpKpd.setCursor(15,45);
                dpKpd.setTextColor(modules[ONFIRST_ID]->state == 0, modules[ONFIRST_ID]->state == 1);
                dpKpd.print("OnFirst");
            }
            dpKpd.display();

            menu_act = -2;
        }
        
        // Handle buttons
        char inc = 1;
        int in = inputClicked(MENU_BUTTON_UP, MENU_NOPTIONS);
        if(in == -1)
        {
            inc = -1;
            in = inputClicked(MENU_BUTTON_DOWN, MENU_NOPTIONS);
        }
        if(in > -1) {
            if(menu_act == -2) menu_act = in;
            click();
        }

        switch (in)
        {
        case 0: // Bomb type
            bombType += inc;
            if(bombType >= NBOMBTYPES)
                bombType = inc > 0 ? 0 : NBOMBTYPES - 1;
            break;
        case 1: // Time
            setTime += (MENU_TIMEINC * inc);
            if(setTime > MENU_TIMEMAX || setTime < MENU_TIMEMIN)
                setTime = inc > 0 ? MENU_TIMEMAX : MENU_TIMEMIN;
            break;
        case 2: // Hardcore mode
            hcMode = !hcMode;
            break;
        case 3: // Password/Morse
            if(bombType == BOMBCUSTOM) module_pwd += inc;
            if(module_pwd > 2) module_pwd = inc>0 ? 0:2;

            setModule(PWD_ID, module_pwd==1);
            setModule(MORSE_ID, module_pwd==2);
            break;
        default:
            break;
        }

        if(inputClicked(MENU_BUTTON_OK))
        {
            if(bombType == BOMBCUSTOM) {
                uint8_t c=0;
                for(uint8_t i=1; i<N_MODULE; i++) if(modules[i]->state == 1) c++;
                if(c > 0) {
                    menuState = 1;
                    menu_act = -1;
                }
                else menu_act = 6;
            }
            else {
                menuState = 1;
                menu_act = -1;
            }
            click();
        }
    }
    else if(menuState == 1)
    {
        if(menu_act > -2)
        {
            dpPwd.clearDisplay();
            dpPwd.setTextColor(WHITE);
            dpPwd.setCursor(0, 0);
            dpPwd.setTextSize(2);
            dpPwd.print("Ready?");

            dpPwd.setCursor(0, 30);
            dpPwd.setTextSize(1);
            dpPwd.println("Let the games begin!");
            dpPwd.println("Press the red button to start");

            dpPwd.display();
            menu_act = -2;
        }

        if(inputClicked(MENU_BUTTON_UP, MENU_NBUTTON) > -1)
        {
            menuState = 0;
            menu_act = -1;
            click();
        }
        if(inputClicked(MENU_BUTTON_OK)) {
            menuState = 2;
            click();
        }
    }
    return menuState == 2;
}