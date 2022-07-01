#pragma once
#include <iostream>
#include <array>
// #include "Arduino.h"
#include "secrets.h"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "KettleFiller/KettleFiller.hpp"

// WiFi
#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

// MQTT setup
#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "kettle-filler"
#define _SUBTOPIC "brewcast/history/spark-three"
#define _PUBTOPIC "brewcast/history/kettle-filler"

// Global
#define _NUMBER_OF_KETTLES 3

/////////////////////////////VALVES////////////////////////////////

std::string mashUnderlet_valve = "ACT_valve-01_mashUnderlet";
std::string mashDrain_valve = "ACT_valve-02_mashDrain";
std::string liqrReturnDrain_valve = "ACT_valve-03_liqrReturnDrain";
std::string mashReturn_valve = "ACT_valve-04_mashReturn";
std::string liqrReturn_valve = "ACT_valve-05_liquorReturn";
std::string mashReturnDrain_valve = "ACT_valve-06_mashReturnDrain";
std::string wortPumpsDrain_valve = "ACT_valve-07_wortPumpsDrain";
std::string boilReturn_valve = "ACT_valve-08_boilReturn";
std::string wortDrain_valve = "ACT_valve-09_wortDrain";
std::string boilDrain_valve = "ACT_valve-10_boilDrain";
std::string hexWortIn_valve = "ACT_valve-11_hexWortIn";
std::string boilReturnDrain_valve = "ACT_valve-12_boilReturnDrain";
std::string liqrDrain_valve = "ACT_valve-13_liquorPump";
std::string liqrIn_valve = "ACT_valve-14_LiquorPumpDrain";
std::string liqr_3w_valve = "ACT_3WV-l_hexLiquorOut";
std::string wort_3w_valve = "ACT_3WV-l_hexWortOut";
std::string h20_3w_valve = "ACT_3WV-l_hexLiqourIn";
/////////////////////////////PUMPS////////////////////////////////

/////////////////////////////PV////////////////////////////////

std::string _PVNM1 = "pv_h20";          // PropValve->name
int _VPIN1 = 4;                         // PropValve->pin: valve pwm pin on esp32
int _VPOS = 0;                           // PropValve->position

PropValve pv(_PVNM1, _VPIN1);
/////////////////////////////PV////////////////////////////////

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

std::array<VolumeSensor, _NUMBER_OF_KETTLES> vs_arr = {
    VolumeSensor(_VSNM1, _CHAN1, _OFFS1),
    VolumeSensor(_VSNM2, _CHAN2, _OFFS2),
    VolumeSensor(_VSNM3, _CHAN3, _OFFS3)};
/////////////////////////////VS////////////////////////////////

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
float _SETL3 = 29.00;std::array<float, _NUMBER_OF_KETTLES> 
    SETPOINTS = {_SETL1, _SETL2, _SETL3};

std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr = {
  KettleFiller (_KFNM1, _SETL1),
  KettleFiller (_KFNM2, _SETL2),
  KettleFiller (_KFNM3, _SETL3)};
/////////////////////////////KF////////////////////////////////








