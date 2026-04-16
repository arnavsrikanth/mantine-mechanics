/* This example shows how to control a single servo on a Maestro
servo controller using the PololuMaestro library. It assumes you
have an RC hobby servo connected on channel 0 of your Maestro,
and that you have already used the Maestro Control Center
software to verify that the servo is powered correctly and moves
when you command it to from the Maestro Control Center.

Before using this example, you should go to the Serial Settings
tab in the Maestro Control Center and apply these settings:

* Serial mode: UART, fixed baud rate
* Baud rate: 9600
* CRC disabled

Be sure to click "Apply Settings" after making any changes.

This example also assumes you have connected your Arduino to your
Maestro appropriately. If you have not done so, please see
https://github.com/pololu/maestro-arduino for more details on how
to make the connection between your Arduino and your Maestro. */

#include <PololuMaestro.h>

/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin 16 to receive (RX) and
pin 17 to transmit (TX). */

#ifdef ARDUINO_ARCH_ESP32
#define maestroSerial Serial2
#else
#ifdef SERIAL_PORT_HARDWARE_OPEN
#define maestroSerial SERIAL_PORT_HARDWARE_OPEN
#else
#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(10, 11);
#endif
#endif

#define BLACK_MIN_PULSE_WIDTH 500  // in microseconds
#define BLACK_MAX_PULSE_WIDTH 2500
#define BLACK_MAX_ANGLE 180

#define BLUE_MIN_PULSE_WIDTH 850
#define BLUE_MAX_PULSE_WIDTH 2350
#define BLUE_MAX_ANGLE 145
// Blue servos go from 850-2350

/* Next, create a Maestro object using the serial port. 
Uncomment one of MicroMaestro or MiniMaestro below depending 
on which one you have. */

MicroMaestro maestro(maestroSerial);
// MiniMaestro maestro(maestroSerial);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting test...");
#ifdef ARDUINO_ARCH_ESP32
  maestroSerial.begin(9600, SERIAL_8N1, 16, 17);
#else
  maestroSerial.begin(9600);
#endif
  // Optional: send a test byte to check TX pin
  maestroSerial.write(0xAA);
  Serial.println("TX test pulse sent");
}
void loop() {
  /* setTarget takes the channel number you want to control, and
     the target position in units of 1/4 microseconds. A typical
     RC hobby servo responds to pulses between 1 ms (4000) and 2
     ms (8000). */

  // Set the target of channel 0 to 1500 us, and wait 2 seconds.
  // Serial.print("Moving to 0: ");
  //apply_motor(0,90);
  //apply_motor(1,72);
  // delay(1000);
  // while (true) {
  // if (Serial.available() > 0) {
  //   if (Serial.read() == 'G') {
  //     break; // Exit the loop only when 'G' is pressed
  //     }
  //   }
  // }

  // // Set the target of channel 0 to 1750 us, and wait 2 seconds.
  // Serial.print("Moving to max: ");
  // apply_motor(1,BLUE_MAX_ANGLE);
  // apply_motor(0,0);
  // delay(1000);
  // while (true) {
  // if (Serial.available() > 0) {
  //   if (Serial.read() == 'G') {
  //     break; // Exit the loop only when 'G' is pressed
  //     }
  //   }
  // }

  // // Set the target of channel 0 to 1250 us, and wait 2 seconds.
  // Serial.print("Moving to 135: ");
  // apply_motor(1,135);
  // delay(500);
  // apply_motor(0,135);
  // delay(500);
  // motor_limit(0);
  // delay(2000);
  motor_limit(1);
}

/**
 * @brief Sets the desired motor to the specified angle
 * 
 * @param channel Channel number of motor. Even channel for black motors
 * @param angle Desired angle
 */
void apply_motor(int channel, float angle) {
  int max_width, min_width, max_angle;
  if (channel % 2 == 0) {
    min_width = BLACK_MIN_PULSE_WIDTH;
    max_width = BLACK_MAX_PULSE_WIDTH;
    max_angle = BLACK_MAX_ANGLE;
  } else {
    min_width = BLUE_MIN_PULSE_WIDTH;
    max_width = BLUE_MAX_PULSE_WIDTH;
    max_angle = BLUE_MAX_ANGLE;
  }
  int pulse_width;
  pulse_width = map(angle, 0, max_angle, min_width, max_width);
  Serial.println(pulse_width);
  pulse_width = constrain(pulse_width, 0, 16383);  // constrain to max values of maestro
  maestro.setTarget(channel, 4 * pulse_width);     // target is in units of quarter-microseconds
}

void motor_limit(int channel) {
  int high = 100;
  int low = 20;
  for (int i = low; i <= high; i++) {
    Serial.print(i * 25);
    Serial.print(" <- us.  i-> ");
    Serial.println(i);
    maestro.setTarget(channel, 100 * i);
    delay(10);
  }
  for (int i = high; i >= low; i--) {
    Serial.print(i * 25);
    Serial.print(" <- us.  i-> ");
    Serial.println(i);
    maestro.setTarget(channel, 100 * i);
    delay(10);
  }
}
