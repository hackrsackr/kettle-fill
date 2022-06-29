#include <iostream>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <Adafruit_ADS1X15.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

#include "KettleFiller/KettleFiller.hpp"
#include "KettleFiller/KettleFiller.cpp"
#include "PropValve/PropValve.hpp"
//#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"
#include "server.h"

StaticJsonDocument<2096> input;
void onConnectionEstablished(void);
void publishData(void);

void setup() {
  Serial.begin(115200);

  client.setMaxPacketSize(4096);
  client.enableOTA(); 

  // WiFi
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(_SSID, _PASS);
  uint8_t failed_connections = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("connecting..");
    failed_connections++;
    if (failed_connections > 20)
    {
      Serial.println("restarting..");
      ESP.restart();
    }
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());

 server_setup();
}

void loop() {
  kf_arr[0].kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  kf_arr[1].kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  kf_arr[2].kf_enabled = (enableArmChecked3 == "checked") ? true : false;
  
  client.loop();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
  {
    deserializeJson(input, payload);
    kf_arr[0].liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
    kf_arr[1].liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
    kf_arr[2].liters = input["data"]["volume-sensors"]["output3-3"]["liters"];
    
    publishData(); 
    });
}

void publishData()
{
  if (client.isConnected())
  {
    StaticJsonDocument<768> message;

    for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++) 
    {
      message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
      message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
      message["data"][kf_arr[i].name]["liters"] = kf_arr[i].liters;
    }
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}



#include <iostream>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <Adafruit_ADS1X15.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

#include "KettleFiller/KettleFiller.hpp"
#include "KettleFiller/KettleFiller.cpp"
#include "PropValve/PropValve.hpp"
//#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"
#include "server.h"

StaticJsonDocument<2096> input;
void onConnectionEstablished(void);
void publishData(void);

void setup() {
  Serial.begin(115200);

  client.setMaxPacketSize(4096);
  client.enableOTA(); 

  // WiFi
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(_SSID, _PASS);
  uint8_t failed_connections = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("connecting..");
    failed_connections++;
    if (failed_connections > 20)
    {
      Serial.println("restarting..");
      ESP.restart();
    }
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.localIP());

 server_setup();
}

void loop() {
  kf_arr[0].kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  kf_arr[1].kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  kf_arr[2].kf_enabled = (enableArmChecked3 == "checked") ? true : false;
  
  client.loop();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
  {
    deserializeJson(input, payload);
    kf_arr[0].liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
    kf_arr[1].liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
    kf_arr[2].liters = input["data"]["volume-sensors"]["output3-3"]["liters"];
    
    publishData(); 
    });
}

void publishData()
{
  if (client.isConnected())
  {
    StaticJsonDocument<768> message;

    for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++) 
    {
      message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
      message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
      message["data"][kf_arr[i].name]["liters"] = kf_arr[i].liters;
    }
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}



