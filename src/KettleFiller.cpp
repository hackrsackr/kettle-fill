#include "ArduinoJson.h"
#include "KettleFiller/KettleFiller.hpp"

Adafruit_ADS1115 ads;
KettleFiller kf1(_NAME1, _VPIN1, _FPIN1, _CHAN1, _OFFS1, _SETL1);

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

  pinMode(_VPIN1, OUTPUT);
  pinMode(_FPIN1, INPUT);
}

void loop(void)
{  
  StaticJsonDocument<1024> message;

  message["key"] = _CLIENTID;
  
  message["data"][kf1.name]["des_ls"] = kf1.desired_liters;
  message["data"][kf1.name]["liters"] = kf1.read_liters();
  message["data"][kf1.name]["filled"] = kf1.get_percent_full();
  message["data"][kf1.name]["va-pos"] = kf1.determine_valve_position();
  message["data"][kf1.name]["out_v"]  = kf1.read_valve_feedback(); 
  
  serializeJsonPretty(message, Serial);

  delay(5000);
}