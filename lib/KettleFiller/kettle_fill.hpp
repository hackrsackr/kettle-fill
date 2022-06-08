#pragma once

class KettleFiller {
public:
    String name;                // Identifier
    uint8_t valve_pin;          // Esp32 pin of the spunder valve relay
    uint8_t ads_channel;        // Channel of the ADS1115 to read
    uint8_t feedback_pin;       // Esp32 pin of the spunder valve relay
    uint16_t sensor_adc;        // Analog Bits

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