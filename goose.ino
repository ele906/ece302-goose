#include <DFRobot_HuskylensV2.h>
#include <ProtocolV2.h>
#include <Result.h>
#include <XiaozhiTextCheck.h>

#include <Wire.h>
#include <SoftwareSerial.h>

// ------ Include Components ------
#include "servo.h"
// #include "tracker.h"
#include "audio.h"

// ------ Serial ------
SoftwareSerial xbee(10, 11); // DOUT - 10 (Rx) DIN - 11 (Tx)

// ------ Motor PWM ------
const int PWM_Motor = 3;
const int u0_motor = 77; // ~30% of 255 (stopped/idle)

// ------ States ------
enum class State {
  IDLE,       // No target, stopped
  ACQUIRE,    // Target just seen
  TRACK,      // Target centered, driving forward
  SEARCH,     // Target lost, looking around
  STOP        // E-stop
};

State currentState = State::IDLE;
State prevState = State::IDLE;

void setup() {
  // 1. Initialize Serial (Fixed Baud Rate)
  Serial.begin(115200); 
  xbee.begin(115200);
  
  // 2. Initialize Motor
  pinMode(PWM_Motor, OUTPUT);
  analogWrite(PWM_Motor, u0_motor); // Make sure car is stopped on boot

  // 3. Initialize Components
  //tracker::begin(); 
  audio::begin(); 
  servo::begin(); 
  
  Serial.println(F("Goose Robot Ready!"));
}

void loop() {
  // ------ 1. Update Inputs/Sensors ------
  //tracker::update();
  audio::update();
  servo::update();

  // ------ 2. Decide what to do (State Machine) ------
  /* (Assuming tracker::targetVisible() returns true if it sees the face)
  bool seesTarget = tracker::targetVisible(); 

  switch (currentState) {
    case State::IDLE:
      if (seesTarget) currentState = State::ACQUIRE;
      break;

    case State::ACQUIRE:
      // Just saw the target, transition to tracking
      currentState = State::TRACK;
      break;

    case State::TRACK:
      if (!seesTarget) {
        currentState = State::SEARCH;
      }
      break;

    case State::SEARCH:
      if (seesTarget) {
        currentState = State::TRACK;
      }
      // (Optional: Add a timeout here to go back to IDLE if lost for 10 seconds)
      break;

    case State::STOP:
      // Do nothing, wait for reset
      break;
  }

  // ------ 3. Play Sounds on State Change ------
  if (currentState != prevState) {
    switch (currentState) {
      case State::ACQUIRE: audio::playSuccess();  break; // The "Success Ding"
      case State::SEARCH:  audio::playHonk2();    break; // The "Confused" honk
      case State::STOP:    audio::playHonk4();    break; // Emergency alarm honk
      default: break;
    }
    prevState = currentState;
  }

  // ------ 4. Act on the current state (Motors & Servos) ------
  switch (currentState) {
    case State::IDLE:
    case State::SEARCH:
    case State::STOP:
      analogWrite(PWM_Motor, u0_motor); // Stop moving
      break;

    case State::TRACK:
      analogWrite(PWM_Motor, u0_motor + 40); // Drive forward (tune this value!)
      break;
      
    case State::ACQUIRE:
      analogWrite(PWM_Motor, u0_motor); // Stop while we transition
      break;
  }
  */

  // ------ 5. Small delay ------
  delay(50); // Reduced from 100ms so the robot reacts faster
}