#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

//24:DC:C3:98:B0:34

void loop()
{
}