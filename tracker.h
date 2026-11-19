#pragma once
#include <Arduino.h>

namespace tracker {

constexpr int  CENTER_THRESHOLD = 30;
constexpr int  FRAME_CENTER_X   = 320;  // HuskyLens 2 resolution is 640
constexpr int  TARGET_ID        = 1;
constexpr unsigned long LOST_TIMEOUT_MS = 500;
constexpr int  PECK_WIDTH_THRESHOLD = 150;  // face width (px) at ~2 ft, measure and tune

void begin();
void update();             // call every loop(); only reads the camera
bool targetVisible();      // seen this frame
bool targetLost();         // not seen for LOST_TIMEOUT_MS
int  getTargetXCenter();   // 0 to 640
int  getTargetWidth();     // bigger = closer

} // namespace tracker