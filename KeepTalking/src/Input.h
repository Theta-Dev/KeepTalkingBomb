#pragma once
#include <Arduino.h>
#include "Button.h"

#define N_BUTTON 46

Button buttons[N_BUTTON] = {
    Button(A13),  // 00 TIMER 1
    Button(A14),  // 01 TIMER 2
    Button(25),   // 02 PIN-CODE 0
    Button(26),   // 03 PIN-CODE 1
    Button(27),   // 04 PIN-CODE 2
    Button(28),   // 05 PIN-CODE 3
    Button(29),   // 06 PIN-CODE 4
    Button(30),   // 07 PIN-CODE 5
    Button(31),   // 08 PIN-CODE 6
    Button(32),   // 09 PIN-CODE 7
    Button(33),   // 10 PIN-CODE 8
    Button(34),   // 11 PIN-CODE 9
    Button(35),   // 12 PIN-CODE OK
    Button(36),   // 13 PIN-CODE CLR
    Button(37),   // 14 PASSWORD 1
    Button(38),   // 15 PASSWORD 2
    Button(39),   // 16 PASSWORD 3
    Button(40),   // 17 PASSWORD 4
    Button(41),   // 18 PASSWORD 5
    Button(42),   // 19 PASSWORD 6
    Button(43),   // 20 PASSWORD 7
    Button(44),   // 21 PASSWORD 8
    Button(45),   // 22 PASSWORD 9
    Button(46),   // 23 PASSWORD 10
    Button(47),   // 24 PASSWORD OK
    Button(A1),   // 25 WIRE 1
    Button(A2),   // 26 WIRE 2
    Button(A3),   // 27 WIRE 3
    Button(A4),   // 28 WIRE 4
    Button(A5),   // 29 WIRE 5
    Button(A6),   // 30 WIRE 6
    Button(A7),   // 31 KEYPAD 1
    Button(A8),   // 32 KEYPAD 2
    Button(A9),   // 33 KEYPAD 3
    Button(A10),  // 34 KEYPAD 4
    Button(A11),  // 35 KEYPAD 5
    Button(A12),  // 36 KEYPAD 6
    Button(15),   // 37 MAZE 1
    Button(16),   // 38 MAZE 2
    Button(17),   // 39 MAZE 3
    Button(18),   // 40 MAZE 4
    /* FINAL PINOUT
    Button(2),    // 41 SIMON 1
    Button(3),    // 42 SIMON 2*/
    Button(48),   // 41 SIMON 1
    Button(49),   // 42 SIMON 2
    Button(4),    // 43 SIMON 3
    Button(5),    // 44 SIMON 4
    Button(14)    // 45 BUTTON
};

#define BTN_TIMER 0
#define BTN_PIN 2
#define BTN_PIN_OK 12
#define BTN_PIN_CLR 13
#define BTN_PWD_UP 14
#define BTN_PWD_DOWN 19
#define BTN_PWD_OK 24
#define BTN_WIRE 25
#define BTN_KEYPAD 31
#define BTN_MAZE 37
#define BTN_SIMON 41
#define BTN_BUTTON 45


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