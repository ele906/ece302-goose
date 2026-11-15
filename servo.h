#pragma once
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

// ------ Steering ------
/* Note: The PCA9865 supports a 12 bit PWM
with values ranging from 0 (duty cycle 0) to 4095 (duty cycle 100) */

// Pin / threshold config lives with the module so it's self-contained.
namespace servo {

constexpr int STEER_PWM_CHANNEL = 0;
constexpr int NECK_PWM_CHANNEL = 1;
constexpr int BEAK_PWM_CHANNEL = 2;

constexpr int STEER_PWM_u0 = 332; /* straight ahead, ~8.1% of 4096 */
constexpr int NECK_PWM_u0 = 300; /* placeholder value*/
constexpr int BEAK_PWM_u0 = 300; /* placeholder value */

void begin();
void update();

} // namespace tracker