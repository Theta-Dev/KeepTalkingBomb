#pragma once
#include <Arduino.h>
#include "Button.h"

#define N_BUTTON 44

Button buttons[N_BUTTON] = {
    Button(A13),  // 00 TIMER 1
    Button(A14),  // 01 BUTTON
    Button(26),   // 02 PIN-CODE 1
    Button(27),   // 03 PIN-CODE 2
    Button(28),   // 04 PIN-CODE 3
    Button(29),   // 05 PIN-CODE 4
    Button(30),   // 06 PIN-CODE 5
    Button(31),   // 07 PIN-CODE 6
    Button(32),   // 08 PIN-CODE 7
    Button(33),   // 09 PIN-CODE 8
    Button(34),   // 10 PIN-CODE 9
    Button(35),   // 11 PIN-CODE 0
    Button(36),   // 12 PIN-CODE OK
    Button(37),   // 13 PASSWORD 1
    Button(38),   // 14 PASSWORD 2
    Button(39),   // 15 PASSWORD 3
    Button(40),   // 16 PASSWORD 4
    Button(41),   // 17 PASSWORD 5
    Button(42),   // 18 PASSWORD 6
    Button(43),   // 19 PASSWORD 7
    Button(44),   // 20 PASSWORD 8
    Button(45),   // 21 PASSWORD 9
    Button(46),   // 22 PASSWORD 10
    Button(47),   // 23 PASSWORD OK
    Button(A1),   // 24 WIRE 1
    Button(A2),   // 25 WIRE 2
    Button(A3),   // 26 WIRE 3
    Button(A4),   // 27 WIRE 4
    Button(A5),   // 28 WIRE 5
    Button(A6),   // 39 WIRE 6
    Button(A7),   // 30 KEYPAD 1
    Button(A8),   // 31 KEYPAD 2
    Button(A9),   // 32 KEYPAD 3
    Button(A10),  // 33 KEYPAD 4
    Button(A11),  // 34 KEYPAD 5
    Button(A12),  // 35 KEYPAD 6
    Button(12),   // 36 MAZE 1
    Button(13),   // 37 MAZE 2
    Button(14),   // 38 MAZE 3
    Button(15),   // 49 MAZE 4
    Button(16),   // 40 SIMON 1
    Button(17),   // 41 SIMON 2
    Button(18),   // 42 SIMON 3
    Button(19)    // 43 SIMON 4
};

#define BTN_TIMER 0
#define BTN_BUTTON 1
#define BTN_PIN 2
#define BTN_PIN_OK 12
#define BTN_PWD_UP 13
#define BTN_PWD_DOWN 18
#define BTN_PWD_OK 23
#define BTN_WIRE 24
#define BTN_KEYPAD 30
#define BTN_MAZE 36
#define BTN_SIMON 40


void inputUpdate()
{
    for (uint8_t i = 0; i < N_BUTTON; i++)
        buttons[i].update();
}

void inputReset()
{
    for (uint8_t i = 0; i < N_BUTTON; i++)
        buttons[i].clicked();
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