#include "servo.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace servo {

static Adafruit_PWMServoDriver pwm;
static int steerCount = STEER_PWM_u0; 

void begin() {
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50); 
  
  pwm.setPWM(STEER_PWM_CHANNEL, 0, STEER_PWM_u0);
  pwm.setPWM(NECK_PWM_CHANNEL,  0, GOOSE_PWM_u0);
  pwm.setPWM(BEAK_PWM_CHANNEL,  0, GOOSE_PWM_u0);
}

void update() {
  pwm.setPWM(STEER_PWM_CHANNEL, 0, steerCount); 
  pwm.setPWM(NECK_PWM_CHANNEL,  0, GOOSE_PWM_u0);
  pwm.setPWM(BEAK_PWM_CHANNEL,  0, GOOSE_PWM_u0);
}

void setSteer(int pwmCount) {
  // Clamp values so it doesn't break the servo
  if (pwmCount < STEER_PWM_MAX) pwmCount = STEER_PWM_MAX; // 250
  if (pwmCount > STEER_PWM_MIN) pwmCount = STEER_PWM_MIN; // 413
  steerCount = pwmCount;
}

} // namespace servo