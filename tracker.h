#pragma once
#include <Arduino.h>

namespace tracker {

constexpr int  CENTER_THRESHOLD = 30;
constexpr int  FRAME_CENTER_X   = 320; // HuskyLens 2 resolution is 640
constexpr int  TARGET_ID        = 1;
constexpr unsigned long LOST_TIMEOUT_MS = 500;

void begin();
void update();             // call from loop()
bool targetVisible();
int  getTargetXCenter();   // Returns where the face is (0 to 640)
int  getTargetWidth();     // Returns width of face (helps estimate distance)

} // namespace tracker