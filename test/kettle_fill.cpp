#include <iostream>
#include <array>

#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "kettle_fill.hpp"
#include "kettle_fill_config.hpp"

Adafruit_ADS1115 ads1;
std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr;

void setup(void)
{
  Serial.begin(115200);
  ads1.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  for (uint8_t kf=0; kf < _NUMBER_OF_KETTLES; kf++)
  {
    kf_arr[kf].name            = _KF_NAMES[kf];
    kf_arr[kf].valve_pin       = _PWM_PINS[kf];
    kf_arr[kf].ads_channel     = _ADS_PINS[kf];
    kf_arr[kf].desired_liters  = _D_LITERS[kf];
    kf_arr[kf].desired_gallons = _D_LITERS[kf] * 0.264172;

    pinMode(kf_arr[kf].valve_pin, OUTPUT);
    digitalWrite(kf_arr[kf].valve_pin, 0);
  }
}

void loop(void)
{  
  StaticJsonDocument<1024> message;
  
  message["key"] = _CLIENTID;

  for (uint8_t kf=0; kf < _NUMBER_OF_KETTLES; kf++)
  {
    message["data"][kf_arr[kf].name]["adc"]     = ads.readADC_SingleEnded(kf_arr[kf].ads_channel);
    message["data"][kf_arr[kf].name]["volts"]   = ads.computeVolts(ads.readADC_SingleEnded(kf_arr[kf].ads_channel));
    message["data"][kf_arr[kf].name]["sp_l"]    = kf_arr[kf].desired_liters;
    message["data"][kf_arr[kf].name]["liters"]  = kf_arr[kf].get_liters();
    message["data"][kf_arr[kf].name]["sp_gal"]  = kf_arr[kf].desired_gallons;
    message["data"][kf_arr[kf].name]["full"]    = kf_arr[kf].percent_full;
    message["data"][kf_arr[kf].name]["pwm"]     = kf_arr[kf].output;
    
    kf_arr[kf].set_pwm();
  }
  serializeJsonPretty(message, Serial);
  
  delay(5000);
}