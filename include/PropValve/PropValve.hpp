#pragma once
#include <string>
#include "Arduino.h"

class PropValve
{
public:
  std::string name;
  int pin;                                    // Esp32 pin of the proportional valve
  int position;                               //
  bool pv_enabled;

  PropValve();
  PropValve(std::string, int);

  void set_position(int);
  void set_pv_enabled(bool);
};

PropValve::PropValve(std::string nm, int pn)
{
  name = nm;
  pin = pn;
  pinMode(this->pin, OUTPUT);
}

void PropValve::set_position(int pos)
{
  if (pv_enabled) {
  position = pos;
  analogWrite(pin, position);
  }
}

void PropValve::set_pv_enabled(bool pv_en)
{
  pv_enabled = pv_en;
}
