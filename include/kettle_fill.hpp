#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

#include "kettle_fill_config.hpp"

Adafruit_ADS1115 ads;


class KettleFiller
{
public:
    String name;
    uint16_t adc;        // Analog Bits
    float volts;         // volts from sensor
    uint8_t ads_channel; // Channel of the ADS1115 to read
    uint8_t valve_pin;   // Esp32 pin of the spunder valve relay
    uint8_t unit_max;    // Max pressure rating of the transducer in kPa

    float max_liters  = _MAXLITERS;
    float max_gallons = _MAXGALLONS;
    float desired_liters;
    float liters;
    float desired_gallons = desired_liters * .264172;
    float gallons = liters * .264172;

    float percent_full;
    float percent_open;
    int output;
    
    KettleFiller();
    KettleFiller(String, uint8_t, uint8_t, float);
    ~KettleFiller();

    float get_liters();
    float get_percent_full();
    float get_percent_open();
    void set_output();
    
    float get_desired_liters();
    int get_output();
    void run();

    uint16_t get_adc();       // Read adc value from ads1115
    float get_volts();        // Read volts value from ads1115
};

KettleFiller::KettleFiller(){}
KettleFiller::~KettleFiller(){ Serial.println("destructor invoked"); }

KettleFiller::KettleFiller( String nm, uint8_t vp, uint8_t ch, float dl)
{
  name            = nm;
  valve_pin       = vp;
  ads_channel     = ch;
  desired_liters  = dl;
}

uint16_t KettleFiller::get_adc()
{
    adc = ads.readADC_SingleEnded(ads_channel);

    return adc;
}

float KettleFiller::get_volts()
{
    //volts = ads.computeVolts(ads.readADC_SingleEnded(ads_channel));
    volts = ads.computeVolts(get_adc());

    return volts;
}

float KettleFiller::get_desired_liters()
{
    return desired_liters;
}

float KettleFiller::get_liters()
{
    //volts = ads.computeVolts(ads.readADC_SingleEnded(ads_channel));
    //liters = (volts() - _ZEROVOLTS) * (_MAXLITERS / (_MAXVOLTS - _ZEROVOLTS));
    liters = (get_volts() - _ZEROVOLTS) * (_MAXLITERS / (_MAXVOLTS - _ZEROVOLTS));
    if (liters < 0) { liters = 0; }; 

    return liters;
}

float KettleFiller::get_percent_full()
{
    percent_full = 1.0 - ((desired_liters - get_liters()) / desired_liters);
    if (percent_full > 1.0) { percent_full = 1.0; }; 
    if (percent_full < 0.0) { percent_full = 0.0; }; 

    return percent_full;
}

float KettleFiller::get_percent_open()
{
    percent_open = (desired_liters - get_liters()) / desired_liters;
    if (percent_open > 1.0) { percent_open = 1.0; }; 
    if (percent_open < 0.0) { percent_open = 0.0; }; 

    return percent_open;
}

int KettleFiller::get_output()
{
    output = int((desired_liters - get_liters()) / desired_liters) * 255;
    return output;
}

void KettleFiller::set_output()
{
    //percent_full = ((desired_liters - get_liters()) / desired_liters);
    digitalWrite(valve_pin, get_output());
}

void KettleFiller::run() 
{
  StaticJsonDocument<1024> message;
  
  message["key"] = _CLIENTID;
  message["data"][name]["adc"]     = get_adc(); 
  message["data"][name]["volts"]   = get_volts();
  message["data"][name]["sp_l"]    = get_desired_liters();
  message["data"][name]["liters"]  = get_liters();
  message["data"][name]["full"]    = get_percent_full();
  message["data"][name]["open"]    = get_percent_open();
  message["data"][name]["output"]  = get_output();
    
  serializeJsonPretty(message, Serial);
  
  set_output();
}
