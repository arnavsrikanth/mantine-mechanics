#include <Servo.h>

Servo distal_L;  // create Servo object to control a servo
Servo distal_R;
Servo proximal_L;
Servo proximal_R;

const float P_d;
const float I_d;
const float D_d;

const float P_p;
const float I_p;
const float D_p;

float setpoint = 0;
const float dt = 0.01;   // 100 Hz control loop

struct IMU_data {
  float roll;
  float pitch;
  float yaw;

};

int imupin = A0;  // analog pin used to connect the potentiometer
IMU_data imu;    // variable to read the value from the analog pin

void setup() {
  proximal_L.attach(9);  // attaches the servo on pin 9 to the Servo object
  proximal_R.attach(10); // TODO figure out howw servo driver works for this
  Serial.begin(9600);
}

void loop() {

  static unsigned long last = 0; // keeps track of time and is preserved through looping

  if (millis() - last >= 10) {   // 100 Hz loop
    last = millis();

    IMU_data measurement = read_imu();

    float control = computePID(measurement);

    apply_motor(control);

    Serial.println(control);
  }
}


float computePID(IMU_data measurement) {

  static float prev_error = 0;
  static float integral = 0;

  float error = setpoint - measurement.roll;

  integral += error * dt;
  integral = constrain(integral, -100, 100);   // anti-windup

  float derivative = (error - prev_error) / dt;

  float output = P_p*error + I_p*integral + D_p*derivative;

  prev_error = error;

  return output;
}


IMU_data read_imu(){ //assuming that there will exist some function from Arnav to get imu data
  IMU_data data;

  data.roll = 0;
  data.pitch = 0;
  data.yaw = 0;
}

void apply_motor(float output) {

  int pwm = constrain(output, -90, 90);
  proximal_L.write(90+pwm);
  proximal_R.write(90-pwm);

}


