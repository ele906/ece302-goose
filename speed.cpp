#include "speed.h"
#include "tracker.h"

namespace speed {

void begin() {
  pinMode(PIN_MOTOR, OUTPUT);
  analogWrite(PIN_MOTOR, MOTOR_IDLE);
}

void stop() {
  analogWrite(PIN_MOTOR, MOTOR_IDLE);
}

void driveForWidth(int w) {
  int v;
  if (w >= tracker::PECK_WIDTH_THRESHOLD)  v = MOTOR_IDLE;
  else if (w <= SLOW_START_W)              v = MOTOR_MAX;
  else v = map(w, SLOW_START_W, tracker::PECK_WIDTH_THRESHOLD, MOTOR_MAX, MOTOR_IDLE);
  analogWrite(PIN_MOTOR, v);
}

} // namespace speed