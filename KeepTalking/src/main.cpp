#include <Arduino.h>

#include "Game.h"

void setup()
{
  gameBegin();
  gameReset();
  gameSetup();
  delay(1000);
  gameRun();
}

void loop()
{
}
