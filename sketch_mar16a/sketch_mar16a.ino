void setup() {
  // 115200 is standard, but some S3's prefer native USB speed
  Serial.begin(115200);

  // Wait up to 5 seconds for the Serial Monitor to actually open
  // The LED will blink or stay still until you open the monitor
  while (!Serial && millis() < 5000) {
    delay(10);
  }
  
  Serial.println("\n--- ESP32-S3 Recovery Test ---");
  Serial.println("If you see this, the CDC port is working!");
}

void loop() {
  Serial.print("X: ");
  Serial.print(random(10), 4);
  Serial.print("\tY: ");
  Serial.print(random(10), 4);
  Serial.print("\tZ: ");
  Serial.print(random(10), 4);
  Serial.println("");
  delay(2000);
}