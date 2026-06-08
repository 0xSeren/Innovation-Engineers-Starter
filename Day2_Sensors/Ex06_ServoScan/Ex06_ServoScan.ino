/*
 * ======================================================================
 *  EXERCISE 6 -- Servo Scan: "Look Around"        (Day 2: Sensor Science)
 * ======================================================================
 *  The ultrasonic sensor sits on a servo. By panning it we turn ONE forward
 *  sensor into a simple radar. This is the perception the rover uses to choose
 *  where to go in Exercise 7.
 *
 *  LEARNING GOALS
 *    - Driving a hobby servo with the Servo library (angle 0..180).
 *    - Servos need TIME to move -- you must delay before reading the sensor.
 *    - Scanning a field of view and picking the most open direction.
 *
 *  WHAT YOU DO: implement lookAt() and scanForClearest().
 * ======================================================================
 */
#include <Servo.h>
#define PIN_SERVO 10
#define PIN_TRIG  13
#define PIN_ECHO  12

Servo head;

// reuse your Exercise 5 solution
float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  return us == 0 ? 400.0 : us / 58.0;
}

float lookAt(int angle) {
  // -------------------------------------------------------------------
  // TODO 1: Point the servo at `angle`, WAIT for it to settle (~150 ms),
  //         then return readDistanceCm(). 90 = straight ahead.
  // -------------------------------------------------------------------
  return 0;   // placeholder
}

int scanForClearest() {
  // -------------------------------------------------------------------
  // TODO 2: Sweep the head from 30 to 150 degrees in steps (e.g. every 30).
  //   - measure the distance at each angle (use lookAt)
  //   - remember which angle had the LARGEST distance (most open space)
  //   - print "angle: distance" for each so you can see the radar sweep
  //   - return the clearest angle
  // -------------------------------------------------------------------
  return 90;  // placeholder
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
  Serial.print(F("Clearest direction = "));
  Serial.println(best);
  head.write(90);   // recenter
  delay(1500);
}
