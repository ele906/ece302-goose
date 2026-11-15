#include "servo.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace servo {
static Adafruit_PWMServoDriver pwm;

void begin() {
  // ------ Initialize Steering PWM Pins ------
  pwm.begin();
  pwm.setPWMFreq(50); // Standard 50Hz frequency for servos
}

void update() {
  // code syntax: pwm.setPin(channel, value)
  pwm.setPWM(STEER_PWM_CHANNEL, 0, STEER_PWM_u0);
  pwm.setPWM(NECK_PWM_CHANNEL, 0, GOOSE_PWM_u0);
  pwm.setPWM(BEAK_PWM_CHANNEL, 0, GOOSE_PWM_u0);
  
}

} // namespace servo