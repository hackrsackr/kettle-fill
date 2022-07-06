#pragma once
#include "Arduino.h"

#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

class KettleFiller
{
public:
    std::string name;
    float desired_liters;                           // Setpoint in Liters
    float liters;                                   // Volume in Liters
    float percent_full;                             // Actual_liters / Desired_liters
    int v_position;                                 // PropValve->position
    bool kf_enabled;                                // Enabled state
    int drain_valve_state;
    int return_valve_state;
    //int returnDrain_valve_state;
    //int aux_drain_valve_state; 
    //int input_valve_state;

    VolumeSensor* vs_ptr;                           // VolumeSensor Pointer 
    //PropValve* pv_ptr;                              //PropValve Pointer
    
    KettleFiller();
    KettleFiller(std::string, float);

    void set_kf_enabled(bool en) {kf_enabled = en;}
    void set_desired_liters(float dl) {desired_liters = dl;}
    int get_percent_full();
    int get_pv_position();
};

KettleFiller::KettleFiller(std::string nm, float dl) {
    set_kf_enabled(true);
    name = nm;
    desired_liters = dl;
}

int KettleFiller::get_percent_full() {
    percent_full = liters / desired_liters * 100;
    return percent_full;
}

int KettleFiller::get_pv_position()
{
    v_position = ((100 - percent_full) / 100) * 255;
    return v_position;
}
