#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EspMQTTClient.h>
#include <Wire.h>
#include "VolumeSensor/VolumeSensor.hpp"
#include "KettleFiller/KettleFiller.hpp"
#include "KettleFiller/KettleFiller.cpp"
#include "config.h"

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = _SSID;
const char* password = _PASS;


// Default Threshold Temperature Value
String inputMessage = "25.0";
String lastVolume;
String enableArmChecked = "checked";
String inputMessage2 = "true";

// HTML web page to handle 2 input fields (threshold_input, enable_arm_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Volume Level Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h2>Volume</h2> 
  <h3>%VOLUME% liters</h3>
  <h2>ESP Arm Trigger</h2>
  <form action="/get">
    Desired volume in Liters <input type="number" step="0.1" name="threshold_input" value="%THRESHOLD%" required><br>
    Arm Trigger <input type="checkbox" name="enable_arm_input" value="true" %ENABLE_ARM_INPUT%><br><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "VOLUME"){
    return lastVolume;
  }
  else if(var == "THRESHOLD"){
    return inputMessage;
  }
  else if(var == "ENABLE_ARM_INPUT"){
    return enableArmChecked;
  }
  return String();
}

// Flag variable to keep track if triggers was activated or not
bool triggerActive = false;

const char* PARAM_INPUT_1 = "threshold_input";
const char* PARAM_INPUT_2 = "enable_arm_input";

// Interval between sensor readings. Learn more about ESP32 timers: https://RandomNerdTutorials.com/esp32-pir-motion-sensor-interrupts-timers/
unsigned long previousMillis = 0;     
const long interval = 5000;    

// GPIO where the output is connected to
const int output = 2;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Receive an HTTP GET request at <ESP_IP>/get?threshold_input=<inputMessage>&enable_arm_input=<inputMessage2>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
      if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
        enableArmChecked = "checked";
      }
      else {
        inputMessage2 = "false";
        enableArmChecked = "";
      }
    }
    Serial.println(inputMessage);
    Serial.println(inputMessage2);
    request->send(200, "text/html", "HTTP GET request sent to your ESP.<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    Serial.println(" Liters");
    
    // Temperature in Fahrenheit degrees
    /*float temperature = sensors.getTempFByIndex(0);
    Serial.print(temperature);
    Serial.println(" *F");*/
    
    lastVolume = String(vs.liters);
    
    // Check if temperature is above threshold and if it needs to trigger output
    if(vs.liters < inputMessage.toFloat() && inputMessage2 == "true" && !triggerActive){
      String message = String("Volume above threshold. Current volume: ") + 
                            String(vs.liters) + String("L");
      Serial.println(message);
      triggerActive = true;
      digitalWrite(output, HIGH);
    }
    // Check if temperature is below threshold and if it needs to trigger output
    else if((vs.liters < inputMessage.toFloat()) && inputMessage2 == "true" && triggerActive) {
      String message = String("Volume below setting. Current volume: ") + 
                            String(vs.liters) + String(" L");
      Serial.println(message);
      triggerActive = false;
      digitalWrite(output, LOW);
    }
  }
}