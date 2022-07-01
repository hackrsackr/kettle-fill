#include <iostream>
#include <ArduinoJson.h>

#include <array>
//#include "VolumeSensor.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

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
    message["data"][vs_arr[i].name]["liters"] = vs_arr[i].read_liters();
    message["data"][vs_arr[i].name]["galloms"] = vs_arr[i].read_gallons();
  }
  serializeJsonPretty(message, Serial);
  delay(5000);
}
