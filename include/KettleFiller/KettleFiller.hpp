#pragma once

#include "config.h"
#include "VolumeSensor/VolumeSensor.hpp"
#include "PropValve/PropValve.hpp"

class KettleFiller
{
public:
    String name;
    float desired_liters;           // Setpoint in Liters
    //float setpoint;           // Setpoint in Liters
    float liters;           // Volume in Liters
    float liqr_liters;           // Setpoint in Liters
    float mash_liters;           // Setpoint in Liters
    float boil_liters;           // Setpoint in Liters
    float percent_full;           // Actual_liters / Desired_liters
    int v_position;
    bool v_state;
    bool kf_enabled;

    KettleFiller();
    KettleFiller(String, float);

    void set_kf_enabled(bool en) {kf_enabled = en;}
    void set_desired_liters(float dl) {desired_liters = dl;}
    int get_percent_full(float);
    int get_pv_position(float);
    void begin(String, float);
    void print_data();
    void run();
    float get_liters();
};

