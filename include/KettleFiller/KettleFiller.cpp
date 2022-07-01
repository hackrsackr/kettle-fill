#include <iostream>
#include <ArduinoJson.h>

#include <array>
//#include "VolumeSensor.hpp"
#include "KettleFiller/KettleFiller.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr = {
    KettleFiller(_KFNM1, _SETL1),
    KettleFiller(_KFNM2, _SETL2),
    KettleFiller(_KFNM3, _SETL3)};

std::array<VolumeSensor, _NUMBER_OF_KETTLES> vs_arr = {
    VolumeSensor(_VSNM1, _CHAN1, _OFFS1),
    VolumeSensor(_VSNM2, _CHAN2, _OFFS2),
    VolumeSensor(_VSNM3, _CHAN3, _OFFS3)};

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
}

void loop(void)
{
  StaticJsonDocument<1024> message;

  for (int i = 0; i < _NUMBER_OF_KETTLES; i++)
  {
    kf_arr[i].vs_ptr = &vs_arr[i];
    message["data"][kf_arr[i].name]["kf_name"] = kf_arr[i].name;
    message["data"][kf_arr[i].name]["vs_name"] = kf_arr[i].vs_ptr->name;
    message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
    message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
    message["data"][kf_arr[i].name]["liters"] = kf_arr[i].vs_ptr->read_liters();
    message["data"][kf_arr[i].name]["gallons"] = kf_arr[i].vs_ptr->read_gallons();
  }
  serializeJsonPretty(message, Serial);
  delay(5000);
}
