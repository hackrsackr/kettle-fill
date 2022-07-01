#include <iostream>
#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <array>

#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "KettleFiller/KettleFiller.hpp"
#include "config.h"
#include "server.h"
//#include "server_dev.h"

std::array<VolumeSensor, _NUMBER_OF_KETTLES> vs_arr = {
    VolumeSensor(_VSNM1, _CHAN1, _OFFS1),
    VolumeSensor(_VSNM2, _CHAN2, _OFFS2),
    VolumeSensor(_VSNM3, _CHAN3, _OFFS3)};

StaticJsonDocument<2096> input;
void onConnectionEstablished(void);
void publishData(void);

void setup()
{
  Serial.begin(115200);
  ads.begin(0x48);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
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

void loop()
{
  kf_arr[0].kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  kf_arr[1].kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  kf_arr[2].kf_enabled = (enableArmChecked3 == "checked") ? true : false;
  pv.pv_enabled = (enableArmChecked4 == "checked") ? true : false;

  client.loop();
  publishData();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
                   {
    deserializeJson(input, payload);
    
    publishData(); });
}

void publishData()
{
  if (client.isConnected())
  {
    StaticJsonDocument<768> message;
    message["key"] = _CLIENTID;
    for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++)
    {
      kf_arr[i].vs_ptr = &vs_arr[i];
      kf_arr[i].pv_ptr = &pv;
      kf_arr[i].pv_ptr->set_position(0);
      message["data"][kf_arr[i].name]["kf_name"] = kf_arr[i].name;
      message["data"][kf_arr[i].name]["vs_name"] = kf_arr[i].vs_ptr->name;
      message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
      message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
      message["data"][kf_arr[i].name]["liters"] = kf_arr[i].vs_ptr->read_liters();
      message["data"][kf_arr[i].name]["gallons"] = kf_arr[i].vs_ptr->read_gallons();
      message["data"][kf_arr[i].name]["pv_enabled"] = kf_arr[i].pv_ptr->pv_enabled;
      message["data"][kf_arr[i].name]["pv_pos"] = kf_arr[i].pv_ptr->position;
    }
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}
