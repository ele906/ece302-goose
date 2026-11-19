#include <Wire.h>
#include <DFRobot_HuskylensV2.h>
#include <math.h>

HuskylensV2 huskylens;

// ========== STOPPING ==========
#define STOP_WIDTH 600
#define RESUME_WIDTH 550

// ========== GLITCH FILTER ==========
#define FILTER_ALPHA 0.35f
#define MAX_WIDTH_JUMP 150
#define MAX_X_JUMP 200
#define MAX_REJECTS 3

// ========== TIMEOUT ==========
#define LOST_TIMEOUT_MS 500

float filteredX = 0;
float filteredW = 0;

bool initialized = false;
bool shouldStop = true;

int rejectedFrames = 0;
unsigned long lastSeen = 0;

void resetTracking() {
  initialized = false;
  rejectedFrames = 0;
  shouldStop = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println(F("Starting HuskyLens 2..."));

  while (!huskylens.begin(Wire)) {
    Serial.println(F("Can't connect!"));
    delay(500);
  }

  Serial.println(F("Connected!"));

  huskylens.switchAlgorithm(ALGORITHM_OBJECT_RECOGNITION);
  delay(1000);
}

void loop() {
  huskylens.getResult(ALGORITHM_OBJECT_RECOGNITION);

  bool objectFound = false;
  int rawX = 0;
  int rawW = 0;

  if (huskylens.available(ALGORITHM_OBJECT_RECOGNITION)) {

    int n = huskylens.getCachedResultNum(
      ALGORITHM_OBJECT_RECOGNITION
    );

    for (int i = 0; i < n; i++) {

      auto r = huskylens.getCachedResultByIndex(
        ALGORITHM_OBJECT_RECOGNITION, i
      );

      if (r == NULL) continue;

      int x = RET_ITEM_NUM(r, Result, xCenter);
      int w = RET_ITEM_NUM(r, Result, width);

      // Select largest detected object
      if (!objectFound || w > rawW) {
        rawX = x;
        rawW = w;
        objectFound = true;
      }
    }
  }

  unsigned long now = millis();

  if (objectFound) {
    lastSeen = now;

    if (!initialized) {
      filteredX = rawX;
      filteredW = rawW;
      initialized = true;
      rejectedFrames = 0;
    } else {

      bool glitch =
        fabs(rawX - filteredX) > MAX_X_JUMP ||
        fabs(rawW - filteredW) > MAX_WIDTH_JUMP;

      if (glitch) {
        rejectedFrames++;

        if (rejectedFrames >= MAX_REJECTS) {
          filteredX = rawX;
          filteredW = rawW;
          rejectedFrames = 0;
        }
      } else {
        rejectedFrames = 0;

        filteredX += FILTER_ALPHA * (rawX - filteredX);
        filteredW += FILTER_ALPHA * (rawW - filteredW);
      }
    }

    // Stop immediately when object is close
    if (rawW >= STOP_WIDTH) {
      shouldStop = true;
    } else if (filteredW <= RESUME_WIDTH) {
      shouldStop = false;
    }

    Serial.print(F("Raw x="));
    Serial.print(rawX);

    Serial.print(F(" w="));
    Serial.print(rawW);

    Serial.print(F(" | Filtered x="));
    Serial.print(filteredX, 1);

    Serial.print(F(" w="));
    Serial.print(filteredW, 1);

    Serial.print(F(" | "));
    Serial.println(shouldStop ? F("STOP") : F("FOLLOW"));

  } else {

    shouldStop = true;

    if (now - lastSeen >= LOST_TIMEOUT_MS) {
      resetTracking();
      Serial.println(F("NO OBJECT - STOP"));
    } else {
      Serial.println(F("OBJECT MISSING - STOP"));
    }
  }

  delay(100);
}