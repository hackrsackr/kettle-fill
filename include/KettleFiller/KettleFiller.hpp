#pragma once

#include "config.hpp"
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
    KettleFiller(std::string, float);

    void set_kf_enabled(bool en) {kf_enabled = en;}
    void set_desired_liters(float dl) {desired_liters = dl;}
    int get_percent_full(float);
    int get_pv_position(float);
    void begin(std::string, float);
    void print_data();
    void run();
};


