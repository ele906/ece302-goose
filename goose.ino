#include <Servo.h>
#include <Wire.h>

// ------ Serial ------
#include <SoftwareSerial.h>
SoftwareSerial xbee(10, 11); // DOUT - 10 (Rx) DIN - 11 (Tx)

// ------ Motor PWM ------
/* Note: PWM Pins in arduino are 8 bit, 
with values ranging from 0 (duty cycle 0) to 255 (duty cycle 100) */
const int PWM_Motor = 3;
const int u0_motor = 77; /* around 30% of 256*/

// ------ Adafruit ------
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_GenericDevice.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

// ------ Steering ------
#include "servo.h"

// ------ HuskyLens 2 ------
#include "tracker.h"

/* ------------------------------------------------------------------------------------------------- */

void setup() {
  // ------ Initialize Motor PWM Pins ------
  pinMode(PWM_Motor, OUTPUT);

  // ------ Initialize Serial ------
  Serial.begin(11520); 
  xbee.begin(11520);
  Wire.begin();

  // ------ Initialize Components ------
  tracker::begin(); // HuskyLens 
  audio::begin(); // Audio
  servo::begin(); // Servo Steering
}

/* ------------------------------------------------------------------------------------------------- */

void loop() {
  // ------ Send commands to PWM ------
  pwm.setPin(STEER_PWM_CHANNEL, 0);
  pwm.setPin(NECK_PWM_CHANNEL, 0);
  pwm.setPin(BEAK_PWM_CHANNEL, 0);

  // ------ Update Components ------
  tracker::update();
  servo::update();
  audio::update();
  
  // ------ Small delay to prevent overwhelming the Arduino with requests ------
  delay(100); 
}
