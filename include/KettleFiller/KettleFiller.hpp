#pragma once
#include "Arduino.h"

#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

class KettleFiller
{
public:
    std::string name;
    float desired_liters;           // Setpoint in Liters
    float liters;                   // Volume in Liters
    float liqr_liters;              // Setpoint in Liters
    float mash_liters;              // Setpoint in Liters
    float boil_liters;              // Setpoint in Liters
    float percent_full;             // Actual_liters / Desired_liters
    int v_position;
    bool kf_enabled;

    VolumeSensor* vs_ptr;
    PropValve* pv_ptr;
    KettleFiller();
    KettleFiller(std::string, float);

    void set_kf_enabled(bool en) {kf_enabled = en;}
    void set_desired_liters(float dl) {desired_liters = dl;}
    int get_percent_full(float);
    int get_pv_position(float);
    void begin(std::string, float);
    //void print_data();
    void run();
    float get_liters();
};

KettleFiller::KettleFiller(std::string nm, float dl) {
    set_kf_enabled(true);
    name = nm;
    desired_liters = dl;
}

int KettleFiller::get_percent_full(float liters) {
    percent_full = liters / desired_liters * 100;
    return percent_full;
}

int KettleFiller::get_pv_position(float liters)
{
    v_position = ((100 - percent_full) / 100) * 255;
    return v_position;
}

void KettleFiller::begin(std::string nm, float dl)  {
    set_kf_enabled(true);
    name = nm;
    desired_liters = dl;

}