#include <iostream>

#include <Arduino.h>

#include "PropValve/PropValve.hpp"
#include "config.h"

int VALVE_PIN = 4;
uint8_t FEEDBACK_PIN = 1;

PropValve pv(_PVNM1, _VPIN1, _FPIN1);

PropValve::PropValve(String nm, int vp, int fp) {
  this->name = nm;
  this->pin = vp;
  this->feedback_pin = fp;
  pinMode(this->pin, OUTPUT);
}

void PropValve::begin(int vp, int fp) {
  this->pin = vp;
  this->feedback_pin = fp;
}

void PropValve::set_position(int pos)
{
  this->position = pos;
  //digitalWrite(this->pin, pos);
  analogWrite(this->pin, pos);
}

int PropValve::get_position()
{
  return this->position;
}

int PropValve::read_feedback()
{
  this->feedback = analogReadMilliVolts(feedback_pin) / 1000;
  return this->feedback;
}

void setup(void)
{
  Serial.begin(115200);
  //pinMode(pv.pin, OUTPUT);
  //pv.set_position(0);
}

void loop(void)
{
  pv.set_position(0);
  std::cout << pv.position << std::endl;
  delay(5000);
  pv.set_position(128);
  std::cout << pv.position << std::endl;
  delay(5000);
  pv.set_position(255);
  std::cout << pv.position << std::endl;
  delay(5000);
  pv.set_position(128);
  std::cout << pv.position << std::endl;
  delay(5000);
  pv.set_position(0);
  std::cout << pv.position << std::endl;
  delay(5000);
}

