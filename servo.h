#pragma once 
#include <Arduino.h>

namespace servo {

constexpr int STEER_PWM_CHANNEL = 0;
constexpr int NECK_PWM_CHANNEL = 1;
constexpr int BEAK_PWM_CHANNEL = 2;

// ------ Steering Servo ------
constexpr int STEER_PWM_u0 = 332; 
constexpr int STEER_PWM_MAX = 250; 
constexpr int STEER_PWM_MIN = 413; 

// ------ Goose Parts PWM ------
constexpr int GOOSE_PWM_u0 = 295; 
constexpr int GOOSE_PWM_MAX = 107; 
constexpr int GOOSE_PWM_MIN = 520; 

void begin();
void update();
void setSteer(int pwmCount); 

} // namespace servo  