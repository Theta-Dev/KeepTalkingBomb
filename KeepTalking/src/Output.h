#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "..\lib\LedControl\src\LedControl.h"
#include "..\lib\Adafruit_NeoPixel\Adafruit_NeoPixel.h"
#include "..\lib\Adafruit-GFX-Library\Adafruit_GFX.h"
#include "..\lib\Adafruit_SSD1306\Adafruit_SSD1306.h"

#define N_MAX 3
#define MAX_LEDS 0
#define MAX_MATRIX 1
#define MAX_7SEG 2
LedControl max = LedControl(22, 24, 23, N_MAX);

#define N_PIXEL 21
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(N_PIXEL, 25, NEO_GRB + NEO_KHZ800);

Adafruit_SSD1306 dpPwd = Adafruit_SSD1306(128, 64, &Wire, -1);
Adafruit_SSD1306 dpKpd = Adafruit_SSD1306(128, 64, &Wire, -1);

#define N_COLOR 7
#define COLOR_WHITE  1
#define COLOR_RED    2
#define COLOR_GREEN  3
#define COLOR_BLUE   4
#define COLOR_YELLOW 5
#define COLOR_PURPLE 6

const uint8_t colors[] = {
    0, 0, 0,        // Off
    85, 85, 85,     // White
    255, 0, 0,      // Red
    0, 255, 0,      // Green
    0, 0, 255,      // Blue
    128, 128, 0,    // Yellow
    128, 0, 128     // Purple
};

const uint8_t statusPixel[] = {0, 0, 1, 2, 15, 16, 17, 18};

#define LED_KEYPAD_R 4
#define LED_KEYPAD_C 0
#define LED_PIN_R 4
#define LED_PIN_C 4
#define LED_PINX_R 7
#define LED_PINX_C 4
#define LED_WIRE_R 5
#define LED_WIRE_C 0
#define LED_WIRESTAR_R 6
#define LED_WIRESTAR_C 0
#define LED_STRIKE_R 5
#define LED_STRIKE_C 6
#define LED_SIMON_R 7
#define LED_SIMON_C 0
#define LED_BAT1_R 6
#define LED_BAT1_C 6
#define LED_BAT2_R 6
#define LED_BAT2_C 7
#define LED_BAT3_R 7
#define LED_BAT3_C 7

#define RGB_WIRE 3
#define RGB_BUTTON 19

#define PIN_BUZZER 2
#define PIN_VIBRATOR 7
#define PIN_MORSE 8
#define PIN_LED_OUT 3
#define N_LED_OUT 4
#define PIN_LED_BUTTON 9
#define N_LED_BUTTON 3

void outputUpdate()
{
    pixel.show();

    dpPwd.display();
    dpKpd.display();
}

void outputReset()
{
    dpPwd.clearDisplay();
    dpKpd.clearDisplay();

    for(int i=0; i<N_MAX; i++) max.clearDisplay(i);

    pixel.clear();

    for(int i=0; i<N_LED_BUTTON; i++) digitalWrite(i, 0);
    for(int i=0; i<N_LED_OUT; i++) digitalWrite(i, 0);
    digitalWrite(PIN_BUZZER, 0);
    digitalWrite(PIN_VIBRATOR, 0);
    digitalWrite(PIN_MORSE, 0);

    outputUpdate();
}

void outputBegin()
{
    // MAX setup
    for (int i = 0; i < N_MAX; i++) {
        max.setIntensity(i, 10);
        max.clearDisplay(i);
        max.shutdown(i, false);
    }

    // Pixel setup
    pixel.begin();
    pixel.setBrightness(50);

    // Display setup
    dpPwd.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    dpKpd.begin(SSD1306_SWITCHCAPVCC, 0x3D);

    // Dout setup
    for(int i=0; i<N_LED_BUTTON; i++) pinMode(PIN_LED_BUTTON+i, OUTPUT);
    for(int i=0; i<N_LED_OUT; i++) pinMode(N_LED_OUT+i, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_VIBRATOR, OUTPUT);
    pinMode(PIN_MORSE, OUTPUT);

    outputReset();
}

void click() {
    tone(PIN_BUZZER, 1000, 10);
}