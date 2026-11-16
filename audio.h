#pragma once
#include <Arduino.h>

namespace audio {

// Wiring: Arduino pin -> Sound Board trigger pin
constexpr int PIN_HONK_1  = 2;   // -> #0 (T00.WAV)
constexpr int PIN_HONK_2  = 3;   // -> #1 (T01.WAV)
constexpr int PIN_HONK_3  = 4;   // -> #2 (T02.WAV)
constexpr int PIN_HONK_4  = 5;   // -> #3 (T03.WAV)
constexpr int PIN_SUCCESS = 6;   // -> #4 (T04.WAV) (The "success ding")

constexpr unsigned long TRIGGER_PULSE_MS = 150;

void begin();
void update(); // MUST be called every loop()

void playHonk1();
void playHonk2();
void playHonk3();
void playHonk4();
void playSuccess();

} // namespace audio