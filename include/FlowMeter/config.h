#pragma once

//#include "secrets.h"

// WiFi setup
//#define _SSID SECRET_SSID
//#define _PASS SECRET_PASS

// MQTT setup
//#define _MQTTHOST "192.168.1.2"
//#define _MQTTPORT 1883
//#define _CLIENTID "kettle-filler"
//#define _SUBTOPIC "brewcast/history/meters"
//#define _PUBTOPIC "brewcast/history/kettle-filler"

// FlowMeter setup
// FlowMeter names
#define _FLNM1 "flow-meter-1"
#define _FLNM2 "flow-meter-2"
#define _FLNM3 "flow-meter-3"

// FlowMeter sensor pins
#define _SPIN1 39
#define _SPIN2 35
#define _SPIN3 36

// FlowMeter calibration Factors
#define _YF_S302 21.85 // [23*Q-3] f = 23*60-(23*3) = 1311 bits per liter 1311/60 = 21.85
#define _YFS402B 32.01 // f = Q*32 32*60 = 1920 bits per liter
#define _SS_FLOW 9.93  // 596 bits per liter 596/60 = 9.93  