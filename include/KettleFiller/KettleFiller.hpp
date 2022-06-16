#pragma once

#include "KettleFiller_config.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "PropValve/PropValve.hpp"

class KettleFiller
{
public:
    String name;
    float desired_liters;           // Setpoint in Liters
    float liters;                   // Volume in Liters
    uint8_t percent_full;           // Actual_liters / Desired_liters

    KettleFiller();
    KettleFiller(String, uint8_t, uint8_t, uint8_t, uint16_t, float);

    float read_liters();
    void change_desired_liters(float);
    uint8_t get_percent_full();
    uint8_t determine_valve_position();
    float read_valve_feedback();

    PropValve *valve;
    VolumeSensor *sensor;
};

KettleFiller::KettleFiller(String nm, uint8_t vp, uint8_t fp, uint8_t ch, uint16_t offset, float dl)
{
    name = nm;
    valve->pin = vp;
    valve->feedback_pin = fp;
    sensor->ads_channel = ch;
    sensor->adc_offset = offset;
    desired_liters = dl;
}

float KettleFiller::read_liters()
{
    uint8_t l = (sensor->read_volts() - _ZEROVOLTS) * (_MAXLITERS / (_MAXVOLTS - _ZEROVOLTS));
    return liters = (l < 0) ? 0 : l;
}

void KettleFiller::change_desired_liters(float dl) {desired_liters = dl;}

uint8_t KettleFiller::get_percent_full()
{
    percent_full = uint8_t((read_liters() / desired_liters) * 100);
    return percent_full;
}

uint8_t KettleFiller::determine_valve_position()
{
    valve->position = uint8_t((100 - get_percent_full() / 100) * 255);
}

float KettleFiller::read_valve_feedback()
{
    return valve->read_feedback();
}