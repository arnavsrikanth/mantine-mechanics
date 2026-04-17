#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    float yaw;
    float pitch;
    float roll;
    float voltage;
    float current_mA;
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  // Format: yaw,pitch,roll,voltage,current
  Serial.print(incomingData.yaw);
  Serial.print(",");
  Serial.print(incomingData.pitch);
  Serial.print(",");
  Serial.print(incomingData.roll);
  Serial.print(",");
  Serial.print(incomingData.voltage);
  Serial.print(",");
  Serial.println(incomingData.current_mA);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Nothing needed here, handled by callback
}