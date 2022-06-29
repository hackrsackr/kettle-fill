#pragma once

#include <array>
#include "secrets.h"
#include "KettleFiller/KettleFiller.hpp"

#define _SSID SECRET_SSID
#define _PASS SECRET_PASS

// MQTT setup
#define _MQTTHOST "192.168.1.2"
#define _MQTTPORT 1883
#define _CLIENTID "kettle-filler"
#define _SUBTOPIC "brewcast/history/meters"
#define _PUBTOPIC "brewcast/history/kettle-filler"

// General
#define _NUMBER_OF_KETTLES 3 //
#define _UNITMAX 10          // Maximum value in kPa
#define _MAXVOLTS 5.03       // VCC
#define _MAXLITERS 77.5      // Max volume in Liter
#define _ZEROVOLTS 1.93      // Voltage with no liquid
#define _ZEROVOLTS_ADC 10314 // Adc at ZEROVOLTS
#define _PVNM1 "pv1"         // PropValve Identifier
#define _VPIN1 4             // PropValve->pin: valve pwm pin on esp32
#define _FPIN1 14            // PropValve->Feedback_pin: valve feedback pin on esp32

// KF1-Specific
String _KFNM1 = "liqr"; // KettleFiller Identifier
String _KFNM2 = "mash"; // KettleFiller Identifier
String _KFNM3 = "boil"; // KettleFiller Identifier
float _SETL1 = 27.00;   // KettleFiller->desired_liters
// VS1-Specific
String _VSNM1 = "vs_liqr"; // VolumeSensor Identifier
uint8_t _CHAN1 = 2;        // VolumeSensor->ads_channel
uint16_t _OFFS1 = 10193;   // VolumeSensor->adc_offset

// KF2-Specific
float _SETL2 = 28.00;   // KettleFiller->desired_liters
// VS2-Specific
String _VSNM2 = "vs_mash"; // VolumeSensor Identifier
uint8_t _CHAN2 = 2;        // VolumeSensor->ads_channel
uint16_t _OFFS2 = 8000;    // VolumeSensor->adc_offset

// KF3-Specific
float _SETL3 = 29.00;   // KettleFiller->desired_liters
// VS3-Specific
String _VSNM3 = "vs_boil"; // VolumeSensor Identifier
uint8_t _CHAN3 = 2;        // VolumeSensor->ads_channel
uint16_t _OFFS3 = 8000;     // VolumeSensor->adc_offset

uint8_t _CHAN4 = 3;        // VolumeSensor->ads_channel

std::array<String, _NUMBER_OF_KETTLES> 
KF_NAMES = {_KFNM1, _KFNM2, _KFNM3};

std::array<float, _NUMBER_OF_KETTLES> 
SETPOINTS = {_SETL1, _SETL2, _SETL3};

std::array<String, _NUMBER_OF_KETTLES>
    VS_NAMES = {_VSNM1, _VSNM2, _VSNM3};

std::array<uint8_t, 4>
    ADS_CHANNELS = {_CHAN1, _CHAN2, _CHAN3, _CHAN4};

std::array<uint16_t, _NUMBER_OF_KETTLES>
    ADC_OFFSETS = {_OFFS1, _OFFS2, _OFFS3};

std::array<const char *, _NUMBER_OF_KETTLES>
    VOL_PLACEHOLDERS = {"VOLUME1", "VOLUME2", "VOLUME3"};

std::array<const char *, _NUMBER_OF_KETTLES>
    SET_PLACEHOLDERS = {"SETPOINT1", "SETPOINT2", "SETPOINT3"};

std::array<const char *, _NUMBER_OF_KETTLES>
    CBX_PLACEHOLDERS = {"CHECKBOX_INPUT1", "CHECKBOX_INPUT2", "CHECKBOX_INPUT3"};


std::array<const char *, _NUMBER_OF_KETTLES>
    TOPICS = {"output3-1", "output3-2", "output3-3"};