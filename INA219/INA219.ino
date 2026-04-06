#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

double total_mAh = 0.0;
float previous_mA = 0.0;
unsigned long last_micros = 0;

void setup() {
  Serial.begin(115200);
  if (!ina219.begin()) {
    Serial.println("INA219 not found!");
    while (1);
  }

  // Optimize for your 1A range
  // setCalibration_32V_2A provides 0.1mA resolution
  ina219.setCalibration_32V_2A();
  
  // Initialize readings
  previous_mA = ina219.getCurrent_mA();
  last_micros = micros();
}

void loop() {
  unsigned long current_micros = micros();
  unsigned long interval = current_micros - last_micros;

  // Sample every 50ms (20Hz)
  if (interval >= 50000) {
    float current_mA = ina219.getCurrent_mA();
    
    // Convert microseconds to hours: (interval / 1,000,000) / 3600
    double delta_hours = (double)interval / 3600000000.0;
    
    // Trapezoidal Rule: Average current * time elapsed
    // This fills the "gap" between samples more accurately
    double avg_current = (current_mA + previous_mA) / 2.0;
    double step_mAh = avg_current * delta_hours;

    total_mAh += step_mAh;

    // Update for next loop
    previous_mA = current_mA;
    last_micros = current_micros;

    // Debug output
    Serial.printf("Current: %.2f mA | Consumed: %.5f mAh\n", current_mA, total_mAh);
  }
}