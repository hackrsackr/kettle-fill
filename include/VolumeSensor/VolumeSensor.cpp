#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "VolumeSensor/VolumeSensor.hpp"

int vusb_ch = 4;
int ads_ch = 0;                     // ads_channel
int ads_off = 8000;                // ads_offset

Adafruit_ADS1115 ads;

VolumeSensor v(ads_ch, ads_off);

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  v.name = "liqr";
}

void loop(void)
{
    v.run();
}

