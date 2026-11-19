#pragma once
#include <Arduino.h>

namespace servo {

constexpr int STEER_PWM_CHANNEL  = 0;
constexpr int NECK_Y_PWM_CHANNEL = 1;   // up/down
constexpr int NECK_X_CHANNEL     = 2;   // left/right

// ------ Steering Servo ------
constexpr int STEER_PWM_u0  = 332;
constexpr int STEER_PWM_MAX = 250;
constexpr int STEER_PWM_MIN = 413;

// ------ Goose Neck PWM (both neck servos) ------
constexpr int GOOSE_PWM_u0  = 295;
constexpr int GOOSE_PWM_MAX = 107;
constexpr int GOOSE_PWM_MIN = 520;

// ------ Peck ------
constexpr int PECK_PWM = 375;                    // neck forward position (tune)
constexpr unsigned long PECK_DOWN_MS     = 200;  // time to reach forward position
constexpr unsigned long PECK_HOLD_MS     = 100;  // stay forward
constexpr unsigned long PECK_UP_MS       = 250;  // time to return
constexpr unsigned long PECK_COOLDOWN_MS = 1500; // min gap between pecks

void begin();
void update();                 // call every loop()

void setSteer(int pwmCount);
void setNeckX(int pwmCount);   // left/right aim
void setNeckY(int pwmCount);   // up/down resting position; peck returns here

void triggerPeck();            // safe to call every loop
bool isPecking();

} // namespace servo