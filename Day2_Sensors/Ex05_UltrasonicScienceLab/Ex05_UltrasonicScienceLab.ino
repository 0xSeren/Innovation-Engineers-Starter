/*
 * ======================================================================
 *  EXERCISE 5 -- Ultrasonic Sensor Science Lab    (Day 2: Sensor Science)
 * ======================================================================
 *  Mars Rover Camp, Day 2 Session I "Sensor Science Lab": investigate the
 *  ultrasonic sensor's range, detection angle, and accuracy. Collect data,
 *  then analyze it.
 *
 *  LEARNING GOALS
 *    - How an HC-SR04 measures distance: send a 10 us trigger pulse, time how
 *      long the ECHO stays HIGH, convert microseconds -> centimeters (/58).
 *    - Real sensors have limits: a minimum range, a maximum range, a beam
 *      WIDTH (it can't see thin/angled targets), and noise.
 *
 *  LAB PROCEDURE (fill the camp data table):
 *    Put a flat target at 10, 20, 30, 40 cm. At each distance take 10 readings
 *    and record how many were within +/-2 cm ("detection success"). Then try a
 *    target at an angle and off to the side to find the beam width.
 *
 *      | Distance | Detection Success (/10) |
 *      |   10 cm  |                         |
 *      |   20 cm  |                         |
 *      |   30 cm  |                         |
 *      |   40 cm  |                         |
 *
 *  WHAT YOU DO: implement readDistanceCm(). The Serial Monitor (9600) prints a
 *  reading twice a second so you can fill the table.
 * ======================================================================
 */
#define PIN_TRIG 13
#define PIN_ECHO 12

float readDistanceCm() {
  // -------------------------------------------------------------------
  // TODO 1: One HC-SR04 ping.
  //   1. PIN_TRIG LOW for 2 us, then HIGH for 10 us, then LOW.
  //      (digitalWrite + delayMicroseconds)
  //   2. unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  //      (25000 us timeout ~= 4.3 m, so it can't hang forever)
  //   3. If us == 0 (no echo) return a large number like 400.0 ("nothing near").
  //   4. Otherwise return us / 58.0   (speed-of-sound round-trip -> cm).
  // -------------------------------------------------------------------
  return 400.0;   // placeholder so the sketch compiles before you start
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(9600);
  Serial.println(F("Ultrasonic Science Lab -- distance (cm):"));
}

void loop() {
  float d = readDistanceCm();
  Serial.println(d);
  delay(500);

  // STRETCH GOAL: take N readings in a tight loop, print the average AND the
  // spread (max-min). Which distance is noisiest? That informs the filter you
  // build in Exercise 9.
}
