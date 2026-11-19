#include <Wire.h>
#include <DFRobot_HuskylensV2.h>

HuskylensV2 huskylens;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Can't connect to HuskyLens 2!"));
    delay(500);
  }
  Serial.println(F("Connected!"));
  huskylens.switchAlgorithm(ALGORITHM_FACE_RECOGNITION);
}

void loop() {
  huskylens.getResult(ALGORITHM_FACE_RECOGNITION);
  if (huskylens.available(ALGORITHM_FACE_RECOGNITION)) {
    int n = huskylens.getCachedResultNum(ALGORITHM_FACE_RECOGNITION);
    for (int i = 0; i < n; i++) {
      auto r = huskylens.getCachedResultByIndex(ALGORITHM_FACE_RECOGNITION, i);
      if (r == NULL) continue;
      Serial.print(F("face ")); Serial.print(i);
      Serial.print(F("  ID="));  Serial.print(RET_ITEM_NUM(r, Result, ID));
      Serial.print(F("  x="));   Serial.print(RET_ITEM_NUM(r, Result, xCenter));
      Serial.print(F("  w="));   Serial.println(RET_ITEM_NUM(r, Result, width));
    }
  } else {
    Serial.println(F("no face"));
  }
  delay(200);
}