#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
#include <ESP32HTTPUpdateServer.h>
#include <EspMQTTClient.h>

#include "KettleFiller.hpp"
#include "config.h"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"

PropValve pv(_PVNM1, _VPIN1, _FPIN1);            // name, valve_pin, feedback_pin
VolumeSensor vs(_VSNM1, _CHAN1, _OFFS1);         // name, ads_channel, adc_offset
KettleFiller kf(_KFNM1, _SETL1);                 // name, desired_liters

KettleFiller::KettleFiller(String nm, float dl) {
    this->set_kf_enabled(true);
    this->name = nm;
    this->desired_liters = dl;
};

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


//void KettleFiller::print_data() {
//  StaticJsonDocument<256> message;
//  message["key"] = _CLIENTID;

//  message["data"][this->name]["enabled"] = this->kf_enabled;
//  message["data"][this->name]["des_ls"] = this->desired_liters;
//  message["data"][this->name]["kf_liters"] = this->get_liters();
  //message["data"][this->name]["adc"] = vs.read_adc();
  //message["data"][this->name]["trim_adc"] = vs.trim_adc();
  //message["data"][this->name]["volts"] = vs.read_volts();
  //message["data"][this->name]["kf_liters"] = vs.read_liters();
  //message["data"][this->name]["filled"] = this->.get_percent_full(vs.liters);
  //message["data"][this->name]["kf-pos"] = this->.get_pv_position(vs.liters);
  //message["data"][this->name]["pv-pos"] = pv.position;
  
//  serializeJsonPretty(message, Serial);
//}

//void KettleFiller::run() {
//  this->get_liters();
  //vs.read_adc();
  //vs.trim_adc();
  //vs.read_volts();
  //this->.get_percent_full(this->liters);
  //this->.get_pv_position(this->.liters);
  //pv.set_position(kf.v_position);

//}