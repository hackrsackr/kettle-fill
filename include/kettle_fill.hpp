#pragma once

//#include "kettle_fill_config.hpp"

class KettleFiller {
public:
    String name;
    uint16_t adc;        // Analog Bits
    uint8_t ads_channel; // Channel of the ADS1115 to read
    uint8_t valve_pin;   // Esp32 pin of the spunder valve relay
    uint8_t feedback_pin;   // Esp32 pin of the spunder valve relay
    uint8_t unit_max;    // Max pressure rating of the transducer in kPa

    float desired_liters;
    float actual_liters;

    float sensor_volts;         // volts from sensor
    //float desired_gallons = desired_liters * .264172;
    //float actual_gallons = actual_liters * .264172;

    int percent_full;
    int percent_open;
    int desired_output;
    float actual_output;
    
    KettleFiller(String, uint8_t, uint8_t, float);
    KettleFiller();

    void set_output(int);
    int get_desired_output();
    float get_actual_output();
    
    float get_actual_liters();
    float get_desired_liters();
    int get_percent_full();
    int get_percent_open();
    
    //void run();

    uint16_t get_adc();       
    float get_volts();  

};