/*
 * ======================================================================
 *  EXERCISE 4 -- Line Following with PID         (Day 1: Rover Foundations)
 * ======================================================================
 *  Your first taste of real autonomy: the rover drives a taped line by itself.
 *  In robotics competitions (FTC/FRC) this is the "follow a line" reflex every
 *  auton is built on. A simple "if L sees line, nudge left" follower wobbles; a
 *  PID controller steers PROPORTIONALLY to how far off the line you are, and
 *  damps the wobble.
 *
 *      error      = how far the line is from center (- = left, + = right)
 *      correction = Kp*error  +  Kd*(error - lastError)      (PD is plenty here)
 *      leftSpeed  = BASE + correction
 *      rightSpeed = BASE - correction
 *
 *  Kp sets how hard it steers; Kd (the derivative term) anticipates and kills
 *  oscillation. (Ki is usually unnecessary for line following.)
 *
 *  LEARNING GOALS
 *    - Turning a sensor reading into a continuous ERROR signal.
 *    - Proportional vs derivative action; tuning Kp then Kd.
 *    - A fixed threshold gets you going now; you'll make the sensing robust
 *      with the per-sensor calibration in Exercise 8 (Lab 3), then come back.
 *
 *  WHAT YOU DO: implement readError() and the PD math in followLine().
 *  Sensors: A2 = Left, A1 = Middle, A0 = Right.
 * ======================================================================
 */
#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

#define BASE 110
float Kp = 0.08;     // <-- TUNE: start here, raise until it tracks, then back off
float Kd = 0.50;     // <-- TUNE: raise until the wobble damps out
float lastError = 0;

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}

float readError() {
  // -------------------------------------------------------------------
  // TODO 1: Produce a signed line-position error from the 3 sensors.
  //   Simple version (assumes dark line reads HIGH):
  //     int l = analogRead(PIN_L), r = analogRead(PIN_R);
  //     return (float)(r - l);   // line to the right -> positive, left -> negative
  //   Better version: normalize l and r with per-sensor min/max from Exercise 8
  //   before subtracting, so uneven sensors don't bias the error.
  // -------------------------------------------------------------------
  return 0;
}

void followLine() {
  float error = readError();
  // -------------------------------------------------------------------
  // TODO 2: PD control.
  //   float correction = Kp * error + Kd * (error - lastError);
  //   lastError = error;
  //   int left  = BASE + correction;
  //   int right = BASE - correction;
  //   setMotors(true, right, true, left);   // both wheels forward, steered
  //   (Make sure the sign matches your readError(); if it steers the WRONG
  //    way, swap left/right or negate correction.)
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  followLine();
}
