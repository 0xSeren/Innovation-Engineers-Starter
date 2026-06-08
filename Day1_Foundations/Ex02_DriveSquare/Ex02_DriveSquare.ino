/*
 * ======================================================================
 *  EXERCISE 2 -- Drive a Square (open-loop)       (Day 1: Rover Foundations)
 * ======================================================================
 *  Make the rover trace a 50 cm square using ONLY time (no sensors). This is
 *  "open-loop / dead-reckoning" control.
 *
 *  LEARNING GOALS
 *    - Turn distance and angle into motor-on TIME.
 *    - SEE the problem: the square never closes perfectly. Wheel slip, battery
 *      sag, and uneven motors accumulate error. THIS is why we add a gyro on
 *      Day 4 (Exercise 13 turns the same square into a closed-loop one).
 *
 *  WHAT YOU DO: fill in the timing constants and the two TODO motions, then
 *  watch the drift. Record how far off the corner is after 4 sides.
 * ======================================================================
 */
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define SPEED    150

// CALIBRATE these two by measuring the real rover (stopwatch + ruler):
#define MS_PER_CM        20    // ms of forward driving per 1 cm at SPEED
#define MS_PER_90_TURN  450    // ms of in-place spin for a 90-degree turn

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void forwardCm(float cm) {
  // -------------------------------------------------------------------
  // TODO 1: Drive straight forward `cm` centimeters.
  //   - both wheels forward at SPEED for (cm * MS_PER_CM) milliseconds
  //   - then stopMotors()
  // -------------------------------------------------------------------
}

void turnRight90() {
  // -------------------------------------------------------------------
  // TODO 2: Spin in place 90 degrees to the right.
  //   - right wheel BACKWARD, left wheel FORWARD at SPEED
  //   - for MS_PER_90_TURN milliseconds, then stopMotors()
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  delay(1500);
}

void loop() {
  for (int side = 0; side < 4; side++) {  // four sides of the square
    forwardCm(50);
    delay(300);
    turnRight90();
    delay(300);
  }
  stopMotors();
  while (true) {}   // stop after one square; reset to run again
}
