#include "KettleFiller/KettleFiller.hpp"
//#include "KettleFiller/KettleFiller_config.hpp"

KettleFiller kf1(_NAME1, _PWM1, _CHAN1, _SETL1);

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  pinMode(kf1.valve_pin, OUTPUT);
  digitalWrite(kf1.valve_pin, 0);
  
//  kf1.feedback_pin = _FBV1;
//  pinMode(kf1.feedback_pin, INPUT);
}

void loop(void)
{  
  StaticJsonDocument<1024> message;

  message["key"] = _CLIENTID;
  
  message["data"][kf1.name]["adc"]    = kf1.get_adc();
  message["data"][kf1.name]["volts"]  = kf1.get_volts();
  message["data"][kf1.name]["sp_l"]   = kf1.get_desired_liters();
  message["data"][kf1.name]["liters"] = kf1.get_actual_liters();
  message["data"][kf1.name]["full"]   = kf1.get_percent_full();
  message["data"][kf1.name]["open"]   = kf1.get_percent_open();
  message["data"][kf1.name]["output"] = kf1.get_desired_output();
  //message["data"][kf1.name]["out_v"]  = kf1.get_actual_output(); 
  
  kf1.set_output(kf1.get_desired_output());

  serializeJsonPretty(message, Serial);

  delay(5000);
}