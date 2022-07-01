#include <iostream>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>

//#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "KettleFiller/KettleFiller.hpp"
#include "config.h"
#include "server.h"

std::array<VolumeSensor, _NUMBER_OF_KETTLES> vs_arr = {
    VolumeSensor(_VSNM1, _CHAN1, _OFFS1),
    VolumeSensor(_VSNM2, _CHAN2, _OFFS2),
    VolumeSensor(_VSNM3, _CHAN3, _OFFS3)};

// StaticJsonDocument<2096> input;
// void onConnectionEstablished(void);
void publishData(void);

void setup()
{
  Serial.begin(115200);

  // client.setMaxPacketSize(4096);
  // client.enableOTA();

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
  // kf_arr[0].kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  // kf_arr[1].kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  // kf_arr[2].kf_enabled = (enableArmChecked3 == "checked") ? true : false;
  // client.loop();
  publishData();
}

// void onConnectionEstablished()
// {
//   client.subscribe(_SUBTOPIC, [](const String &payload)
//   {
//     deserializeJson(input, payload);
//     kf_arr[0].liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
//     kf_arr[1].liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
//     kf_arr[2].liters = vs_arr[2].read_liters();

//     publishData();
//     });
// }

void publishData()
{
  // if (client.isConnected())
  // {
  StaticJsonDocument<768> message;
  message["key"] = _CLIENTID;

  for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++)
  {
    kf_arr[i].vs_ptr = &vs_arr[i];
    kf_arr[i].liters = kf_arr[i].vs_ptr->read_liters();
    // kf_arr[i].liters = vs_arr[i].vs_ptr->liters;
    message["data"][kf_arr[i].name]["vs_name"] = kf_arr[i].vs_ptr->name;
    message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
    message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
    message["data"][kf_arr[i].name]["liters"] = kf_arr[i].liters;
    // message["data"][kf_arr[i].name]["gallons"] = vs_arr[i].read_gallons();
  }
  serializeJsonPretty(message, Serial);
  client.publish(_PUBTOPIC, message.as<String>());
  delay(5000);
}
