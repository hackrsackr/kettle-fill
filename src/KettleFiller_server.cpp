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
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

AsyncWebServer server(80);
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
String processor(const String&);
void notFound(AsyncWebServerRequest*);
StaticJsonDocument<4096> input;
String setpoint_input = String(_SETL1);
String enabled_input = "true";
String enableArmChecked = "";
const char *PARAM_INPUT_1 = "setpoint_input";
//const char *PARAM_INPUT_2 = "enable_arm_input";
const char *PARAM_INPUT_2 = "checkbox_input";

// HTML web page to handle 2 input fields (setpoint_input, enabled_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Volume Level Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  padding: 25px;
  background-color: black;
  color: grey;
  font-size: 25px;
}
</style>
</head>
<body>
<h4><strong>Kettle-1</strong></h4>
<h4>Setpoint %SETPOINT% liters</h4>
<h4>Volume %VOLUME% liters</h4>
<form action="/get">
  Enabled <input type="checkbox" name="checkbox_input" value="true" %CHECKBOX_INPUT%><br>
  Setpoint[L] <input type="number" step="0.5" name="setpoint_input" value="%SETPOINT%" required><br>
  <input type="submit" value="Submit">
</form>
</body></html>)rawliteral";
void onConnectionEstablished(void);
void publishData(void);

void setup() {
  Serial.begin(115200);
  pinMode(pv.pin, OUTPUT);
  digitalWrite(pv.pin, LOW);

  // ADS 
  ads.begin(0x48);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }

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
  
  // Server
  //Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  // Receive an HTTP GET request at <ESP_IP>/get?setpoint_input=<setpoint_input>&enable_arm_input=<enabled_input>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      setpoint_input = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        enabled_input = request->getParam(PARAM_INPUT_2)->value();
        enableArmChecked = "checked";
      }
      else {
        enabled_input = "false";
        enableArmChecked = "";
      }
    }
    kf.kf_enabled = enabled_input;
    Serial.println(setpoint_input);
    Serial.println(enabled_input);
  request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  StaticJsonDocument<1024> message;
  kf.run();
  if(enableArmChecked == "checked") {
    kf.kf_enabled = true;
  } else {
    kf.kf_enabled = false;
  }
  publishData();
  client.loop();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
  {
    Serial.println(payload);
    deserializeJson(input, payload);
    kf.liqr_liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
    kf.mash_liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
    kf.boil_liters = input["data"]["volume-sensors"]["output3-3"]["liters"];
    
    //StaticJsonDocument<200> doc;
    //doc["liqr"] = kf.liqr_liters; 
    //doc["mash"] = kf.mash_liters; 
    //doc["boil"] = kf.boil_liters;
    //serializeJson(doc, Serial);
    
    publishData(); });
}

void publishData()
{
  if (client.isConnected())
  {
    kf.run();
    StaticJsonDocument<768> message;
    //DynamicJsonDocument message(768);
    message["key"] = _CLIENTID;

    message["data"][kf.name]["enabled"] = kf.kf_enabled;
    message["data"][kf.name]["adc"] = vs.read_adc();
    message["data"][kf.name]["trim_adc"] = vs.trim_adc();
    message["data"][kf.name]["volts"] = vs.read_volts();
    message["data"][kf.name]["des_ls"] = kf.desired_liters;
    message["data"][kf.name]["liqr_liters"] = kf.liqr_liters;
    message["data"][kf.name]["mash_liters"] = kf.mash_liters;
    message["data"][kf.name]["boil_liters"] = kf.boil_liters;
    message["data"][kf.name]["vs_liters"] = vs.read_liters();
    message["data"][kf.name]["filled"] = kf.get_percent_full(vs.liters);
    message["data"][kf.name]["kf-pos"] = kf.get_pv_position(vs.liters);
    message["data"][kf.name]["pv-pos"] = pv.position;
  
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "VOLUME"){
    return String(vs.liters);
  }
  else if(var == "SETPOINT"){
    kf.desired_liters = setpoint_input.toFloat();
    return setpoint_input;
  }
  else if(var == "CHECKBOX_INPUT"){
    return enableArmChecked;
  }
  return String();
}