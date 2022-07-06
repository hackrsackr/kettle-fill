#include <iostream>
#include <array>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <EspMQTTClient.h>

#include "config.h"
#include "server.h"

StaticJsonDocument<2096> input;
void onConnectionEstablished(void);
void publishData(void);

void setup()
{
  Serial.begin(115200);

  // WiFi
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

  // Web server
  server_setup();
  client.setMaxPacketSize(4096);
  client.enableOTA();
  
  // ADS1115
  ads.begin(0x48);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  
  // KettleFiller->ptrVolumeSensor 
  for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++)
  {
    kf_arr[i].vs_ptr = &vs_arr[i];
  }
}

void loop()
{
  kf_arr[0].kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  kf_arr[1].kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  kf_arr[2].kf_enabled = (enableArmChecked3 == "checked") ? true : false;
  pv.pv_enabled = (enableArmChecked4 == "checked") ? true : false;

  publishData();
  client.loop();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
                   {
    deserializeJson(input, payload);
    kf_arr[0].drain_valve_state = input["data"][liqrDrain_valve]["state"];
    kf_arr[0].return_valve_state = input["data"][liqrReturn_valve]["state"];
    kf_arr[1].drain_valve_state = input["data"][mashDrain_valve]["state"];
    kf_arr[1].return_valve_state = input["data"][mashReturn_valve]["state"];
    kf_arr[2].drain_valve_state = input["data"][boilDrain_valve]["state"];
    kf_arr[2].return_valve_state = input["data"][boilReturn_valve]["state"];
    publishData(); });
}

void publishData()
{
  if (client.isConnected())
  {
    StaticJsonDocument<2048> message;
    message["key"] = _CLIENTID;
    for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++)
    {
      message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
      message["data"][kf_arr[i].name]["Volume"]["vs_name"] = kf_arr[i].vs_ptr->name;
      message["data"][kf_arr[i].name]["Volume"]["liters"] = kf_arr[i].vs_ptr->read_liters();
      message["data"][kf_arr[i].name]["Volume"]["gallons"] = kf_arr[i].vs_ptr->read_gallons();
      message["data"][kf_arr[i].name]["Volume"]["desired liters"] = kf_arr[i].desired_liters;
      message["data"][kf_arr[i].name]["Volume"]["fill %"] = kf_arr[i].get_percent_full();
      message["data"][kf_arr[i].name]["Valves"]["drain valve state"] = kf_arr[i].drain_valve_state;
      message["data"][kf_arr[i].name]["Valves"]["return valve state"] = kf_arr[i].return_valve_state;
      message["data"][kf_arr[i].name]["Valves"]["PropValve"]["enabled"] = pv.pv_enabled;
      message["data"][kf_arr[i].name]["Valves"]["PropValve"]["position"] = pv.position;
      message["data"][kf_arr[i].name]["Valves"]["PropValve"]["setpoint"] = kf_arr[i].get_pv_position();
    }
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}