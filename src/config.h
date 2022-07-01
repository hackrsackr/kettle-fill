#pragma once
#include <array>
#include "Arduino.h"
#include "secrets.h"

#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

// MQTT setup
#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "kettle-filler"
#define _SUBTOPIC "brewcast/history/meters"
#define _PUBTOPIC "brewcast/history/kettle-filler"

#define _NUMBER_OF_KETTLES 3

/////////////////////////////KF////////////////////////////////
// KettleFiller->name 
std::string _KFNM1 = "kf_liqr";
std::string _KFNM2 = "kf_mash";
std::string _KFNM3 = "kf_boil";
std::array<std::string, _NUMBER_OF_KETTLES> 
    KF_NAMES = {_KFNM1, _KFNM2, _KFNM3};

// KettleFiller->desired_liters
float _SETL1 = 27.00;
float _SETL2 = 28.00;
float _SETL3 = 29.00;
std::array<float, _NUMBER_OF_KETTLES> 
    SETPOINTS = {_SETL1, _SETL2, _SETL3};
/////////////////////////////KF////////////////////////////////

/////////////////////////////VS////////////////////////////////

// VolumeSensor->name
std::string _VSNM1 = "vs_liqr";
std::string _VSNM2 = "vs_mash";
std::string _VSNM3 = "vs_boil";
std::array<std::string, _NUMBER_OF_KETTLES>
    VS_NAMES = {_VSNM1, _VSNM2, _VSNM3};

// VolumeSensor->ads_channel
int _CHAN1 = 0;
int _CHAN2 = 1;
int _CHAN3 = 2;
std::array<int, _NUMBER_OF_KETTLES>
    ADS_CHANNELS = {_CHAN1, _CHAN2, _CHAN3};

// VolumeSensor->adc_offset
int _OFFS1 = 10193;   
int _OFFS2 = 10208;
int _OFFS3 = 8000;
std::array<int, _NUMBER_OF_KETTLES>
    ADC_OFFSETS = {_OFFS1, _OFFS2, _OFFS3};
/////////////////////////////VS////////////////////////////////

/////////////////////////////PV////////////////////////////////
// PropValve->name
std::string _PVNM1 = "pv_h20";
// PropValve->pin: valve pwm pin on esp32
int _VPIN1 = 4;
int _POS = 0;
/////////////////////////////PV////////////////////////////////









