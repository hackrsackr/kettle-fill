#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "KettleFiller/KettleFiller.hpp"
#include "KettleFiller/KettleFiller_config.hpp"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"

PropValve pv(_PVID1, _VPIN1, _FPIN1);            // name, valve_pin, feedback_pin
VolumeSensor vs(_VSNM1, _CHAN1, _OFFS1);         // name, ads_channel, adc_offset
KettleFiller kf(_KFNM1, _SETL1);                 // name, desired_liters

void setup(void)
{
  Serial.begin(115200);

  ads.begin(0x48);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop(void)
{
  kf.run();
}

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

void KettleFiller::print_data() {
  std::cout << "setpoint: " << kf.desired_liters << std::endl;
  std::cout << "liters: " << vs.read_liters() << std::endl;
  std::cout << "kf.pf: " << kf.get_percent_full(vs.liters) << std::endl;
  std::cout << "kf.pos: " << kf.get_pv_position(vs.liters) << std::endl;
  pv.set_position(kf.v_position);
  std::cout << "valve_position: " << pv.get_position() << std::endl;

}

void KettleFiller::run() {
  StaticJsonDocument<1024> message;

  message["key"] = _CLIENTID;

  message["data"][kf.name]["enabled"] = kf.kf_enabled;
  message["data"][kf.name]["adc"] = vs.read_adc();
  message["data"][kf.name]["trim_adc"] = vs.trim_adc();
  message["data"][kf.name]["volts"] = vs.read_volts();
  message["data"][kf.name]["des_ls"] = kf.desired_liters;
  message["data"][kf.name]["liters"] = vs.read_liters();
  message["data"][kf.name]["filled"] = kf.get_percent_full(vs.liters);
  message["data"][kf.name]["kf-pos"] = kf.get_pv_position(vs.liters);
  pv.set_position(kf.v_position);
  message["data"][kf.name]["pv-pos"] = pv.position;
  message["data"][kf.name]["pv-fb"] = pv.read_feedback();

  serializeJsonPretty(message, Serial);

  delay(5000);
}