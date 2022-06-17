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

String processor(const String &);
String setpoint_input = String(_SETL1);
//String lastTemperature;
String enabled_input = "true";
String enableArmChecked = "checked";
const char *PARAM_INPUT_1 = "setpoint_input";
const char *PARAM_INPUT_2 = "enable_arm_input";

// HTML web page to handle 2 input fields (threshold_input, enable_arm_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Volume Level Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Enabled <input type="checkbox" name="enable_arm_input" value="true" %ENABLE_ARM_INPUT%><br><br>
    Setpoint[L] <input type="number" step="0.5" name="setpoint_input" value="%SETPOINT%" required><br>
    <input type="submit" value="Submit">
  </form>
  <h3>Setpoint %SETPOINT% liters</h3>
  <h3>Volume %VOLUME% liters</h3>
</body></html>)rawliteral";

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
  else if(var == "ENABLE_ARM_INPUT"){
    return enableArmChecked;
  }
  return String();
}

// Flag variable to keep track if triggers was activated or not
bool triggerActive = false;

void setup() {
  Serial.begin(115200);
  // WiFi
  WiFi.disconnect(true);
  //ESP.restart();
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
  ads.begin(0x48);
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  
  pinMode(pv.pin, OUTPUT);
  digitalWrite(pv.pin, LOW);
  kf.print_data(); 
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
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
  if(enableArmChecked == "checked") {
    kf.kf_enabled = true;
  } else {
    kf.kf_enabled = false;
  }
  kf.run();
  delay(5000);
}