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

  // Switch to object detection
  huskylens.switchAlgorithm(ALGORITHM_OBJECT_RECOGNITION);

  delay(1000);
}

void loop() {
  huskylens.getResult(ALGORITHM_OBJECT_RECOGNITION);

  if (huskylens.available(ALGORITHM_OBJECT_RECOGNITION)) {

    int n = huskylens.getCachedResultNum(ALGORITHM_OBJECT_RECOGNITION);

    for (int i = 0; i < n; i++) {

      auto r = huskylens.getCachedResultByIndex(
        ALGORITHM_OBJECT_RECOGNITION, i
      );

      if (r == NULL) continue;

      int id = RET_ITEM_NUM(r, Result, ID);
      int x = RET_ITEM_NUM(r, Result, xCenter);
      int w = RET_ITEM_NUM(r, Result, width);
      int h = RET_ITEM_NUM(r, Result, height);

      Serial.print(F("Object "));
      Serial.print(i);

      Serial.print(F(" ID="));
      Serial.print(id);

      Serial.print(F(" x="));
      Serial.print(x);

      Serial.print(F(" w="));
      Serial.print(w);

      Serial.print(F(" h="));
      Serial.println(h);
    }

  } else {
    Serial.println(F("No objects detected"));
  }

  delay(200);
}