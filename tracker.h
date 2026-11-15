#pragma once // include a header file only once during a single compilation
#include <Arduino.h>

// Pin / threshold config lives with the module so it's self-contained.
namespace tracker {

constexpr int  PIN_RIGHT  = 6;
constexpr int  PIN_CENTER = 7;
constexpr int  PIN_LEFT   = 8;
constexpr int  CENTER_THRESHOLD = 30;
constexpr int  FRAME_CENTER_X   = 320; // HuskyLens 2 resolution is 640
constexpr int  TARGET_ID        = 1;
constexpr unsigned long LOST_TIMEOUT_MS = 500;

void begin();
void update();   // call from loop()
bool targetVisible();

} // namespace tracker