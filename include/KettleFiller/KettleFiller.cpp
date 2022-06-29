#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
#include <ESP32HTTPUpdateServer.h>
#include <EspMQTTClient.h>

#include "KettleFiller.hpp"
#include "config.h"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"

//PropValve pv(_PVNM1, _VPIN1, _FPIN1);            // name, valve_pin, feedback_pin
//VolumeSensor vs(_VSNM1, _CHAN1, _OFFS1);         // name, ads_channel, adc_offset
//KettleFiller kf(_KFNM1, _SETL1);                 // name, desired_liters

KettleFiller::KettleFiller(String nm, float dl) {
    this->set_kf_enabled(true);
    this->name = nm;
    this->desired_liters = dl;
};

//KettleFiller::KettleFiller(String nm, float dl, VolumeSensor vs, PropValve pv) {
//    this->set_kf_enabled(true);
//    this->name = nm;
//    this->desired_liters = dl;
///   this->pv_ptr = &pv;
//    this->vs_ptr = &vs;
//};

int KettleFiller::get_percent_full(float liters) {
    this->percent_full = liters / this->desired_liters * 100;
    return this->percent_full;
}

int KettleFiller::get_pv_position(float liters)
{
    this->v_position = ((100 - this->percent_full) / 100) * 255;
    return this->v_position;
}

void KettleFiller::begin(String nm, float dl)  {
    this->set_kf_enabled(true);
    this->name = nm;
    this->desired_liters = dl;

}