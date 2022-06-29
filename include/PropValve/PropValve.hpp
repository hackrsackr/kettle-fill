#pragma once

class PropValve
{
public:
    String name;
    int pin;              // Esp32 pin of the proportional valve
    int position;           //
    int feedback_pin;           // Esp32 pin of the prop valve feedback signal
    float feedback;                 // Volts from feedback pin

    PropValve();
    PropValve(String, int, int);
    void begin(int, int);
    void set_position(int);
    int get_position();
    int read_feedback();
};


