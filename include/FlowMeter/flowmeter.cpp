#include "ArduinoJson.h"

//#include "FlowMeter/flowmeter.hpp"
#include "flowmeter.hpp"
#include "config.hpp"

FlowMeter f1(_FLOW1, _SPIN1, _YFS402B);

void pulseCounter() { f1.pulse_count++; }

void setup()
{
    // Initialize a serial connection for reporting values to the host
    Serial.begin(115200);
    
    pinMode(f1.sensor_pin, INPUT_PULLUP);
    attachInterrupt(f1.sensor_pin, pulseCounter, FALLING);
}

void publish_data()
{
    StaticJsonDocument<200> message;
    message["key"] = "^^^";

    f1.flowmeter_run();
    message["data"][f1.name]["Flow rate[LPM]"] = f1.flow_rate;
    message["data"][f1.name]["Total[mLs]"] = f1.total_mLs;
    attachInterrupt(f1.sensor_pin, pulseCounter, FALLING);
    
    serializeJsonPretty(message, Serial);
    delay(5000);
}

void loop()
{
    publish_data();
}
