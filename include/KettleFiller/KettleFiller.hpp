#pragma once

#include "KettleFiller_config.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "PropValve/PropValve.hpp"

class KettleFiller
{
public:
    std::string name;
    float desired_liters;           // Setpoint in Liters
    float percent_full;           // Actual_liters / Desired_liters
    int v_position;
    bool kf_enabled;

    KettleFiller();

    void set_kf_enabled(bool en) {kf_enabled = en;}
    void set_desired_liters(float dl) {desired_liters = dl;}
    int get_percent_full(float);
    int get_pv_position(float);
};

KettleFiller::KettleFiller() {};


int KettleFiller::get_percent_full(float liters) {
    this->percent_full = liters / this->desired_liters * 100;
    return this->percent_full;
}

int KettleFiller::get_pv_position(float liters)
{
    this->v_position = ((100 - this->percent_full) / 100) * 255;
    return this->v_position;
}

