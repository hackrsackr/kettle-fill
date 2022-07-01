#pragma once
#include <iostream>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

class VolumeSensor
{
public:
    Adafruit_ADS1115* ads_ptr;
    std::string name;
    int adc;                       // Analog Bits
    int net_adc;                       // Analog Bits
    int ads_channel;               // Channel of the ADS1115 to read
    int adc_offset;                // Analog Bits at level flush
    float bitsPerGallon = 1675.00; // Analog Bits
    float bitsPerLiter = 442.54;   // Analog Bits
    float liters;                  // Volume in Liters
    float gallons;                   // Volume in Volts
    float volts;                   // Volume in Volts

    VolumeSensor();
    VolumeSensor(std::string, int, int); // (name, ads_channel, adc_offset)

    int read_adc();       // Read sensor value
    int trim_adc();                                 // Read sensor value minus offset
    float read_volts();                             // Read sensor value in Volts
    float read_liters();                            // Read sensor value in Liters
    float read_gallons();                           // Read sensor value in Gallons
    void print_data();
    void print_volume();
    void run();
};

VolumeSensor::VolumeSensor(std::string nm, int ch, int offset)
{
    name = nm;
    ads_channel = ch;
    adc_offset = offset;
}

int VolumeSensor::read_adc()
{
    adc = ads.readADC_SingleEnded(ads_channel);
    return adc;
}

int VolumeSensor::trim_adc()
{
    net_adc = ads.readADC_SingleEnded(ads_channel) - adc_offset;
    return net_adc;
}

float VolumeSensor::read_volts()
{
    volts = ads.computeVolts(read_adc());
    return volts;
}

float VolumeSensor::read_liters()
{
    liters = ((trim_adc() / bitsPerLiter) > 0) ? (trim_adc() / bitsPerLiter) : 0;
    return liters;
}

float VolumeSensor::read_gallons()
{
    gallons = ((trim_adc() / bitsPerGallon) > 0) ? (trim_adc() / bitsPerGallon) : 0;
    return gallons;
}

void VolumeSensor::print_data()
{
    std::cout << name << "\n"
              << "adc: " << read_adc() << "\n"
              << "net-adc: " << trim_adc() << "\n"
              << "volts: " << read_volts() << "\n"
              << "liters: " << read_liters() << "\n"
              << "gallons: " << read_gallons() << "\n"
              << "\n";
}

void VolumeSensor::print_volume()
{
    std::cout.precision(3);
    std::cout << "{ \"" << name << "\": "
              << "{ \"liters\": " << read_liters() << ", "
              << "\"gallons\": " << read_gallons() << " }}\n"
              << "\n";
}

void VolumeSensor::run()
{
    adc = ads.readADC_SingleEnded(ads_channel);
    volts = ads.computeVolts(read_adc());
    net_adc = ads.readADC_SingleEnded(ads_channel) - adc_offset;
    liters = ((trim_adc() / bitsPerLiter) > 0) ? (trim_adc() / bitsPerLiter) : 0;
    gallons = ((trim_adc() / bitsPerGallon) > 0) ? (trim_adc() / bitsPerGallon) : 0;
}
