/**
 * @brief Prints the MAC address of the ESP board to the serial console.
 *
 * This function is called once during the setup phase of the Arduino sketch.
 * It initializes the serial communication at 115200 baud and then prints the
 * MAC address of the ESP board to the serial console.
 */
#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop()
{
}