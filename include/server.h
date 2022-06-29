#pragma once

#include <iostream>
#include <Arduino.h>

#include "config.h"

KettleFiller kf1(_KFNM1, _SETL1);                 // name, desired_liters
KettleFiller kf2(_KFNM2, _SETL2);                 // name, desired_liters
KettleFiller kf3(_KFNM3, _SETL3);                 // name, desired_liters
std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr = {kf1, kf2, kf3};
//std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr;

AsyncWebServer server(80);
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

String setpoint_input1 = String(_SETL1);
String setpoint_input2 = String(_SETL2);
String setpoint_input3 = String(_SETL3);
std::array<String, _NUMBER_OF_KETTLES> setInputs = {setpoint_input1, setpoint_input2, setpoint_input3};

String enabled_input1 = "true";
String enabled_input2 = "true";
String enabled_input3 = "true";
std::array<String, _NUMBER_OF_KETTLES> enInputs = {enabled_input1, enabled_input2, enabled_input3};

String enableArmChecked1 = "";
String enableArmChecked2 = "";
String enableArmChecked3 = "";
std::array<String, _NUMBER_OF_KETTLES> enChecked = {enableArmChecked1, enableArmChecked2, enableArmChecked3};

const char *SET_INPUT_1 = "setpoint_input1";
const char *SET_INPUT_2 = "setpoint_input2";
const char *SET_INPUT_3 = "setpoint_input3";

const char *CBX_INPUT_1 = "checkbox_input1";
const char *CBX_INPUT_2 = "checkbox_input2";
const char *CBX_INPUT_3 = "checkbox_input3";

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

String processor(const String& var){
  //Serial.println(var);
  if(var == "VOLUME1") { return String(kf_arr[0].liters); }
  if(var == "VOLUME2") { return String(kf_arr[1].liters); }
  if(var == "VOLUME3") {return String(kf_arr[2].liters); }
  if(var == "CHECKBOX_INPUT1") { return enableArmChecked1; }
  if(var == "CHECKBOX_INPUT2") { return enableArmChecked2; }
  if(var == "CHECKBOX_INPUT3") { return enableArmChecked3; }
  if(var == "SETPOINT1") {
    kf_arr[0].desired_liters = setpoint_input1.toFloat();
    return setpoint_input1;
  }
  if(var == "SETPOINT2") {
    kf_arr[1].desired_liters = setpoint_input2.toFloat();
    return setpoint_input2;
  }
  if(var == "SETPOINT3") {
    kf_arr[2].desired_liters = setpoint_input3.toFloat();
    return setpoint_input3;
  }
  
  return String();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void server_setup() {
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
      setpoint_input3 = request->getParam(SET_INPUT_3)->value();
      inputMessage = setpoint_input3; 
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
    Serial.println(inputParam);
    request->send(200, "text/html", "HTTP GET request sent. {enabled: " + inputParam + ", Setpoint: " + inputMessage + " }"
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
  server.onNotFound(notFound);
  server.begin();
} 

