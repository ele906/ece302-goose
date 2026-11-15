#pragma once
#include <Arduino.h>

namespace audio {

// -------- Wiring: Arduino GPIO -> Sound Board trigger pin --------
constexpr int PIN_TRIG_HONK      = 2;   // -> Sound Board #0  (T00.WAV)
constexpr int PIN_TRIG_FOUND_YOU = 3;   // -> Sound Board #1  (T01.WAV)
constexpr int PIN_TRIG_CONFUSED  = 4;   // -> Sound Board #2  (T02.WAV)
constexpr int PIN_TRIG_HAPPY     = 5;   // -> Sound Board #3  (T03.WAV)

constexpr unsigned long TRIGGER_PULSE_MS = 150;

void begin();
void update();     // call every loop() — services non-blocking pulses

void honk();       // T00
void foundYou();   // T01
void confused();   // T02
void happy();      // T03

} // namespace audio