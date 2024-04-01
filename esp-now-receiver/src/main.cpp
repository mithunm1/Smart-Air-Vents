#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> // Include the Servo library

// Receiver Logic
typedef struct struct_message
{
  int id;
  float temperature;
} struct_message;

struct_message boardsStruct[3];

Servo servo; // Create a Servo object

#define SERVO_PIN 15    // Define the pin for the servo
#define ANGLE_OPEN 90   // Define the angle for open position
#define ANGLE_CLOSED 30 // Define the angle for closed position

#define TARGET_TEMP 23 // Define the temperature set in thermostat

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  char macStr[18];
  // Serial.print("Packet received from: ");
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.println(macStr);

  struct_message receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  boardsStruct[receivedData.id - 1] = receivedData;

  // Serial.printf("Board ID %u: Temperature: %.2f°C\n", receivedData.id, receivedData.temperature);
  // Serial.println();
}

void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // Initialize servo
  servo.attach(SERVO_PIN);
}

void loop()
{
  float totalTemp = 0;
  // Calculate average temperature
  for (int i = 0; i < 3; i++)
  {
    totalTemp += boardsStruct[i].temperature;
  }
  float avgTemp = totalTemp / 3.0;

  // Control servo based on temperature
  if (avgTemp < (TARGET_TEMP - 2)) // If average temperature is less than 19°C
  {
    servo.write(ANGLE_CLOSED); // Close the servo
  }
  else if (avgTemp > (TARGET_TEMP + 2)) // If average temperature is more than 23°C
  {
    servo.write(ANGLE_OPEN); // Open the servo
  }
  else // If average temperature is between 19°C and 23°C
  {
    // Map temperature range to servo angle range
    int servoAngle = map(avgTemp, (TARGET_TEMP - 2), (TARGET_TEMP + 2), ANGLE_OPEN, ANGLE_CLOSED);
    servo.write(servoAngle);
  }

  // Print servo angle and average temperature
  Serial.print("Servo Angle: ");
  Serial.println(servo.read());
  Serial.print("Average Temperature: ");
  Serial.println(avgTemp);
  for (int i = 0; i < 3; i++)
  {
    Serial.printf("Board %d - Temperature: %.2f°C\n", i + 1, boardsStruct[i].temperature);
    boardsStruct[i].temperature = 0;
  }

  delay(10000); // Delay for 10 seconds before updating again
}

/*void loop()
{
  // You can access the received temperature data for each board from the boardsStruct array.
  // For example:
  for (int i = 0; i < 3; i++)
  {
    Serial.printf("Board %d - Temperature: %.2f°C\n", i + 1, boardsStruct[i].temperature);
  }

  delay(10000); // Delay for 10 seconds before printing again
}
*/