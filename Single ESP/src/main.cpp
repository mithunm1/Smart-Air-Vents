/**
 * @file main.cpp
 * @brief Main entry point for the Smart Air Vents project.
 *
 * This file sets up the WiFi connection, starts the web server, and handles the
 * temperature and humidity readings from the DHT11 sensor. It also controls the
 * servo motor to adjust the air vent based on the average temperature.
 */

/**
 * @brief Handles the root URL request and generates the HTML response.
 *
 * This function is called when a client requests the root URL ("/"). It generates
 * an HTML page that displays the current temperature and humidity readings from
 * the DHT11 sensor.
 */
void handleRoot();

/**
 * @brief Reads the temperature from the DHT11 sensor.
 *
 * @return The current temperature in Celsius, or -1 if the sensor read fails.
 */
float readDHTTemperature();

/**
 * @brief Reads the humidity from the DHT11 sensor.
 *
 * @return The current humidity percentage, or -1 if the sensor read fails.
 */
float readDHTHumidity();
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define DHT_PIN 15   // DHT11 sensor data pin
#define SERVO_PIN 13 // Servo motor control pin

DHT dht(DHT_PIN, DHT11);
Servo myServo;
WebServer server(80);

const char *ssid = "Mithun";
const char *password = "drowssap";

float readDHTTemperature();
float readDHTHumidity();

void handleRoot() // HTML ver 1
{
  char msg[1500];

  snprintf(msg, 1500,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>Smart Air Vents</h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity());
  server.send(200, "text/html", msg);
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("smartairvents"))
  {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
  delay(2000);

  float temperature = dht.readTemperature();

  if (!isnan(temperature))
  {
    // Serial.print("Temperature: ");
    // Serial.println(temperature);

    int servoAngle = map(temperature, 25, 29, 0, 180);
    int angle = map((int)((t1 + t2 + t3) / 3), 15, 30, 30, 90);
    servo.write(angle);

    // Serial.print("Servo Angle: ");
    // Serial.println(servoAngle);
    Serial.println("Its working.....");
    myServo.write(servoAngle);
  }
  else
  {
    Serial.println("Failed to read from DHT sensor!");
  }
}

float readDHTTemperature()
{
  // Sensor readings may also be up to 2 seconds
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else
  {
    // Serial.println(t);
    return t;
  }
}

float readDHTHumidity()
{
  // Sensor readings may also be up to 2 seconds
  float h = dht.readHumidity();
  if (isnan(h))
  {
    Serial.println("Failed to read from DHT sensor!");
    return -1;
  }
  else
  {
    // Serial.println(h);
    return h;
  }
}
