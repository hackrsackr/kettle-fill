#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

Adafruit_ADS1115 ads;

void setup(void)
{
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  digitalWrite(26, 0);

}

void loop(void)
{
  digitalWrite(26, int(255/10));


}
