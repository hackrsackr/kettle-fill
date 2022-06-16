#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "KettleFiller/KettleFiller.hpp"
#include "KettleFiller/KettleFiller_config.hpp"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"

PropValve pv(4, 1);
VolumeSensor vs(0, 8000);
KettleFiller kf;

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  kf.set_kf_enabled(false);
  kf.set_desired_liters(25.50);
  kf.name = "liqr";
}

void loop(void)
{  
  //std::cout << "setpoint: " << kf.desired_liters << std::endl;
  //std::cout << "liters: " << vs.read_liters() << std::endl;
  //std::cout << "kf.pf: " << kf.get_percent_full(vs.liters) << std::endl;
  //std::cout << "kf.pos: " << kf.get_pv_position(vs.liters) << std::endl;
  //pv.set_position(kf.v_position);
  //std::cout << "valve_position: " << pv.get_position() << std::endl;

  StaticJsonDocument<1024> message;

  message["key"] = _CLIENTID;
  
  message["data"][kf.name]["enabled"] = kf.kf_enabled;
  message["data"][kf.name]["des_ls"] = kf.desired_liters;
  message["data"][kf.name]["liters"] = vs.read_liters();
  message["data"][kf.name]["filled"] = kf.get_percent_full(vs.liters);
  message["data"][kf.name]["va-pos"] = kf.get_pv_position(vs.liters);
  pv.set_position(kf.v_position);
  message["data"][kf.name]["out_v"]  = pv.position; 
  
  serializeJsonPretty(message, Serial);

  delay(5000);
}