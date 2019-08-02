#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "..\lib\LedControl\src\LedControl.h"
#include "..\lib\Adafruit_NeoPixel\Adafruit_NeoPixel.h"
#include "..\lib\Adafruit-GFX-Library\Adafruit_GFX.h"
#include "..\lib\Adafruit_SSD1306\Adafruit_SSD1306.h"

#define N_MAX 3
#define N_PIXEL 21

LedControl max = LedControl(15, 17, 16, N_MAX);
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(N_PIXEL, 14, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 dpPwd = Adafruit_SSD1306(128, 64, &Wire, -1);
Adafruit_SSD1306 dpKpd = Adafruit_SSD1306(128, 64, &Wire, -1);

#define N_COLOR 6
#define COLOR_WHITE  1
#define COLOR_RED    2
#define COLOR_GREEN  3
#define COLOR_BLUE   4
#define COLOR_YELLOW 5

const uint8_t colors[] = {
    0, 0, 0,        // Off
    85, 85, 85,     // White
    255, 0, 0,      // Red
    0, 255, 0,      // Green
    0, 0, 255,      // Blue
    128, 128, 0     // Yellow
};

#define PIN_BUZZER 22
#define PIN_LED_MORSE 13
#define PIN_LED_BUTTON 10
#define N_LED 3


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

    for(int i=0; i<N_PIXEL; i++) pixel.setPixelColor(i, 0);

    for(int i=0; i<N_LED; i++) digitalWrite(i, 0);
    digitalWrite(PIN_BUZZER, 0);
    digitalWrite(PIN_LED_MORSE, 0);

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
    for(int i=0; i<N_LED; i++) pinMode(PIN_LED_BUTTON+i, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_MORSE, OUTPUT);

    outputReset();
}