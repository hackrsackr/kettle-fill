#pragma once

#include <ArduinoJson.h>

#define _CLIENTID "kettle-fillers"  // key for message

// Common
#define _UNITMAX 10                 // Maximum value in kPa
#define _MAXVOLTS  5.03             // VCC
#define _MAXVOLTS_ADC  26825        // ADC at MAXVOLTS
#define _MAXLITERS 77.5             // Max volume in Liter
#define _ZEROVOLTS 1.93             // Voltage with no liquid
#define _ZEROVOLTS_ADC 10314        // Adc at ZEROVOLTS

// KF1-Specific
#define _NAME1 "liqr"               // Identifier
#define _PWM1 4                     // Valve_pin: valve pwm pin on esp32
#define _FBV1 1                     // Feedback_pin: valve feedback pin on esp32
#define _CHAN1 1                    // ADS1115 channel
#define _SETL1 27.00f               // Setpoint in Liters 

// KF2-Specific
#define _NAME2 "mash"               // Identifier
#define _PWM2 26                    // Valve_pin: valve pwm pin on esp32
#define _FBV2 2                     // Feedback_pin: valve feedback pin on esp32
#define _CHAN2 2                    // ADS1115 channel
#define _SETL2 27.00f               // Setpoint in Liters 

// KF3-Specific
#define _NAME3 "boil"               // Identifier
#define _PWM3 26                    // Valve_pin: valve pwm pin on esp32
#define _FBV3 2                     // Feedback_pin: valve feedback pin on esp32
#define _CHAN3 3                    // ADS1115 channel
#define _SETL3 27.00f               // Setpoint in Liters 