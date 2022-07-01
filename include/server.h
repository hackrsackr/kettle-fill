#pragma once

//#include <iostream>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EspMQTTClient.h>
#include <ESP32HTTPUpdateServer.h>
#include <WiFi.h>
#include "KettleFiller/KettleFiller.hpp"

#include "config.h"

AsyncWebServer server(80);
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);

String setpoint_input1 = String(_SETL1);
String setpoint_input2 = String(_SETL2);
String setpoint_input3 = String(_SETL3);
String setpoint_input4 = String(_VPOS);
std::array<String, _NUMBER_OF_KETTLES> setInputs = {setpoint_input1, setpoint_input2, setpoint_input3};

String enabled_input1 = "true";
String enabled_input2 = "true";
String enabled_input3 = "true";
String enabled_input4 = "true";
std::array<String, _NUMBER_OF_KETTLES> enInputs = {enabled_input1, enabled_input2, enabled_input3};

String enableArmChecked1 = "";
String enableArmChecked2 = "";
String enableArmChecked3 = "";
String enableArmChecked4 = "";
std::array<String, _NUMBER_OF_KETTLES> enChecked = {enableArmChecked1, enableArmChecked2, enableArmChecked3};

const char *SET_INPUT_1 = "setpoint_input1";
const char *SET_INPUT_2 = "setpoint_input2";
const char *SET_INPUT_3 = "setpoint_input3";
const char *SET_INPUT_4 = "setpoint_input4";
std::array<std::string, _NUMBER_OF_KETTLES> 
SET_INPUTS = {enableArmChecked1.c_str(), enableArmChecked2.c_str(), enableArmChecked3.c_str()};

const char *CBX_INPUT_1 = "checkbox_input1";
const char *CBX_INPUT_2 = "checkbox_input2";
const char *CBX_INPUT_3 = "checkbox_input3";
const char *CBX_INPUT_4 = "checkbox_input4";
std::array<std::string, _NUMBER_OF_KETTLES> 
CBX_INPUTS = {enableArmChecked1.c_str(), enableArmChecked2.c_str(), enableArmChecked3.c_str()};

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
  Liqr Volume: %LIQR_VOL% [L]<br>
  <input type="checkbox" name="checkbox_input1" value="true" %CBX_INPUT1%> Setpoint Enabled<br>
  Setpoint: %LIQR_SP% [L]<br>
  <input type="number" step="0.5" name="setpoint_input1" value="%LIQR_SP%" required>
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  Mash Volume: %MASH_VOL% [L]<br>
  <input type="checkbox" name="checkbox_input2" value="true" %CBX_INPUT2%> Setpoint Enabled<br>
  Setpoint: %MASH_SP% [L]<br> 
  <input type="number" step="0.5" name="setpoint_input2" value="%MASH_SP%" required>
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  Boil Volume: %BOIL_VOL% [L]<br> 
  <input type="checkbox" name="checkbox_input3" value="true" %CBX_INPUT3%> Setpoint Enabled<br>
  Setpoint: %BOIL_SP% [L]<br> 
  <input type="number" step="0.5" name="setpoint_input3" value="%BOIL_SP%" required>
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  Pv position: %PV_POS%<br> 
  <input type="checkbox" name="checkbox_input4" value="true" %CBX_INPUT4%> PropValve Enabled<br>
  New Position: %NEW_POS%<br> 
  <input type="number" step="5" name="setpoint_input4" value="%NEW_POS%" required>
  <input type="submit" value="Submit">
</form><br>
</body></html>)rawliteral";

String processor(const String& var){
  if(var == "LIQR_VOL") { return String(kf_arr[0].liters); }
  if(var == "MASH_VOL") { return String(kf_arr[1].liters); }
  if(var == "BOIL_VOL") {return String(kf_arr[2].liters); }
  if(var == "PV_POS") {return String(pv.position); }
  if(var == "CBX_INPUT1") { return enableArmChecked1; }
  if(var == "CBX_INPUT2") { return enableArmChecked2; }
  if(var == "CBX_INPUT3") { return enableArmChecked3; }
  if(var == "CBX_INPUT4") { return enableArmChecked4; }
  if(var == "LIQR_SP") {
    kf_arr[0].desired_liters = setpoint_input1.toFloat();
    return setpoint_input1;
  }
  if(var == "MASH_SP") {
    kf_arr[1].desired_liters = setpoint_input2.toFloat();
    return setpoint_input2;
  }
  if(var == "BOIL_SP") {
    kf_arr[2].desired_liters = setpoint_input3.toFloat();
    return setpoint_input3;
  }
  if(var == "NEW_POS") {
    if (enableArmChecked4 == "checked") { 
      pv.set_position(setpoint_input4.toInt());
      return setpoint_input4;
    } else { std::cout << "!pv_enabled" << "\n"; }
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
      kf_arr[0].kf_enabled = enabled_input1;
      inputParam = enabled_input1;
    }
    if (request->hasParam(SET_INPUT_2)) {
      setpoint_input2 = request->getParam(SET_INPUT_2)->value();
      inputMessage = setpoint_input2; 
      if (request->hasParam(CBX_INPUT_2)) {
        enabled_input2 = request->getParam(CBX_INPUT_2)->value();
        enableArmChecked2 = "checked";
      } else {
        enabled_input2 = "false";
        enableArmChecked2 = "";
      }
      kf_arr[1].kf_enabled = enabled_input2;
      inputParam = enabled_input2;
    }
    if (request->hasParam(SET_INPUT_3)) {
      setpoint_input3 = request->getParam(SET_INPUT_3)->value();
      inputMessage = setpoint_input3; 
      if (request->hasParam(CBX_INPUT_3)) {
        enabled_input3 = request->getParam(CBX_INPUT_3)->value();
        enableArmChecked3 = "checked";
      } else {
        enabled_input3 = "false";
        enableArmChecked3 = "";
      }
      kf_arr[2].kf_enabled = enabled_input3;
      inputParam = enabled_input3;
    }
    if (request->hasParam(SET_INPUT_4)) {
      setpoint_input4 = request->getParam(SET_INPUT_4)->value();
      inputMessage = setpoint_input4; 
      if (request->hasParam(CBX_INPUT_4)) {
        enabled_input4 = request->getParam(CBX_INPUT_4)->value();
        enableArmChecked4 = "checked";
      } else {
        enabled_input4 = "false";
        enableArmChecked4 = "";
      }
      pv.pv_enabled = enabled_input4;
      inputParam = enabled_input4;
    }
    Serial.println(inputMessage);
    Serial.println(inputParam);
    request->send(200, "text/html", "HTTP GET request sent. {enabled: " + inputParam + ", Setpoint: " + inputMessage + " }"
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
  server.onNotFound(notFound);
  server.begin();
} 

