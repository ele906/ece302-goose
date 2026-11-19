#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setWireTimeout(3000, true);
  delay(1000);
  Serial.println("Scanning I2C...");
}

void loop() {
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("FOUND: 0x");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan finished");
  delay(2000);
}