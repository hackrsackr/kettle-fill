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
//#include "PropValve/PropValve.hpp"
//#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"


KettleFiller kf1(_KFNM1, _SETL1);                 // name, desired_liters
KettleFiller kf2(_KFNM2, _SETL2);                 // name, desired_liters
KettleFiller kf3(_KFNM3, _SETL3);                 // name, desired_liters
std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr = {kf1, kf2, kf3};

String setpoint_input1 = String(_SETL1);
String setpoint_input2 = String(_SETL2);
String setpoint_input3 = String(_SETL3);

String enabled_input1 = "true";
String enabled_input2 = "true";
String enabled_input3 = "true";

String enableArmChecked1 = "";
String enableArmChecked2 = "";
String enableArmChecked3 = "";

const char *SET_INPUT_1 = "setpoint_input1";
const char *SET_INPUT_2 = "setpoint_input2";
const char *SET_INPUT_3 = "setpoint_input3";

const char *CBX_INPUT_1 = "checkbox_input1";
const char *CBX_INPUT_2 = "checkbox_input2";
const char *CBX_INPUT_3 = "checkbox_input3";

AsyncWebServer server(80);
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
StaticJsonDocument<2096> input;

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
</style>
</head>
<body>
<form action="/get">
  Liqr Volume: %VOLUME1% [L]<br>
  <input type="checkbox" name="checkbox_input1" value="true" %CHECKBOX_INPUT1%> Setpoint Enabled<br>
  Setpoint: %SETPOINT1% [L]<br>
  <input type="number" step="0.5" name="setpoint_input1" value="%SETPOINT1%" required>
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  Mash Volume: %VOLUME2% [L]<br>
  <input type="checkbox" name="checkbox_input2" value="true" %CHECKBOX_INPUT2%> Setpoint Enabled<br>
  Setpoint: %SETPOINT2% [L]<br> 
  <input type="number" step="0.5" name="setpoint_input2" value="%SETPOINT2%" required>
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  Boil Volume: %VOLUME3% [L]<br> 
  <input type="checkbox" name="checkbox_input3" value="true" %CHECKBOX_INPUT3%> Setpoint Enabled<br>
  Setpoint: %SETPOINT3% [L]<br> 
  <input type="number" step="0.5" name="setpoint_input3" value="%SETPOINT3%" required>
  <input type="submit" value="Submit">
</form><br>
</body></html>)rawliteral";

String processor(const String&);
void notFound(AsyncWebServerRequest*);
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
  
  // Server - Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  // Input Handler
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(SET_INPUT_1)) {
      setpoint_input1 = request->getParam(SET_INPUT_1)->value();
      inputMessage = setpoint_input1; 
      if (request->hasParam(CBX_INPUT_1)) {
        enabled_input1 = request->getParam(CBX_INPUT_1)->value();
        enableArmChecked1 = "checked";
      } else {
        enabled_input1 = "false";
        enableArmChecked1 = "";
      }
      kf1.kf_enabled = enabled_input1;
      inputParam = enabled_input1;
    }
    else if (request->hasParam(SET_INPUT_2)) {
      setpoint_input2 = request->getParam(SET_INPUT_2)->value();
      inputMessage = setpoint_input2; 
      if (request->hasParam(CBX_INPUT_2)) {
        enabled_input2 = request->getParam(CBX_INPUT_2)->value();
        enableArmChecked2 = "checked";
      } else {
        enabled_input2 = "false";
        enableArmChecked2 = "";
      }
      kf2.kf_enabled = enabled_input2;
      inputParam = enabled_input2;
    }
    else if (request->hasParam(SET_INPUT_3)) {
      inputMessage = setpoint_input3; 
      setpoint_input3 = request->getParam(SET_INPUT_3)->value();
      if (request->hasParam(CBX_INPUT_3)) {
        enabled_input3 = request->getParam(CBX_INPUT_3)->value();
        enableArmChecked3 = "checked";
      } else {
        enabled_input3 = "false";
        enableArmChecked3 = "";
      }
      kf3.kf_enabled = enabled_input3;
      inputParam = enabled_input3;
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent. {enabled: " + inputParam + ", Setpoint: " + inputMessage + " }"
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  StaticJsonDocument<1024> message;
  kf1.kf_enabled = (enableArmChecked1 == "checked") ? true : false;
  kf2.kf_enabled = (enableArmChecked2 == "checked") ? true : false;
  kf3.kf_enabled = (enableArmChecked3 == "checked") ? true : false;

  publishData();
  client.loop();
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
  {
    std::cout << payload << std::endl;
    deserializeJson(input, payload);
    kf1.liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
    kf2.liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
    kf3.liters = input["data"]["volume-sensors"]["output3-3"]["liters"];
    
    publishData(); });
}

void publishData()
{
  if (client.isConnected())
  {
    //kf.get_liters();
    StaticJsonDocument<768> message;
    //DynamicJsonDocument message(768);
    message["key"] = _CLIENTID;

    message["data"][kf1.name]["enabled"] = kf1.kf_enabled;
    message["data"][kf1.name]["setpoint"] = kf1.desired_liters;
    message["data"][kf1.name]["liters"] = kf1.liters;
    //message["data"][kf1.name]["delta_Ls"] = kf1.desired_liters - kf1.liters;
    
    message["data"][kf2.name]["enabled"] = kf2.kf_enabled;
    message["data"][kf2.name]["setpoint"] = kf2.desired_liters;
    message["data"][kf2.name]["liters"] = kf2.liters;
    //message["data"][kf2.name]["delta_Ls"] = kf2.desired_liters - kf2.liters;
    
    message["data"][kf3.name]["enabled"] = kf3.kf_enabled;
    message["data"][kf3.name]["setpoint"] = kf3.desired_liters;
    message["data"][kf3.name]["liters"] = kf3.liters;
    //message["data"][kf3.name]["delta_Ls"] = kf3.desired_liters - kf3.liters;
    //message["data"][kf.name]["adc"] = vs.read_adc();
    //message["data"][kf.name]["trim_adc"] = vs.trim_adc();
    //message["data"][kf.name]["volts"] = vs.read_volts();
    //message["data"][kf.name]["mash_liters"] = kf.mash_liters;
    //message["data"][kf.name]["boil_liters"] = kf.boil_liters;
    //message["data"][kf.name]["kf_liters"] = kf.liters;
    //message["data"][kf.name]["vs_liters"] = vs.read_liters();
    //message["data"][kf.name]["filled"] = kf.get_percent_full(kf.liters);
    //message["data"][kf.name]["kf-pos"] = kf.get_pv_position(kf.liters);
    //message["data"][kf.name]["pv-pos"] = pv.position;
  
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
  if(var == "VOLUME1") { return String(kf1.liters); }
  else if(var == "SETPOINT1") {
    kf1.desired_liters = setpoint_input1.toFloat();
    return setpoint_input1;
  }
  else if(var == "CHECKBOX_INPUT1") { return enableArmChecked1; }
  
  else if(var == "VOLUME2") { return String(kf2.liters); }
  else if(var == "SETPOINT2"){
    kf2.desired_liters = setpoint_input2.toFloat();
    return setpoint_input2;
  }
  else if(var == "CHECKBOX_INPUT2"){ return enableArmChecked2; }
  
  else if(var == "VOLUME3") {return String(kf3.liters); }
  else if(var == "SETPOINT3") {
    kf3.desired_liters = setpoint_input3.toFloat();
    return setpoint_input3;
  }
  else if(var == "CHECKBOX_INPUT3") { return enableArmChecked3; }
  
  return String();
}