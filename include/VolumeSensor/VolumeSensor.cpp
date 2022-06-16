#include <iostream>

#include "Adafruit_ADS1X15.h"

#include "VolumeSensor.hpp"

uint8_t ads_ch = 0;                     // ads_channel
uint16_t ads_off = 8000;                // ads_offset

Adafruit_ADS1115 ads;

VolumeSensor v(ads_ch, ads_off);

void setup() {
    Serial.begin(11520);
    ads.begin(0x48);

    v.print_data();
}

void loop() {

}