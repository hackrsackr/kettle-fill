#include <iostream>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
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

String processor(const String&);
void notFound(AsyncWebServerRequest*);

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
<p><strong>KettleFiller</strong></p>
<form action="/get">
  Enabled <input type="checkbox" name="checkbox_input" value="true" %CHECKBOX_INPUT%><br>
</form>
<h4>Setpoint %SETPOINT% liters</h4>
<h4>Volume %VOLUME% liters</h4>
<form action="/get">
  Setpoint[L] <input type="number" step="0.5" name="setpoint_input" value="%SETPOINT%" required><br>
  <input type="submit" value="Submit">
</form>
</body></html>)rawliteral";

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
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  kf.print_data();
  if(enableArmChecked == "checked") {
    kf.kf_enabled = true;
  } else {
    kf.kf_enabled = false;
  }
  kf.run();
  delay(5000);
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