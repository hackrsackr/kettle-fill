#pragma once

// General 
#define _CLIENTID "kettle-fillers"  // Key for output message
//#define _NUMBER_OF_KETTLES 3        // 
#define _UNITMAX 10                 // Maximum value in kPa
#define _MAXVOLTS  5.03             // VCC
#define _MAXLITERS 77.5             // Max volume in Liter
#define _ZEROVOLTS 1.93             // Voltage with no liquid
#define _ZEROVOLTS_ADC 10314        // Adc at ZEROVOLTS
#define _PVID1 "pv1"                // PropValve Identifier
#define _VPIN1 4                    // PropValve->pin: valve pwm pin on esp32
#define _FPIN1 2                   // PropValve->Feedback_pin: valve feedback pin on esp32

// KF1-Specific
#define _KFNM1 "liqr"               // KettleFiller Identifier
#define _SETL1 27.00                // KettleFiller->desired_liters 
// VS1-Specific
#define _VSNM1 "vs_liqr"            // VolumeSensor Identifier
#define _CHAN1 1                    // VolumeSensor->ads_channel
#define _OFFS1 10193                // VolumeSensor->adc_offset 

// KF2-Specific
#define _KFNM2 "mash"               // KettleFiller Identifier
#define _SETL2 27.00                // KettleFiller->desired_liters 
// VS2-Specific
#define _VSNM2 "vs_mash"            // VolumeSensor Identifier
#define _CHAN2 1                    // VolumeSensor->ads_channel
#define _OFFS2 8000                 // VolumeSensor->adc_offset 

// KF3-Specific
#define _KFNM3 "boil"               // KettleFiller Identifier
#define _SETL3 27.00                // KettleFiller->desired_liters 
// VS3-Specific
#define _VSNM3 "vs_boil"            // VolumeSensor Identifier
#define _CHAN3 1                    // VolumeSensor->ads_channel
#define _OFFS3 8000                 // VolumeSensor->adc_offset 