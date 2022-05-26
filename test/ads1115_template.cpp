// The ADC input range (or gain) can be changed via the following
// functions, but be careful never to exceed VDD +0.3V max, or to
// exceed the upper and lower limits if you adjust the input range!
// Setting these values incorrectly may destroy your ADC!
//                                                                ADS1015  ADS1115
//                                                                -------  -------
// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
// 
// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
// Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>

Adafruit_ADS1115 ads;

void setup(void)
{
  Serial.begin(115200);
  ads.begin(0x48);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop(void)
{
  //DynamicJsonDocument message(256);
  StaticJsonDocument<384> message;
  
  message["key"] = "adc_test";
  
  message["data"]["ch1"]["adc"]   = ads.readADC_SingleEnded(0);
  message["data"]["ch1"]["volts"] = ads.computeVolts(ads.readADC_SingleEnded(0));
  message["data"]["ch2"]["adc"]   = ads.readADC_SingleEnded(1);
  message["data"]["ch2"]["volts"] = ads.computeVolts(ads.readADC_SingleEnded(1));
  message["data"]["ch3"]["adc"]   = ads.readADC_SingleEnded(2);
  message["data"]["ch3"]["volts"] = ads.computeVolts(ads.readADC_SingleEnded(2));
  message["data"]["ch4"]["adc"]   = ads.readADC_SingleEnded(3);
  message["data"]["ch4"]["volts"] = ads.computeVolts(ads.readADC_SingleEnded(3));
  
  //std::cout << measureJson(message) << std::endl;
  //std::cout << measureJsonPretty(message) << std::endl;
  
  //std::cout << std::endl;
  std::cout << message << std::endl;
  serializeJsonPretty(message, Serial);
  std::cout << std::endl;
  
  delay(5000);
}
