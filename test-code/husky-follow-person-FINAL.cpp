#include <Wire.h>
#include <DFRobot_HuskylensV2.h>
#include <math.h>

HuskylensV2 huskylens;

#define ALGO ALGORITHM_OBJECT_RECOGNITION

// ========== PERSON FILTER ==========
#define PERSON_ID 1           // the ID the camera gave "person" when you learned it
#define MIN_BOX_W 40          // ignore tiny junk boxes

// ========== STOPPING ==========
// Your person box was w~520 at your test distance. Stand where you want the
// goose to stop, read w, and set STOP_WIDTH to it.
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

  bool ok = huskylens.switchAlgorithm(ALGO);
  Serial.print(F("switchAlgorithm ok? "));
  Serial.println(ok);

  delay(1500);
}

void loop() {

  huskylens.getResult(ALGO);

  bool objectFound = false;
  int rawX = 0;
  int rawW = 0;

  if (huskylens.available(ALGO)) {

    int n = huskylens.getCachedResultNum(ALGO);

    Serial.print(F("Number of detections: "));
    Serial.print(n);
    Serial.print(F("  Learned: "));
    Serial.println(huskylens.getCachedResultLearnedNum(ALGO));

    for (int i = 0; i < n; i++) {

      auto r = huskylens.getCachedResultByIndex(ALGO, i);

      if (r == NULL) continue;

      int x = RET_ITEM_NUM(r, Result, xCenter);
      int w = RET_ITEM_NUM(r, Result, width);
      int y = RET_ITEM_NUM(r, Result, yCenter);
      int h = RET_ITEM_NUM(r, Result, height);
      int id = RET_ITEM_NUM(r, Result, ID);

      bool isPerson = (id == PERSON_ID) && (w >= MIN_BOX_W);

      // ========== PRINT EVERY DETECTION ==========
      Serial.print(F("Detection "));
      Serial.print(i);

      Serial.print(F(" | ID="));
      Serial.print(id);

      Serial.print(F(" | x="));
      Serial.print(x);

      Serial.print(F(" | y="));
      Serial.print(y);

      Serial.print(F(" | w="));
      Serial.print(w);

      Serial.print(F(" | h="));
      Serial.print(h);

      Serial.println(isPerson ? F("  <- person") : F("  (skipped)"));

      if (!isPerson) continue;

      // Select largest person
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

    // ========== STOPPING LOGIC ==========
    if (rawW >= STOP_WIDTH) {

      shouldStop = true;

    } else if (filteredW <= RESUME_WIDTH) {

      shouldStop = false;
    }

    // ========== PRINT TRACKING ==========
    Serial.print(F(">> Raw x="));
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

      Serial.println(F(">> NO PERSON - STOP"));

    } else {

      Serial.println(F(">> PERSON MISSING - STOP"));
    }
  }

  delay(100);
}