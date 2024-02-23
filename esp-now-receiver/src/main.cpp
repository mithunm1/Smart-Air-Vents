#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
typedef struct struct_message
{
  int id;            // Sender board ID
  float temperature; // Temperature data
} struct_message;

// Create a structure to hold the readings from each board
struct_message boardsStruct[3];

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  // Extract the data from the incoming packet
  struct_message receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Update the structures with the new incoming data
  boardsStruct[receivedData.id - 1] = receivedData;

  Serial.printf("Board ID %u: Temperature: %.2f°C\n", receivedData.id, receivedData.temperature);
  Serial.println();
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packet info
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