#include <array>
#include "kettle_fill.hpp"
#include "kettle_fill_config.hpp"

uint16_t duty_cycle;
const int freq = 5000;
const int adc_channel = 2;
const int pwm_channel = 4;
const int resolution  = 12;
const int MAX_DUTY = (int)(pow(2, resolution) - 1);
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
/*
* LAB: 5
* Name: ESP32 PWM LED Control
* Author: Khaled Magdy
* For More Info Visit: www.DeepBlueMbedded.com
*/
 
#define LED_GPIO   5
#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000
 
int PWM1_DutyCycle = 0;
 
void setup()
{
  ledcAttachPin(LED_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
}
 
void loop()
{
  while(PWM1_DutyCycle < 255)
  {
    ledcWrite(PWM1_Ch, PWM1_DutyCycle++);
    delay(10);
  }
  while(PWM1_DutyCycle > 0)
  {
    ledcWrite(PWM1_Ch, PWM1_DutyCycle--);
    delay(10);
  }
}

void setup(void)
{
  Serial.begin(115200);

  pinMode(kf1.valve_pin, OUTPUT);
  digitalWrite(kf1.valve_pin, 255);
}

void loop(void)
{  

  //duty_cycle = analogRead(adc_channel);
  //Serial.println(duty_cycle);
  //duty_cycle = map(duty_cycle, 0, resolution, 0, MAX_DUTY);
  //digitalWrite(pwm_channel, duty_cycle);

  float valve_volts = analogReadMilliVolts(2) / 1000.0;
  
  Serial.println(valve_volts);
  
  digitalWrite(kf1.valve_pin, 255);
  delay(5000);
}