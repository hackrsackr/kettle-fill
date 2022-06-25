#include <iostream>

#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <Adafruit_ADS1X15.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <array>

#include "KettleFiller/KettleFiller.hpp"
#include "PropValve/PropValve.hpp"
#include "VolumeSensor/VolumeSensor.hpp"
#include "config.h"

KettleFiller kf1(_KFNM1, _SETL1);
KettleFiller kf2(_KFNM2, _SETL2);
KettleFiller kf3(_KFNM3, _SETL3);
std::array<KettleFiller, 3> kf_arr = {kf1, kf2, kf3};
String inputMessage1 = String(_SETL1);
String inputMessage2 = String(_SETL2);
String inputMessage3 = String(_SETL3);
String checkbox_input1 = "true";
String checkbox_input2 = "true";
String checkbox_input3 = "true";
String enableArmChecked1 = "";
String enableArmChecked2 = "";
String enableArmChecked3 = "";
const char *SET_INPUT_1 = "setpoint_input1";
const char *SET_INPUT_2 = "setpoint_input2";
const char *SET_INPUT_3 = "setpoint_input3";
const char *CBX_INPUT_1 = "checkbox_input1";
const char *CBX_INPUT_2 = "checkbox_input2";
const char *CBX_INPUT_3 = "checkbox_input3";
//VolumeSensor vs1(_VSNM1, _CHAN1, _OFFS1);
//VolumeSensor vs2(_VSNM2, _CHAN2. _OFFS2);
//VolumeSensor vs3(_VSNM3, _CHAN3, _OFFS3);
//std::array<VolumeSensor, 3> vs_arr = {vs1, vs2, vs3};

AsyncWebServer server(80);
EspMQTTClient client(_SSID, _PASS, _MQTTHOST, _CLIENTID, _MQTTPORT);
String processor(const String&);
void notFound(AsyncWebServerRequest*);
StaticJsonDocument<4096> input;
//};                 // name, desired_liters

//std::array<KettleFiller kf1(_KFNM1, _SETL1);                 // name, desired_liters
//std::array<KettleFiller, _NUMBER_OF_KETTLES> kf_arr;

// HTML web page to handle 2 input fields (setpoint_input, enabled_input)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
<title>KettleFillers</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body {
    padding: 25px;
    background-color: black;
    color: grey;
    font-size: 25px;
  }
</style></head>
<body>
<form action="/get">
  Enabled <input type="checkbox" name="checkbox_input1" value="true" %CHECKBOX_INPUT1%><br>
  Liqr Setpoint[L] <input type="number" step="0.5" name="inputMessage1" value="%SETPOINT1%" required><br>
  <input type="submit" value="Submit">
</form>
<form action="/get">
  Enabled <input type="checkbox" name="checkbox_input2 value="true" %CHECKBOX_INPUT2%><br>
  Mash Setpoint[L] <input type="number" step="0.5" name="inputMessage2" value="%SETPOINT2%" required><br>
  <input type="submit" value="Submit">
</form>
<form action="/get">
  Enabled <input type="checkbox" name="checkbox_input3" value="true" %CHECKBOX_INPUT3%><br>
  Boil Setpoint[L] <input type="number" step="0.5" name="inputMessage3" value="%SETPOINT3%" required><br>
  <input type="submit" value="Submit">
</form>
</body></html>)rawliteral";



void onConnectionEstablished(void);
void publishData(void);


void setup() {
  Serial.begin(115200);
  //pinMode(pv.pin, OUTPUT);
  //digitalWrite(pv.pin, LOW);

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
  
  //for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++) {
  //  kf_arr[i] = KettleFiller(KF_NAMES[i], SETPOINTS[i]);
  //}

  // Receive an HTTP GET request at <ESP_IP>/get?setpoint_input=<setpoint_input>&enable_arm_input=<enabled_input>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(SET_INPUT_1)) {
      inputMessage1 = request->getParam(SET_INPUT_1)->value();
      kf1.desired_liters = inputMessage1.toFloat();
      inputMessage = inputMessage1;
      inputParam = SET_INPUT_1;
      if (request->hasParam(CBX_INPUT_1)) {
        checkbox_input1 = request->getParam(CBX_INPUT_1)->value();
        enableArmChecked1 = "checked";
      } else {
        checkbox_input1 = "false";
        enableArmChecked1 = "";
      }
    }
    else if (request->hasParam(SET_INPUT_2)) {
      inputMessage2 = request->getParam(SET_INPUT_2)->value();
      kf2.desired_liters = inputMessage2.toFloat();
      inputMessage = inputMessage2;
      inputParam = SET_INPUT_2;
      if (request->hasParam(CBX_INPUT_2)) {
        checkbox_input2 = request->getParam(CBX_INPUT_2)->value();
        enableArmChecked2 = "checked";
      } else {
        checkbox_input2 = "false";
        enableArmChecked2 = "";
      }
    }
    else if (request->hasParam(SET_INPUT_3)) {
      inputMessage3 = request->getParam(SET_INPUT_3)->value();
      kf3.desired_liters = inputMessage3.toFloat();
      inputMessage = inputMessage3;
      inputParam = SET_INPUT_3;
      if (request->hasParam(CBX_INPUT_3)) {
        checkbox_input3 = request->getParam(CBX_INPUT_3)->value();
        enableArmChecked3 = "checked";
      } else {
        checkbox_input3 = "false";
        enableArmChecked3 = "";
      }
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"); });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  StaticJsonDocument<1024> message;
  for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++) {
    //kf_arr[i].run();
    if(CHECKS[i] == "checked") { kf_arr[i].kf_enabled = true; } 
      else { kf_arr[i].kf_enabled = false; }
    publishData();
  client.loop();
  }
}

void onConnectionEstablished()
{
  client.subscribe(_SUBTOPIC, [](const String &payload)
  {
    std::cout << payload << std::endl;
    deserializeJson(input, payload);
    kf1.liqr_liters = input["data"]["volume-sensors"]["output3-1"]["liters"];
    kf2.mash_liters = input["data"]["volume-sensors"]["output3-2"]["liters"];
    kf3.boil_liters = input["data"]["volume-sensors"]["output3-3"]["liters"];
    
    publishData(); });
}

void publishData()
{
  StaticJsonDocument<1024> message;
  message["key"] = _CLIENTID;
  if (client.isConnected())
  {
    for (uint8_t i = 0; i < _NUMBER_OF_KETTLES; i++) 
    {
      //kf_arr[i].run();
      kf_arr[i].kf_enabled = (CHECKS[i] == "checked") ? "true" : "false"; 

      message["data"][kf_arr[i].name]["enabled"] = kf_arr[i].kf_enabled;
      message["data"][kf_arr[i].name]["setpoint"] = kf_arr[i].desired_liters;
      //message["data"][kf_arr[i].name]["volume"] = kf_arr[i].liters;
      //message["data"][kf_arr[i].name]["delta_Ls"] = kf_arr[i].desired_liters - kf_arr[i].liters;
      //message["data"][kf_arr[i].name]["adc"] = vs.read_adc();
      //message["data"][kf_arr[i].name]["trim_adc"] = vs.trim_adc();
      //message["data"][kf_arr[i].name]["volts"] = vs.read_volts();
      message["data"][kf_arr[i].name]["liqr_liters"] = kf1.liqr_liters;
      message["data"][kf_arr[i].name]["mash_liters"] = kf2.mash_liters;
      message["data"][kf_arr[i].name]["boil_liters"] = kf3.boil_liters;
      //message["data"][kf_arr[i].name]["volume"] = vs.read_liters();
      //message["data"][kf_arr[i].name]["filled"] = kf_arr[i].get_percent_full(kf_arr[i].liters);
      //message["data"][kf_arr[i].name]["kf-pos"] = kf_arr[i].get_pv_position(kf_arr[i].liters);
      //message["data"][kf_arr[i].name]["pv-pos"] = pv.position;
    }
    serializeJsonPretty(message, Serial);
    client.publish(_PUBTOPIC, message.as<String>());
    delay(5000);
  }
}


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String processor(const String &var)
{
  // Serial.println(var);
  if (var == "SETPOINT1")
  {
    _SETL1 = inputMessage1.toFloat();
    return inputMessage1;
  }
  else if (var == "SETPOINT2")
  {
    _SETL2 = inputMessage2.toFloat();
    return inputMessage2;
  }
  else if (var == "SETPOINT3")
  {
    _SETL3 = inputMessage3.toFloat();
    return inputMessage3;
  }
  return String();
}