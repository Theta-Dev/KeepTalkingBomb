#pragma once
#include <Arduino.h>
#include "Button.h"

#define N_BUTTON 42

Button buttons[N_BUTTON] = {
    Button(18),  //00 BUTTON
    Button(23),  //01 PIN-CODE 0
    Button(24),  //02 PIN-CODE 1
    Button(25),  //03 PIN-CODE 2
    Button(26),  //04 PIN-CODE 3
    Button(27),  //05 PIN-CODE 4
    Button(28),  //06 PIN-CODE 5
    Button(29),  //07 PIN-CODE 6
    Button(30),  //08 PIN-CODE 7
    Button(31),  //09 PIN-CODE 8
    Button(32),  //10 PIN-CODE 9
    Button(33),  //11 PASSWORD ok
    Button(34),  //12 PASSWORD 1
    Button(35),  //13 PASSWORD 2
    Button(36),  //14 PASSWORD 3
    Button(37),  //15 PASSWORD 4
    Button(38),  //16 PASSWORD 5
    Button(39),  //17 PASSWORD 6
    Button(40),  //18 PASSWORD 7
    Button(41),  //19 PASSWORD 8
    Button(42),  //20 PASSWORD 9
    Button(43),  //21 PASSWORD 10
    Button(44),  //22 MAZE 1
    Button(45),  //23 MAZE 2
    Button(46),  //24 MAZE 3
    Button(47),  //25 MAZE 4
    Button(A1),  //26 WIRE 1
    Button(A2),  //27 WIRE 2
    Button(A3),  //28 WIRE 3
    Button(A4),  //29 WIRE 4
    Button(A5),  //30 WIRE 5
    Button(A6),  //31 WIRE 6
    Button(A7),  //32 KEYPAD 1
    Button(A8),  //33 KEYPAD 2
    Button(A9),  //34 KEYPAD 3
    Button(A10),  //35 KEYPAD 4
    Button(A11), //36 KEYPAD 5
    Button(A12), //37 KEYPAD 6
    Button(6),   //38 SIMON 1
    Button(7),   //39 SIMON 2
    Button(8),   //40 SIMON 3
    Button(9)    //41 SIMON 4
};

void inputUpdate()
{
    for (uint8_t i = 0; i < N_BUTTON; i++)
        buttons[i].update();
}

bool inputClicked(uint8_t i)
{
    return buttons[i].clicked();
}

int inputClicked(uint8_t i1, uint8_t n)
{
    for (uint8_t i=0; i<n; i++)
        if (buttons[i1+i].clicked()) return i;
    return -1;
}

bool inputPressed(uint8_t i)
{
    return buttons[i].pressed();
}