#pragma once

class PropValve
{
public:
    std::string name;
    int pin;              // Esp32 pin of the proportional valve
    int position;           //
    int feedback_pin;           // Esp32 pin of the prop valve feedback signal
    float feedback;                 // Volts from feedback pin

    PropValve();
    PropValve(std::string, int, int);
    void begin(int, int);
    void set_position(int);
    int get_position();
    int read_feedback();
};

PropValve::PropValve(std::string nm, int vp, int fp) {
  this->name = nm;
  this->pin = vp;
  this->feedback_pin = fp;
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
