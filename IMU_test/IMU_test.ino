#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Standard S3 I2C pins (Check your specific DevKit label if 8/9 are used)
#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void) 
{

  Serial.begin(115200);
  delay(2000); // Give the computer time to recognize the USB
  Serial.println("HELLO? IS THIS WORKING?"); 
  
  // ... rest of code
  // S3 boards often use higher baud rates

  // For S3 Native USB: wait up to 5 seconds for serial port to connect
  unsigned long start = millis();
  while (!Serial && millis() - start < 5000); 

  Serial.println("\n--- ESP32-S3 BNO055 Test ---");
  
  /* Initialise I2C */
  if (!Wire.begin(I2C_SDA, I2C_SCL)) {
    Serial.println("I2C Bus failed to begin!");
    while(1);
  }
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    Serial.print("BNO055 not detected. Check pins 8 (SDA) and 9 (SCL).");
    while(1);
  }
  
  delay(1000);
  bno.setExtCrystalUse(true);
}

void loop(void) 
{
  sensors_event_t event; 
  bno.getEvent(&event);
  
  Serial.printf("X: %.4f\tY: %.4f\tZ: %.4f\n", 
                event.orientation.x, 
                event.orientation.y, 
                event.orientation.z);
  
  delay(500);
}