/*
 * ======================================================================
 *  EXERCISE 13 -- Closed-Loop Turn-by-Angle        (Day 4: Gyro & Fusion)
 * ======================================================================
 *  In Exercise 2 the rover turned by TIME and the square never closed. Now we
 *  turn by measured ANGLE using the calibrated gyro from Exercise 11. This is
 *  CLOSED-LOOP control: keep turning until the sensor says we got there.
 *
 *  Re-run the square from Exercise 2 but with turnAngle(90) and watch how much
 *  tighter it closes. THIS is the payoff of all the gyro work.
 *
 *  LEARNING GOALS
 *    - Open-loop (guess a time) vs closed-loop (measure and stop) control.
 *    - Reusing bias calibration + integration to act, not just to display.
 *
 *  WHAT YOU DO: implement turnAngle(). calibrateBias()/readRateDps() are
 *  provided (your Exercise 11 solutions, condensed).
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define TURN_SPEED 120

MPU6050 imu;
float biasDps = 0;

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }
float readRateDps() { return imu.getRotationZ() / 131.0 - biasDps; }
void calibrateBias() {
  long sum = 0; const int N = 500;
  for (int i = 0; i < N; i++) { sum += imu.getRotationZ(); delay(2); }
  biasDps = (sum / (float)N) / 131.0;
}

// Turn in place by `deg` degrees. Positive = one direction, negative = other.
void turnAngle(float deg) {
  // -------------------------------------------------------------------
  // TODO: Closed-loop in-place turn.
  //   1. float turned = 0;  unsigned long lastUs = micros();
  //   2. Decide spin direction from the sign of deg:
  //        deg > 0 -> spin one way  (e.g. right wheel back, left wheel fwd)
  //        deg < 0 -> spin the other way
  //   3. Loop until fabs(turned) >= fabs(deg):
  //        - keep the spin motors on
  //        - float rate = readRateDps();
  //        - unsigned long now = micros();
  //          turned += rate * (now - lastUs) / 1000000.0;   // integrate angle
  //          lastUs = now;
  //   4. stopMotors();
  //   Tip: slow TURN_SPEED near the end for less overshoot (stretch goal: P-control).
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Wire.begin();
  Serial.begin(9600);
  imu.initialize();
  Serial.println(F("Calibrating -- hold still..."));
  calibrateBias();
  delay(500);
}

void loop() {
  // Closed-loop square: should close FAR better than Exercise 2.
  for (int s = 0; s < 4; s++) {
    setMotors(true, 150, true, 150); delay(900);   // a side (still timed; that's fine)
    stopMotors(); delay(250);
    turnAngle(90);                                 // but the TURN is measured
    delay(250);
  }
  stopMotors();
  while (true) {}
}
