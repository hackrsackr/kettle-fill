#pragma once

#define _CLIENTID "kettle-fillers"  // key for message

// ALL
#define _NUMBER_OF_KETTLES 2        // kettles
#define _UNITMAX 10                 // kPa
#define _MAXVOLTS  5.03             // VCC
#define _MAXVOLTS_ADC  26825        // adc at MAXVOLTS
#define _MAXLITERS 77.5             // adc at MAXVOLTS
#define _MAXGALLONS 20              // adc at MAXVOLTS
#define _ZEROVOLTS 1.93             // voltage with no liquid
#define _ZEROVOLTS_ADC 10314        // adc at ZEROVOLTS

// KF1
#define _PWM1 4                     // valve pwm pin on esp32
#define _NAME1 "liqr"               // name
#define _CHAN1 1                    // ads channel
#define _SETL1 27.00                // setpoint in liters 

// KF2
#define _PWM2 26                    // valve pwm pin on esp32
#define _NAME2 "boil"               // name
#define _CHAN2 3                    // ads channel
#define _SETL2 27.00                // setpoint in liters 




std::array<const String,  _NUMBER_OF_KETTLES> _KF_NAMES = { _NAME1, _NAME2 };
std::array<const uint8_t, _NUMBER_OF_KETTLES> _PWM_PINS = { _PWM1, _PWM2 };
std::array<const uint8_t, _NUMBER_OF_KETTLES> _ADS_PINS = { _CHAN1, _CHAN2 };
std::array<const float,   _NUMBER_OF_KETTLES> _D_LITERS = { _SETL1, _SETL2 };
