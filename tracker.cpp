#include "tracker.h"
#include <Wire.h>

#include <DFRobot_HuskylensV2.h>
#include <ProtocolV2.h>
#include <Result.h>

namespace tracker {

static HuskylensV2 huskylens;
static unsigned long lastSeenMs = 0;
static bool visible = false;
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
    auto r = huskylens.getCachedCenterResult(ALGORITHM_FACE_RECOGNITION);
    int id = RET_ITEM_NUM(r, Result, ID);

    if (id == TARGET_ID) {
      visible    = true;
      lastSeenMs = millis();
      currentX   = RET_ITEM_NUM(r, Result, xCenter);
      currentW   = RET_ITEM_NUM(r, Result, width);
    }
  }
}

bool targetVisible() { return visible; }
bool targetLost()    { return !visible && (millis() - lastSeenMs > LOST_TIMEOUT_MS); }
int  getTargetXCenter() { return currentX; }
int  getTargetWidth()   { return currentW; }

} // namespace tracker