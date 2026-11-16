#include "tracker.h"
#include <Wire.h>

#include <DFRobot_HuskylensV2.h>
#include <ProtocolV2.h>
#include <Result.h>

#include "servo.h"

namespace tracker {

static HuskylensV2 huskylens;      
static unsigned long lastSeenMs = 0;
static bool          everSeen   = false;
static bool          visible    = false;

static int currentX = 0;
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
    auto centerResult = huskylens.getCachedCenterResult(ALGORITHM_FACE_RECOGNITION);
    
    // Check ID (using the macro from ProtocolV2.h / Result.h)
    int id = RET_ITEM_NUM(centerResult, Result, ID);
    
    if (id == TARGET_ID) {
      visible     = true;
      everSeen    = true;
      lastSeenMs  = millis();
      
      currentX    = RET_ITEM_NUM(centerResult, Result, xCenter);
      currentW    = RET_ITEM_NUM(centerResult, Result, width);

      int error = currentX - FRAME_CENTER_X; 
      
      if (error > CENTER_THRESHOLD) {
        servo::setSteer(servo::STEER_PWM_u0 + (error / 2));
      } else if (error < -CENTER_THRESHOLD) {
        servo::setSteer(servo::STEER_PWM_u0 + (error / 2));
      } else {
        servo::setSteer(servo::STEER_PWM_u0);
      }
    }
  }

  if (!visible && everSeen && (millis() - lastSeenMs > LOST_TIMEOUT_MS)) {
    servo::setSteer(servo::STEER_PWM_u0);
  }
}

bool targetVisible() { return visible; }
int  getTargetXCenter() { return currentX; }
int  getTargetWidth() { return currentW; }

} // namespace tracker