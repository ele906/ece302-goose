#include "tracker.h"
#include "HUSKYLENS.h"
#include <Wire.h>

namespace tracker {

static HUSKYLENS huskylens;
static unsigned long lastSeenMs = 0;
static bool          everSeen   = false;
static bool          visible    = false;

static void allOff() {
  digitalWrite(PIN_RIGHT,  LOW);
  digitalWrite(PIN_CENTER, LOW);
  digitalWrite(PIN_LEFT,   LOW);
}

void begin() {
  Wire.begin();
  if (!huskylens.begin(Wire)) {
    Serial.println(F("Can't connect to HuskyLens!"));
    while (true);
  }
  huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);

  pinMode(PIN_RIGHT,  OUTPUT);
  pinMode(PIN_CENTER, OUTPUT);
  pinMode(PIN_LEFT,   OUTPUT);
  allOff();
}

void update() {
  visible = false;

  if (huskylens.requestLearned() && huskylens.available() > 0) {
    int n = huskylens.countBlocks();
    for (int i = 0; i < n; i++) {
      HUSKYLENSResult r = huskylens.getBlock(i);
      if (r.ID != TARGET_ID) continue;

      visible     = true;
      everSeen    = true;
      lastSeenMs  = millis();

      allOff();
      if (r.xCenter > FRAME_CENTER_X + CENTER_THRESHOLD) {
        digitalWrite(PIN_RIGHT, HIGH);
        Serial.println("RIGHT");
      } else if (r.xCenter < FRAME_CENTER_X - CENTER_THRESHOLD) {
        digitalWrite(PIN_LEFT, HIGH);
        Serial.println("LEFT");
      } else {
        digitalWrite(PIN_CENTER, HIGH);
        Serial.println("CENTER");
      }
      break;
    }
  }

  if (!visible && everSeen && (millis() - lastSeenMs > LOST_TIMEOUT_MS)) {
    allOff();
    Serial.println("LOST - stopped");
  }
}

bool targetVisible() { return visible; }

} // namespace tracker