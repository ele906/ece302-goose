#include "servo.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace servo {

static Adafruit_PWMServoDriver pwm;

static int steerCount = STEER_PWM_u0;
static int neckXCount = GOOSE_PWM_u0;   // left/right target
static int neckYRest  = GOOSE_PWM_u0;   // up/down resting position
static int neckYCount = GOOSE_PWM_u0;   // up/down value actually written

static bool pecking = false;
static unsigned long peckStartMs = 0;
static unsigned long lastPeckEndMs = 0;

static int clampNeck(int pwmCount) {
  if (pwmCount < GOOSE_PWM_MAX) pwmCount = GOOSE_PWM_MAX;  // 107
  if (pwmCount > GOOSE_PWM_MIN) pwmCount = GOOSE_PWM_MIN;  // 520
  return pwmCount;
}

void begin() {
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50);

  pwm.setPWM(STEER_PWM_CHANNEL,  0, STEER_PWM_u0);
  pwm.setPWM(NECK_Y_PWM_CHANNEL, 0, GOOSE_PWM_u0);
  pwm.setPWM(NECK_X_CHANNEL,     0, GOOSE_PWM_u0);
}

void setSteer(int pwmCount) {
  if (pwmCount < STEER_PWM_MAX) pwmCount = STEER_PWM_MAX;  // 250
  if (pwmCount > STEER_PWM_MIN) pwmCount = STEER_PWM_MIN;  // 413
  steerCount = pwmCount;
}

void setNeckX(int pwmCount) { neckXCount = clampNeck(pwmCount); }
void setNeckY(int pwmCount) { neckYRest  = clampNeck(pwmCount); }

void triggerPeck() {
  if (pecking) return;
  if (lastPeckEndMs != 0 && millis() - lastPeckEndMs < PECK_COOLDOWN_MS) return;
  pecking = true;
  peckStartMs = millis();
}

bool isPecking() { return pecking; }

static void servicePeck() {
  if (!pecking) { neckYCount = neckYRest; return; }

  unsigned long t = millis() - peckStartMs;
  if (t < PECK_DOWN_MS + PECK_HOLD_MS) {
    neckYCount = clampNeck(PECK_PWM);       // forward
  } else if (t < PECK_DOWN_MS + PECK_HOLD_MS + PECK_UP_MS) {
    neckYCount = neckYRest;                 // back to rest
  } else {
    pecking = false;
    lastPeckEndMs = millis();
    neckYCount = neckYRest;
  }
}

void update() {
  servicePeck();
  pwm.setPWM(STEER_PWM_CHANNEL,  0, steerCount);
  pwm.setPWM(NECK_Y_PWM_CHANNEL, 0, neckYCount);
  pwm.setPWM(NECK_X_CHANNEL,     0, neckXCount);
}

} // namespace servo