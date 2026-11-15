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

// ------ Steering Servo ------
constexpr int STEER_PWM_u0 = 332; /* straight ahead, ~8.1% of 4096 */
constexpr int STEER_PWM_MAX = 250; /* 6.1% * 4096*/
constexpr int STEER_PWM_MIN = 413; /* 10.1% * 4096*/

// ------ Goose Parts PWM ------
// 2.6%, 7.2%, 12.7%
constexpr int GOOSE_PWM_u0 = 295; /* straight ahead - 4096 * 7.2% */
constexpr int GOOSE_PWM_MAX = 107; /* left - 4096 * 2.6% */
constexpr int GOOSE_PWM_MIN = 520; /* right - 4096 * 12.7% */
void begin();
void update();

} // namespace tracker