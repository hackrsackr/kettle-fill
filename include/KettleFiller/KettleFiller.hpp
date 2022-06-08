#pragma once

#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "KettleFiller_config.hpp"

Adafruit_ADS1115 ads;

class KettleFiller {
public:
    String name;                // Identifier
    uint8_t valve_pin;          // Esp32 pin of the proportional valve
    uint8_t ads_channel;        // Channel of the ADS1115 to read
    uint8_t feedback_pin;       // Esp32 pin of the feedback signal
    uint16_t sensor_adc;        // Analog Bits from level sensor

    float desired_liters;       // Setpoint in Liters
    float actual_liters;        // Volume in Liters
    float sensor_volts;         // Volts from sensor
    float actual_output;        // Volts from feedback pin
    float desired_output;       // Desired valve PWM
    float percent_full;         // Actual_liters / Desired_liters
    float percent_open;         // Inverse of percentage full
    
    KettleFiller(String, uint8_t, uint8_t, float);
    uint16_t get_adc();       
    float get_volts();  
    void set_output(int);
    float get_desired_output();
    float get_actual_output();
    float get_actual_liters();
    float get_desired_liters();
    float get_percent_full();
    float get_percent_open();
};

KettleFiller::KettleFiller(String nm, uint8_t vp, uint8_t ch, float dl)
{
    name = nm;
    valve_pin = vp;
    ads_channel = ch;
    desired_liters = dl;
}

uint16_t KettleFiller::get_adc()
{
    sensor_adc = ads.readADC_SingleEnded(ads_channel);
    return sensor_adc;
}

float KettleFiller::get_volts()
{
    sensor_volts = ads.computeVolts(get_adc());
    return sensor_volts;
}

float KettleFiller::get_desired_liters()
{
    return desired_liters;
}

float KettleFiller::get_actual_liters()
{
    actual_liters = (get_volts() - _ZEROVOLTS) * (_MAXLITERS / (_MAXVOLTS - _ZEROVOLTS));
    if (actual_liters < 0) {actual_liters = 0;}
    return actual_liters;
}

float KettleFiller::get_percent_full()
{
    //percent_full = 1.0 - ((desired_liters - get_actual_liters()) / desired_liters) * 100;
    percent_full = (get_actual_liters() / desired_liters) * 100;
    if (percent_full > 100) {percent_full = 100;}
    if (percent_full < 0) {percent_full = 0;}
    return percent_full;
}

float KettleFiller::get_percent_open()
{
    //percent_open = (desired_liters - get_actual_liters()) / desired_liters * 100;
    //percent_open = 1 - ((get_actual_liters() / desired_liters) * 100);
    percent_open = 100 - get_percent_full();
    if (percent_open > 100) {percent_open = 100;}
    if (percent_open < 0) {percent_open = 0;}
    return percent_open;
}

float KettleFiller::get_desired_output()
{
    desired_output = (get_percent_open() / 100.0) * 255.0;
    return desired_output;
}

void KettleFiller::set_output(int d_output)
{
    digitalWrite(valve_pin, get_desired_output());
}

float KettleFiller::get_actual_output() 
{
    actual_output = analogReadMilliVolts(feedback_pin) / 1000.0;
    return actual_output;
}