#pragma once

class PropValve
{
public:
    int pin;              // Esp32 pin of the proportional valve
    int position;           //
    int feedback_pin;           // Esp32 pin of the prop valve feedback signal
    float feedback;                 // Volts from feedback pin

    PropValve();
    PropValve(int, int);
    void set_position(int);
    int get_position();
    int read_feedback();
};

PropValve::PropValve(int vp, int fp)
{
  this->pin = vp;
  this->feedback_pin = fp;
}

void PropValve::set_position(int pos)
{
  digitalWrite(this->pin, pos);
}

int PropValve::get_position()
{
  return this->position;
}

int PropValve::read_feedback()
{
  return analogReadMilliVolts(feedback_pin) / 1000;
}