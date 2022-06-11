#pragma once

class PropValve
{
public:
    uint8_t pin;              // Esp32 pin of the proportional valve
    uint8_t position;           //
    uint8_t feedback_pin;           // Esp32 pin of the prop valve feedback signal
    uint8_t feedback;                 // Volts from feedback pin

    PropValve();
    PropValve(uint8_t, uint8_t);
    void set_position(uint8_t);
    uint8_t read_feedback();
};

PropValve::PropValve(uint8_t vp, uint8_t fp)
{
    pin = vp;
    feedback_pin = fp;
}

void PropValve::set_position(uint8_t pos)
{
    pos = position; 
    digitalWrite(pin, position);
}

uint8_t PropValve::read_feedback()
{
    feedback = analogReadMilliVolts(feedback_pin) / 1000;
    return feedback;
}