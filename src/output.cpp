#include <iostream>
#include <array>

#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "kettle_fill.hpp"
#include "kettle_fill_config.hpp"

Adafruit_ADS1115 ads1;
KettleFiller kf2(_NAME2, _PWM2, _CHAN2, _SETL2);

void setup(void)
{
  Serial.begin(115200);
  ads1.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  pinMode(kf2.valve_pin, OUTPUT);
  //digitalWrite(kf2.valve_pin, 0);
}

void loop(void)
{  
  //StaticJsonDocument<1024> message;
  
  //message["key"] = _CLIENTID;


  //message["data"][kf2.name]["adc"]     = kf2.get_adc(); 
  //message["data"][kf2.name]["volts"]   = kf2.get_volts();
  //message["data"][kf2.name]["sp_l"]    = kf2.get_desired_liters();
  //message["data"][kf2.name]["liters"]  = kf2.get_liters();
  //message["data"][kf2.name]["full"]    = kf2.get_percent_full();
  //message["data"][kf2.name]["open"]    = kf2.get_percent_open();
  //message["data"][kf2.name]["output"]     = kf2.get_output();
    
  //kf2.set_output();
  //serializeJsonPretty(message, Serial);
  kf2.run();
  delay(5000);
}