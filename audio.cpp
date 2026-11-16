#include "audio.h"

namespace audio {

struct Channel {
  int           pin;
  bool          active;
  unsigned long startMs;
};

// Set up the 5 channels
static Channel chHonk1   = { PIN_HONK_1,  false, 0 };
static Channel chHonk2   = { PIN_HONK_2,  false, 0 };
static Channel chHonk3   = { PIN_HONK_3,  false, 0 };
static Channel chHonk4   = { PIN_HONK_4,  false, 0 };
static Channel chSuccess = { PIN_SUCCESS, false, 0 };

static void initChannel(Channel &c) {
  pinMode(c.pin, OUTPUT);
  digitalWrite(c.pin, HIGH); // HIGH = Not triggered
  c.active = false;
}

static void fire(Channel &c) {
  // Only fire if it's not already firing
  if (!c.active) {
    digitalWrite(c.pin, LOW); // LOW = Trigger sound
    c.active = true;
    c.startMs = millis();
  }
}

static void service(Channel &c) {
  if (c.active && (millis() - c.startMs >= TRIGGER_PULSE_MS)) {
    digitalWrite(c.pin, HIGH); // Reset back to HIGH
    c.active = false;
  }
}

void begin() {
  initChannel(chHonk1);
  initChannel(chHonk2);
  initChannel(chHonk3);
  initChannel(chHonk4);
  initChannel(chSuccess);
}

void playHonk1()   { fire(chHonk1); }
void playHonk2()   { fire(chHonk2); }
void playHonk3()   { fire(chHonk3); }
void playHonk4()   { fire(chHonk4); }
void playSuccess() { fire(chSuccess); }

void update() {
  service(chHonk1);
  service(chHonk2);
  service(chHonk3);
  service(chHonk4);
  service(chSuccess);
}

} // namespace audio