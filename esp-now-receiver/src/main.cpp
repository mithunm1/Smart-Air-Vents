#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Receiver Logic
typedef struct struct_message
{
  int id;
  float temperature;
} struct_message;

struct_message boardsStruct[3];

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  struct_message receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  boardsStruct[receivedData.id - 1] = receivedData;

  Serial.printf("Board ID %u: Temperature: %.2f°C\n", receivedData.id, receivedData.temperature);
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

/* void loop()
{
  // You can access the received temperature data for each board from the boardsStruct array.
  // For example:
  for (int i = 0; i < 3; i++) {
    Serial.printf("Board %d - Temperature: %.2f°C\n", i + 1, boardsStruct[i].temperature);
  }

  delay(10000); // Delay for 10 seconds before printing again
}*/

void loop()
{
  // Acess the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  int board3X = boardsStruct[2].x;
  int board3Y = boardsStruct[2].y;*/

  delay(10000);
}