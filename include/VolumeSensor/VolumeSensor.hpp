#pragma once

#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

Adafruit_ADS1115 ads;

class VolumeSensor
{
public:
    std::string name;
    int ads_channel;                        // Channel of the ADS1115 to read
    int adc_offset;                        // Analog Bits at level flush
    float bitsPerGallon = 1675.00;                      // Analog Bits
    float bitsPerLiter = 442.54;                      // Analog Bits
    //int sensor_pin;                         // 
    //int raw_adc;                               // Analog Bits
    //float volts;
    float liters;                               //  
    //float esp_vusb;                             // Actual voltage of the 5v intput

    VolumeSensor();
    VolumeSensor(int, int);                 // (ads_channel, adc_offset)
    
    int read_adc();                  // Read sensor value in Liters
    int trim_adc();                  // Read sensor value in Liters
    float read_volts();                  // Read sensor value in Liters
    float read_liters();    // Read sensor value in Liters
    float read_gallons();    // Read sensor value in Liters
    void print_data();
    void run();
    
};

VolumeSensor::VolumeSensor(int ch, int offset) 
{
    this->ads_channel = ch;
    this->adc_offset = offset;
}

int VolumeSensor::read_adc()
{
    return ads.readADC_SingleEnded(this->ads_channel);
}

int VolumeSensor::trim_adc()
{
    return (ads.readADC_SingleEnded(this->ads_channel) - this->adc_offset);
}

float VolumeSensor::read_volts()
{
    return ads.computeVolts(this->read_adc());
}

float VolumeSensor::read_liters()
{
    this->liters = ((this->trim_adc() / this->bitsPerLiter) > 0) ? (this->trim_adc() / this->bitsPerLiter) : 0;
    return this->liters;
}

float VolumeSensor::read_gallons()
{
    return ((this->trim_adc() / this->bitsPerGallon) > 0) ? (this->trim_adc() / this->bitsPerGallon) : 0;
}

void VolumeSensor::print_data() {
    std::cout << "adc: " << this->read_adc() << std::endl; 
    std::cout << "net-adc: " << this->trim_adc() << std::endl; 
    std::cout << "volts: " << this->read_volts() << std::endl; 
    std::cout << "liters: " << this->read_liters() << std::endl; 
    std::cout << "gallons: " << this->read_gallons() << std::endl; 
}

void VolumeSensor::run() {
  StaticJsonDocument<384> message;
  
  message["key"] = "vs_test";
  this->name = "vs1";
  
  message["data"][this->name]["adc"] = this->read_adc();
  message["data"][this->name]["net-adc"] = this->trim_adc();
  message["data"][this->name]["volts"] = this->read_volts();
  message["data"][this->name]["liters"] = this->read_liters();
  message["data"][this->name]["gallons"] = this->read_gallons();
  
  //std::cout << message << std::endl;
  serializeJsonPretty(message, Serial);
  std::cout << std::endl;
  
  delay(5000);
}