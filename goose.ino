#include <DFRobot_HuskylensV2.h>
#include <ProtocolV2.h>
#include <Result.h>
#include <XiaozhiTextCheck.h>

#include <Wire.h>
#include <SoftwareSerial.h>

#include "servo.h"
#include "tracker.h"
#include "audio.h"
#include "speed.h"

SoftwareSerial xbee(10, 11); // DOUT - 10 (Rx) DIN - 11 (Tx)

enum class State { IDLE, ACQUIRE, TRACK, PECK, SEARCH };

State state = State::IDLE;
State prevState = State::IDLE;
unsigned long stateStartMs = 0;
const unsigned long SEARCH_TIMEOUT_MS = 10000;  // lost this long -> back to IDLE

// Point steering and head at the face
static void aimAtTarget() {
  int error = tracker::getTargetXCenter() - tracker::FRAME_CENTER_X;

  if (abs(error) <= tracker::CENTER_THRESHOLD) {
    servo::setSteer(servo::STEER_PWM_u0);
  } else {
    servo::setSteer(servo::STEER_PWM_u0 + (error / 2));  // flip sign if it steers away
  }
  servo::setNeckX(servo::GOOSE_PWM_u0 + (error / 4));    // flip sign if head turns away
}

static void centerEverything() {
  servo::setSteer(servo::STEER_PWM_u0);
  servo::setNeckX(servo::GOOSE_PWM_u0);
}

void setup() {
  Serial.begin(115200);
  xbee.begin(115200);

  speed::begin();     // car stopped on boot
  tracker::begin();
  audio::begin();
  servo::begin();

  Serial.println(F("Goose Robot Ready!"));
}

void loop() {
  // ------ 1. Inputs ------
  tracker::update();
  audio::update();

  bool sees  = tracker::targetVisible();
  bool lost  = tracker::targetLost();
  bool close = sees && tracker::getTargetWidth() >= tracker::PECK_WIDTH_THRESHOLD;

  // ------ 2. State transitions ------
  switch (state) {
    case State::IDLE:
      if (sees) state = State::ACQUIRE;
      break;

    case State::ACQUIRE:
      state = State::TRACK;
      break;

    case State::TRACK:
      if (lost)       state = State::SEARCH;
      else if (close) state = State::PECK;
      break;

    case State::PECK:
      if (lost) state = State::SEARCH;
      else if (!close && !servo::isPecking()) state = State::TRACK;
      break;

    case State::SEARCH:
      if (sees) state = State::TRACK;
      else if (millis() - stateStartMs > SEARCH_TIMEOUT_MS) state = State::IDLE;
      break;
  }

  // ------ 3. Sounds on state change ------
  if (state != prevState) {
    stateStartMs = millis();
    switch (state) {
      case State::ACQUIRE: audio::playSuccess(); break;
      case State::SEARCH:  audio::playHonk2();   break;
      default: break;
    }
    prevState = state;
  }

  // ------ 4. Act ------
  switch (state) {
    case State::IDLE:
    case State::SEARCH:
      speed::stop();
      centerEverything();
      break;

    case State::ACQUIRE:
      speed::stop();
      break;

    case State::TRACK:
      if (sees) {                       // on a missed frame, keep last command
        aimAtTarget();
        speed::driveForWidth(tracker::getTargetWidth());
      }
      break;

    case State::PECK: {
      speed::stop();
      if (sees) aimAtTarget();
      bool wasPecking = servo::isPecking();
      servo::triggerPeck();             // cooldown inside servo.cpp
      if (!wasPecking && servo::isPecking()) audio::playHonk1();  // honk once per peck
      break;
    }
  }

  servo::update();
}