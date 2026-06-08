/*
 *  EXERCISE 6 -- Servo Scan: "Look Around"  (SOLUTION)
 */
#include <Servo.h>
#define PIN_SERVO 10
#define PIN_TRIG  13
#define PIN_ECHO  12
Servo head;

float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  return us == 0 ? 400.0 : us / 58.0;
}

float lookAt(int angle) {
  head.write(angle);
  delay(160);                 // let the servo settle before reading
  return readDistanceCm();
}

int scanForClearest() {
  int best = 90; float bestDist = -1;
  for (int a = 30; a <= 150; a += 30) {
    float d = lookAt(a);
    Serial.print(a); Serial.print(F(": ")); Serial.println(d);
    if (d > bestDist) { bestDist = d; best = a; }
  }
  return best;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  head.attach(PIN_SERVO);
  head.write(90);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  int best = scanForClearest();
  Serial.print(F("Clearest direction = ")); Serial.println(best);
  head.write(90);
  delay(1500);
}
