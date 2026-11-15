#include "audio.h"

namespace audio {

struct Channel {
  int           pin;
  bool          active;
  unsigned long startMs;
};

static Channel chHonk      = { PIN_TRIG_HONK,      false, 0 };
static Channel chFoundYou  = { PIN_TRIG_FOUND_YOU, false, 0 };
static Channel chConfused  = { PIN_TRIG_CONFUSED,  false, 0 };
static Channel chHappy     = { PIN_TRIG_HAPPY,     false, 0 };

static void initChannel(Channel &c) {
  pinMode(c.pin, OUTPUT);
  digitalWrite(c.pin, HIGH);   // inactive (pull-up on board keeps it idle)
  c.active  = false;
  c.startMs = 0;
}

static void fire(Channel &c) {
  digitalWrite(c.pin, LOW);
  c.active  = true;
  c.startMs = millis();
}

static void service(Channel &c) {
  if (c.active && (millis() - c.startMs >= TRIGGER_PULSE_MS)) {
    digitalWrite(c.pin, HIGH);
    c.active = false;
  }
}

void begin() {
  initChannel(chHonk);
  initChannel(chFoundYou);
  initChannel(chConfused);
  initChannel(chHappy);
}

void honk()      { fire(chHonk); }
void foundYou()  { fire(chFoundYou); }
void confused()  { fire(chConfused); }
void happy()     { fire(chHappy); }

void update() {
  service(chHonk);
  service(chFoundYou);
  service(chConfused);
  service(chHappy);
}

} // namespace audio