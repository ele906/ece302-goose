#include <Wire.h>
#include <SoftwareSerial.h>

// ------ Include Components ------
#include "audio.h"
#include "servo.h"
// #include "tracker.h" // Commented out for now to just test audio

// ------ Serial ------
SoftwareSerial xbee(10, 11); // DOUT - 10 (Rx) DIN - 11 (Tx)

// ------ Motor PWM ------
const int PWM_Motor = 3;
const int u0_motor = 77; // ~30% of 255 (stopped/idle)

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Audio Test on Pins 3, 4, 5, 6, 7...");

  // Set all trigger pins to OUTPUT and HIGH (so they don't trigger randomly on boot)
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  
  delay(2000); // 2 seconds before the test starts
}

void loop() {
  Serial.println("--- Test Starting... ---\n");
  // --- Test Pin 3 ---
  Serial.println("Testing Pin 3 (Sound #0)");
  digitalWrite(3, LOW);   // Trigger
  delay(150);             // Hold low for 150ms (required by sound board)
  digitalWrite(3, HIGH);  // Reset
  delay(3000);            // Wait 3 seconds for sound to finish

  // --- Test Pin 4 ---
  Serial.println("Testing Pin 4 (Sound #1)");
  digitalWrite(4, LOW);   
  delay(150);             
  digitalWrite(4, HIGH);  
  delay(3000);            

  // --- Test Pin 5 ---
  Serial.println("Testing Pin 5 (Sound #2)");
  digitalWrite(5, LOW);   
  delay(150);             
  digitalWrite(5, HIGH);  
  delay(3000);            

  // --- Test Pin 6 ---
  Serial.println("Testing Pin 6 (Sound #3)");
  digitalWrite(6, LOW);   
  delay(150);             
  digitalWrite(6, HIGH);  
  delay(3000);            

  // --- Test Pin 7 ---
  Serial.println("Testing Pin 7 (Sound #4)");
  digitalWrite(7, LOW);   
  delay(150);             
  digitalWrite(7, HIGH);  
  delay(3000);            
  
  Serial.println("--- Test Loop Complete. Restarting... ---\n");
  delay(2000);
}