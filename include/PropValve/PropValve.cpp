#include <iostream>

#include <Arduino.h>

#include "PropValve/PropValve.hpp"

uint8_t VALVE_PIN = 4;
uint8_t FEEDBACK_PIN = 1;

PropValve pv(VALVE_PIN, FEEDBACK_PIN);

void setup(void)
{
  Serial.begin(115200);
  pinMode(pv.pin, OUTPUT);
  pv.set_position(0);
}

void loop(void)
{
  std::cout << pv.position << std::endl;
  if (pv.position < 255)
  {
    pv.set_position(pv.position + 10);
  }
  else
  {
    pv.set_position(pv.position = 0);
  }
  std::cout << pv.position << std::endl;
  delay(1000);
}

