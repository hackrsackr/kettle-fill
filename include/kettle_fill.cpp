#include <iostream>
#include <array>

#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "kettle_fill.hpp"
#include "kettle_fill_config.hpp"

Adafruit_ADS1115 ads;

KettleFiller::KettleFiller() {}

KettleFiller::KettleFiller(String nm, uint8_t vp, uint8_t ch, float dl)
{
    name = nm;
    valve_pin = vp;
    ads_channel = ch;
    desired_liters = dl;
}

uint16_t KettleFiller::get_adc()
{
    adc = ads.readADC_SingleEnded(ads_channel);
    return adc;
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

int KettleFiller::get_percent_full()
{
    percent_full = 1.0 - ((desired_liters - get_actual_liters()) / desired_liters) * 100;
    if (percent_full > 100) {percent_full = 100;}
    if (percent_full < 0) {percent_full = 0;}
    return percent_full;
}

int KettleFiller::get_percent_open()
{
    percent_open = (desired_liters - get_actual_liters()) / desired_liters * 100;
    if (percent_open > 100) {percent_open = 100;}
    if (percent_open < 0) {percent_open = 0;}
    return percent_open;
}

int KettleFiller::get_desired_output()
{
    desired_output = (get_percent_open() / 100) * 255;
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

//void KettleFiller::run()
//{
//    StaticJsonDocument<1024> data;

//    data[name]["adc"] = get_adc();
//    data[name]["volts"] = get_volts();
//    data[name]["sp_l"] = get_desired_liters();
//    data[name]["liters"] = get_actual_liters();
//    data[name]["full"] = get_percent_full();
//    data[name]["open"] = get_percent_open();
//    data[name]["output"] = get_desired_output();

//    set_output(get_desired_output());

//    serializeJsonPretty(data, Serial);

//}