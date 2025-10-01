/*
  MIC1 Week 1. Basics

  Author Barna-Botond Janosi
         2170302
  Date   05/06/2025
*/

const int ledPins[] = {13, 12, 11, 10, 9, 8};

void setup() {
  // Configure pins and initial pin value
  for(int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}
//index indicates which item in the pin array is currently being handled
void loop() {
  for(int i = 0; i <= 5; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(200);
    digitalWrite(ledPins[i], LOW);
    //delay(200);
  }
  for(int i = 4; i >= 1; i--) {
    digitalWrite(ledPins[i], HIGH);
    delay(200);
    digitalWrite(ledPins[i], LOW);
    //delay(200);
  }
}
