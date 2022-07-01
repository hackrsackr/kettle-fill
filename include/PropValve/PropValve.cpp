#include <iostream>

#include <Arduino.h>

#include "PropValve/PropValve.hpp"
#include "config.h"

PropValve pv(_PVNM1, _VPIN1);

void setup(void)
{
  Serial.begin(115200);
  pinMode(pv.pin, OUTPUT);
  pv.set_position(255);
}

void loop(void)
{
}

