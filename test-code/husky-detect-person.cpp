#include <Wire.h>
#include <DFRobot_HuskylensV2.h>

HuskylensV2 huskylens;

// Set this to the ID printed when the camera detects "person".
// -1 means calibration mode: print all detected objects.
const int PERSON_CLASS_ID = -1;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println(F("Starting HuskyLens 2..."));

  while (!huskylens.begin(Wire)) {
    Serial.println(F("Can't connect to HuskyLens 2!"));
    delay(500);
  }

  Serial.println(F("Connected!"));

  // Switch to object recognition
  huskylens.switchAlgorithm(ALGORITHM_OBJECT_RECOGNITION);

  delay(1000);
}

void loop() {
  huskylens.getResult(ALGORITHM_OBJECT_RECOGNITION);

  bool personFound = false;

  if (huskylens.available(ALGORITHM_OBJECT_RECOGNITION)) {

    int n = huskylens.getCachedResultNum(
      ALGORITHM_OBJECT_RECOGNITION
    );

    for (int i = 0; i < n; i++) {

      auto r = huskylens.getCachedResultByIndex(
        ALGORITHM_OBJECT_RECOGNITION, i
      );

      if (r == NULL) continue;

      int id = RET_ITEM_NUM(r, Result, ID);
      int x = RET_ITEM_NUM(r, Result, xCenter);
      int w = RET_ITEM_NUM(r, Result, width);

      // Calibration mode: display all IDs
      if (PERSON_CLASS_ID == -1) {
        Serial.print(F("Detected ID="));
        Serial.print(id);
        Serial.print(F(" x="));
        Serial.print(x);
        Serial.print(F(" width="));
        Serial.println(w);
        continue;
      }

      // Ignore objects that are not people
      if (id != PERSON_CLASS_ID) continue;

      personFound = true;

      Serial.print(F("Person x="));
      Serial.print(x);
      Serial.print(F(" width="));
      Serial.println(w);
    }
  }

  if (PERSON_CLASS_ID == -1) {
    Serial.println(F("---"));
  }
  else if (!personFound) {
    Serial.println(F("No person detected"));
  }

  delay(200);
}