#pragma once

#include <iostream>

#include "Adafruit_ADS1X15.h"

class VolumeSensor 
{
public:

    uint8_t sensor_pin;                         // 
    uint8_t ads_channel;                        // Channel of the ADS1115 to read
    uint16_t raw_adc;                               // Analog Bits
    uint16_t adc_offset;                        // Analog Bits at level flush
    float bitsPerLiter = 442.54;                      // Analog Bits
    float volts;
    float liters;                               //  
    float esp_vusb;                             // Actual voltage of the 5v intput

    VolumeSensor();
    VolumeSensor(uint8_t, uint16_t);
    
    uint16_t read_adc();                  // Read sensor value in Liters
    uint8_t read_volts();                  // Read sensor value in Liters
    float adc_to_liters();    // Read sensor value in Liters

    void print_data();
    Adafruit_ADS1115* ads;
};

VolumeSensor::VolumeSensor(uint8_t ch, uint16_t offset) 
{
    ads_channel = ch;
    adc_offset = offset;
}

uint16_t VolumeSensor::read_adc()
{
    raw_adc = ads->readADC_SingleEnded(ads_channel);
    return raw_adc;
}

uint8_t VolumeSensor::read_volts()
{
    volts = ads->computeVolts(ads->readADC_SingleEnded(ads_channel));
    return volts;
}

float VolumeSensor::adc_to_liters()
{
    liters = (read_adc() - adc_offset) * bitsPerLiter;
    return liters;
}

void VolumeSensor::print_data() {
    std::cout << "adc" << this->read_adc() << std::endl; 
    std::cout << "volts" << this->read_volts() << std::endl; 
    std::cout << "liters" << this->adc_to_liters() << std::endl; 
}