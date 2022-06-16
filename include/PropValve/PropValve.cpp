#include <arduino.h>

#include "PropValve.hpp"

uint8_t VALVE_PIN = 4;
uint8_t FEEDBACK_PIN = 1;

PropValve pv(VALVE_PIN, FEEDBACK_PIN);

void setup(void)
{
  Serial.begin(115200);
  pinMode(pv.pin, OUTPUT);
  pv.read_feedback();
  pv.set_position(pv.position);
}

void loop(void)
{
}
