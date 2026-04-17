#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_INA219.h>

// REPLACE WITH YOUR RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0x1C, 0xDB, 0xD4, 0x9C, 0x1D, 0xEC};

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_INA219 ina219;

// Structure to match the receiver
typedef struct struct_message {
    float yaw;
    float pitch;
    float roll;
    float voltage;
    float current_mA;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional: Serial.print("Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!bno.begin()) { Serial.println("No BNO055 detected"); while(1); }
  if (!ina219.begin()) { Serial.println("No INA219 detected"); while(1); }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  myData.yaw = event.orientation.x;
  myData.pitch = event.orientation.y;
  myData.roll = event.orientation.z;
  myData.voltage = ina219.getBusVoltage_V();
  myData.current_mA = ina219.getCurrent_mA();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  delay(100); // 10Hz update rate
}