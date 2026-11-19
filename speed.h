#pragma once
#include <Arduino.h>

namespace speed {

constexpr int PIN_MOTOR      = 9;    // moved off pin 3 (audio honk 2 uses 3)
constexpr int MOTOR_IDLE     = 77;   // ~30%, stopped
constexpr int MOTOR_MAX      = 117;  // idle + 40, tune
constexpr int SLOW_START_W   = 80;   // face width where we start slowing down

void begin();
void stop();
void driveForWidth(int faceWidth);   // full speed when far, ramps to stop when close

} // namespace speed