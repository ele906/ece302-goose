#include "tracker.h"
#include <Wire.h>

#include <DFRobot_HuskylensV2.h>
#include <ProtocolV2.h>
#include <Result.h>

namespace tracker {

static HuskylensV2 huskylens;
static unsigned long lastSeenMs = 0;
static bool visible = false;
static bool hasLock = false;          // locked onto one person
static int currentX = FRAME_CENTER_X;
static int currentW = 0;

void begin() {
  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Can't connect to HuskyLens 2!"));
    delay(100);
  }
  huskylens.switchAlgorithm(ALGORITHM_FACE_RECOGNITION);
}

void update() {
  visible = false;

  huskylens.getResult(ALGORITHM_FACE_RECOGNITION);
  if (huskylens.available(ALGORITHM_FACE_RECOGNITION)) {
    int n = huskylens.getCachedResultNum(ALGORITHM_FACE_RECOGNITION);

    int bestIdx = -1;
    int bestScore = 0;   // lock: smaller distance from last X wins; no lock: bigger width wins

    for (int i = 0; i < n; i++) {
      auto r = huskylens.getCachedResultByIndex(ALGORITHM_FACE_RECOGNITION, i);
      if (r == NULL) continue;
      if (!ANY_FACE && RET_ITEM_NUM(r, Result, ID) != TARGET_ID) continue;

      int w = RET_ITEM_NUM(r, Result, width);
      int x = RET_ITEM_NUM(r, Result, xCenter);

      int score = hasLock ? -abs(x - currentX) : w;   // higher score is better
      if (bestIdx < 0 || score > bestScore) { bestIdx = i; bestScore = score; }
    }

    if (bestIdx >= 0) {
      auto r = huskylens.getCachedResultByIndex(ALGORITHM_FACE_RECOGNITION, bestIdx);
      if (r != NULL) {
        visible    = true;
        hasLock    = true;
        lastSeenMs = millis();
        currentX   = RET_ITEM_NUM(r, Result, xCenter);
        currentW   = RET_ITEM_NUM(r, Result, width);
      }
    }
  }

  // Lost for too long: drop the lock so the next update picks the closest face again
  if (!visible && (millis() - lastSeenMs > LOST_TIMEOUT_MS)) hasLock = false;
}

bool targetVisible() { return visible; }
bool targetLost()    { return !visible && (millis() - lastSeenMs > LOST_TIMEOUT_MS); }
int  getTargetXCenter() { return currentX; }
int  getTargetWidth()   { return currentW; }

} // namespace tracker