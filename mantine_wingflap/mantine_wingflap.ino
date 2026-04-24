#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_INA219.h>

#include <PololuMaestro.h>
#include <math.h>
//#include <TrajectoryData.h>

// REPLACE WITH YOUR RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0x1C, 0xDB, 0xD4, 0x9C, 0x1D, 0xEC};

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_INA219 ina219;
int count;

#define maestroSerial Serial2
#define BLACK_MIN_PULSE_WIDTH 500  // in microseconds
#define BLACK_MAX_PULSE_WIDTH 2500
#define BLACK_MAX_ANGLE 180

#define BLUE_MIN_PULSE_WIDTH 850
#define BLUE_MAX_PULSE_WIDTH 2350
#define BLUE_MAX_ANGLE 145
// Blue servos go from 850-2350
MicroMaestro maestro(maestroSerial);

int flipped[4] = { 1, 1, -1, -1 }; // Whether motors are flipped wrt each other
int angle_offset[4] = {0, 0, 0, 0}; // Angle offsets for the motors

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
  int count = 0;
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  #ifdef ARDUINO_ARCH_ESP32
  maestroSerial.begin(9600, SERIAL_8N1, 17, 16);
#else
  maestroSerial.begin(9600);
#endif
  // Optional: send a test byte to check TX pin
  maestroSerial.write(0xAA);
  Serial.println("TX test pulse sent");

  if (!bno.begin()) { Serial.println("No BNO055 detected"); while(1); }
  if (!ina219.begin()) { Serial.println("No INA219 detected"); while(1); }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  if (count % 10 == 0){
  sensors_event_t event;
  bno.getEvent(&event);

  myData.yaw = event.orientation.x;
  myData.pitch = event.orientation.y;
  myData.roll = event.orientation.z;
  myData.voltage = ina219.getBusVoltage_V();
  myData.current_mA = ina219.getCurrent_mA();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
  float theta1 = angle_data[0][count] * PI * 180.0;
  float theta2 =  angle_data[1][count] * PI * 180.0;
  // int i = 0;
  // i = count % 90;
  // i -= 45;
  // int j;
  // if (abs(i-45) < 5){
  //   j = 45 * (i > 0) - (i < 0);
  // }
  // else{
  //   j = i;
  // }
  apply_motor(0,theta1);
  apply_motor(1,theta2);
  apply_motor(2,theta1);
  apply_motor(3,theta1);
  count++;
  delay(10); // 100Hz update rate
}

/**
 * @brief Sets the desired motor to the specified angle
 * 
 * @param channel Channel number of motor. Even channel for black motors
 * @param angle Desired angle
 */
void apply_motor(int channel, float angle) {
  int max_width, min_width, max_angle;
  min_width = BLUE_MIN_PULSE_WIDTH;
  max_width = BLUE_MAX_PULSE_WIDTH;
  max_angle = BLUE_MAX_ANGLE;
  int pulse_width;
  float angle_adjusted = max_angle / 2.0 + angle * flipped[channel] + angle_offset[channel];
  pulse_width = map(angle_adjusted, 0, max_angle, min_width, max_width);
  //Serial.print(pulse_width);
  //Serial.println(", ");
  pulse_width = constrain(pulse_width, 0, 16383);  // constrain to max values of maestro
  maestro.setTarget(channel, 4 * pulse_width);     // target is in units of quarter-microseconds
}
