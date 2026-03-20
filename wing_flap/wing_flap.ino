#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BLACK_MIN_PULSE_WIDTH 500
#define BLACK_MAX_PULSE_WIDTH 2500
#define FREQUENCY 50  // we use a digital servo that can run at higher frequencies

#define BLUE_MIN_PULSE_WIDTH 850
#define BLUE_MAX_PULSE_WIDTH 2350
// Blue servos go from 850-2350

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();  // creat driver instance
Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int distal_L = 0;  // BLUE motor
const int proximal_L = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(FREQUENCY);  // Analog servos run at ~50 Hz updates
  driver.setPWM(proximal_L, 0, pulseWidth(90));
  driver.setPWM(distal_L,0,pulseWidth(90,false))
}

void loop() {
  // put your main code here, to run repeatedly:
  for (float i = 0; i <90; i++){
    apply_motor(i);
    delay(20);
  }
  driver.setPWM(distal_L,0,pulseWidth(45,false));
  driver.setPWM(proximal_L,0,pulseWidth(0));
  driver.setPWM(distal_L,0,pulseWidth(135,false));
  driver.setPWM(proximal_L,0,pulseWidth(90));
    driver.setPWM(distal_L,0,pulseWidth(90,false));
}

void apply_motor(float output) {
  int offset = constrain(output, -90, 90);
  Serial.print("Setting motor to: ");
  Serial.println(90+offset);
  driver.setPWM(proximal_L,0,pulseWidth(90+offset));
  driver.setPWM(distal_L,0,pulseWidth(90+offset/2,false));
  //proximal_R.write(90-pwm);
}

int pulseWidth(int angle, bool color = true) {
  int max_width;
  int min_width
  if (bool) {
    min_width = BLACK_MIN_PULSE_WIDTH;
    max_width = BLACK_MAX_PULSE_WIDTH;
  }
  else{
    min_width = BLUE_MIN_PULSE_WIDTH;
    max_width = BLUE_MAX_PULSE_WIDTH;
  }
  int pulse_wide, analog_value;
  pulse_wide = map(angle, 0, 180, min_width, max_width);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}