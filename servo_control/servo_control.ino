#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2500
#define FREQUENCY 50  // we use a digital servo that can run at higher frequencies

// Blue servos go from 850-2350

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();  // creat driver instance
Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int distal_L = 0;  // assign driver ports to servos
const int distal_R = 2;
const int proximal_L = 1;
const int proximal_R = 3;

const float P_d;
const float I_d;
const float D_d;

const float P_p = 1;
const float I_p;
const float D_p;

float setpoint = 0;
const float dt = 0.01;  // 100 Hz control loop

struct IMU_data {
  float roll;
  float pitch;
  float yaw;
};


void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(FREQUENCY);  // Analog servos run at ~50 Hz updates

  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }
  delay(1000);
  bno.setExtCrystalUse(true);
  driver.setPWM(proximal_L, 0, pulseWidth(90));
}

void loop() {

  static unsigned long last = 0;  // keeps track of time and is preserved through looping

  if (millis() - last >= 20) {  // 50 Hz loop
    last = millis();

    float measurement = read_imu();

    float control = computePID(measurement);

    apply_motor(control);

    //Serial.println(control);
  }
}


float computePID(float measurement) {

  static float prev_error = 0;
  static float integral = 0;

  float error = setpoint - measurement;

  integral += error * dt;
  integral = constrain(integral, -100, 100);  // anti-windup

  float derivative = (error - prev_error) / dt;

  float output = P_p * error + I_p * integral + D_p * derivative;

  prev_error = error;

  return output;
}


float read_imu() {  //assuming that there will exist some function from Arnav to get imu data
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  return event.orientation.y;
}

void apply_motor(float output) {

  int offset = constrain(output, -90, 90);
  Serial.print("Setting motor to: ");
  Serial.println(90+offset);
  driver.setPWM(proximal_L,0,pulseWidth(90+offset));
  //proximal_R.write(90-pwm);
}

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}
