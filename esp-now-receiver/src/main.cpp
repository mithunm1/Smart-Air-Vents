/**
 * @brief Handles the reception of data from other boards using ESP-NOW.
 *
 * This function is called whenever data is received from another board using the
 * ESP-NOW protocol. It extracts the received data, which includes an ID and a
 * temperature value, and stores it in the `boardsStruct` array.
 *
 * @param mac_addr The MAC address of the board that sent the data.
 * @param incomingData A pointer to the received data.
 * @param len The length of the received data.
 */
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  // Function implementation
}
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

typedef struct struct_message
{
  int id;
  float temperature;
} struct_message;

struct_message boardsStruct[3];

Servo servo;

#define SERVO_PIN 15
#define ANGLE_OPEN 50
#define ANGLE_CLOSED 30

#define TOLERANCE 3
#define TARGET_TEMP 26

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  char macStr[18];

  struct_message receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  boardsStruct[receivedData.id - 1] = receivedData;
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

  servo.attach(SERVO_PIN);
}

void loop()
{
  float totalTemp = 0;
  for (int i = 0; i < 3; i++)
  {
    totalTemp += boardsStruct[i].temperature;
  }
  float avgTemp = totalTemp / 3.0;

  if (avgTemp < (TARGET_TEMP - TOLERANCE))
  {
    servo.write(ANGLE_CLOSED);
  }
  else if (avgTemp > (TARGET_TEMP + TOLERANCE))
  {
    servo.write(ANGLE_OPEN);
  }
  else
  {
    int servoAngle = map(avgTemp, (TARGET_TEMP - TOLERANCE), (TARGET_TEMP + TOLERANCE), ANGLE_OPEN, ANGLE_CLOSED);
    servo.write(servoAngle);
  }

  Serial.println("---------------------------------");
  Serial.print("Servo Angle: ");
  Serial.println(servo.read());
  Serial.print("Average Temperature: ");
  Serial.println(avgTemp);
  for (int i = 0; i < 3; i++)
  {
    Serial.printf("Board %d - Temperature: %.2f°C\n", i + 1, boardsStruct[i].temperature);
    boardsStruct[i].temperature = 0;
  }

  delay(5000);
}
