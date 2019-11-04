#include <Arduino.h>

#include "Game.h"

void setup()
{
  gameBegin();
}

void loop()
{
  gameMenu();
  gameReset();
  if(gameSetup() == 2) return;
  gameFinal(gameRun());
}
