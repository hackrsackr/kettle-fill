#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
#include <ESP32HTTPUpdateServer.h>
#include <EspMQTTClient.h>

#include "KettleFiller/KettleFiller.hpp"
#include "config.h"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"

PropValve pv(_PVNM1, _VPIN1, _FPIN1);            // name, valve_pin, feedback_pin
VolumeSensor vs(_VSNM1, _CHAN1, _OFFS1);         // name, ads_channel, adc_offset
KettleFiller kf(_KFNM1, _SETL1);                 // name, desired_liters

KettleFiller::KettleFiller(std::string nm, float dl) {
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

void KettleFiller::begin(std::string nm, float dl)  {
    this->set_kf_enabled(true);
    this->name = nm;
    this->desired_liters = dl;

}

JsonObject KettleFiller::print_data() {
  JsonObject data;
  //message["key"] = _CLIENTID;

  data[kf.name]["enabled"] = kf.kf_enabled;
  data[kf.name]["adc"] = vs.read_adc();
  data[kf.name]["trim_adc"] = vs.trim_adc();
  data[kf.name]["volts"] = vs.read_volts();
  data[kf.name]["des_ls"] = kf.desired_liters;
  data[kf.name]["kf_liters"] = vs.read_liters();
  data[kf.name]["filled"] = kf.get_percent_full(vs.liters);
  data[kf.name]["kf-pos"] = kf.get_pv_position(vs.liters);
  data[kf.name]["pv-pos"] = pv.position;
  
  serializeJsonPretty(data, Serial);
  return data;
}

void KettleFiller::run() {
  vs.read_adc();
  vs.trim_adc();
  vs.read_volts();
  vs.read_liters();
  kf.get_percent_full(vs.liters);
  kf.get_pv_position(vs.liters);
  pv.set_position(kf.v_position);

}

