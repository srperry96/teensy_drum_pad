#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Wag1");
  delay(250);
}