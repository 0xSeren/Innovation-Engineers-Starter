/*
 * ======================================================================
 *  EXERCISE 11 -- Gyro Bias Calibration & Integration (Day 4: Gyro & Fusion)
 * ======================================================================
 *  A gyro measures RATE (deg/sec). To get an ANGLE (heading) we INTEGRATE:
 *  add up rate * time. But if there is even a tiny leftover bias, integrating
 *  it makes the heading slowly crawl away from the truth -- gyro DRIFT.
 *
 *  Two-part fix you implement here:
 *    1. CALIBRATE the bias: average many samples while the rover is dead still;
 *       that average IS the zero-rate offset. Subtract it from every reading.
 *    2. INTEGRATE the corrected rate over real elapsed time (use millis(), not
 *       a fixed delay -- the loop period varies).
 *
 *  LEARNING GOALS
 *    - Bias estimation by averaging (the same trick ELEGOO's firmware uses).
 *    - Numerical integration: heading += rate * dt.
 *    - WHY it still drifts a little -> motivates fusion (Exercise 12) and
 *      closed-loop correction.
 *
 *  TEST: calibrate, then rotate the rover by hand exactly 90 deg and check the
 *  printed heading. Then leave it still for a minute and watch drift.
 *
 *  WHAT YOU DO: implement calibrateBias() and updateHeading().
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 imu;
float biasDps = 0;          // learned zero-rate offset (deg/sec)
float heading = 0;          // integrated heading (deg)
unsigned long lastUs = 0;

void calibrateBias() {
  // -------------------------------------------------------------------
  // TODO 1: Estimate the zero-rate bias. KEEP THE ROVER STILL.
  //   - take ~500 samples of imu.getRotationZ()
  //   - sum them, divide by 500 -> average raw bias
  //   - store biasDps = (average raw) / 131.0   (deg/sec)
  //   A small delay(2) between samples is fine.
  // -------------------------------------------------------------------
}

void updateHeading() {
  // -------------------------------------------------------------------
  // TODO 2: One integration step.
  //   1. float rate = imu.getRotationZ()/131.0 - biasDps;   // corrected dps
  //   2. unsigned long now = micros();
  //      float dt = (now - lastUs) / 1000000.0;             // seconds
  //      lastUs = now;
  //   3. heading += rate * dt;
  //   OPTIONAL: if (fabs(rate) < 0.5) skip adding (deadband kills tiny drift).
  // -------------------------------------------------------------------
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  imu.initialize();
  Serial.println(F("Calibrating gyro bias -- DO NOT MOVE..."));
  calibrateBias();
  Serial.print(F("bias = ")); Serial.print(biasDps, 4); Serial.println(F(" deg/s"));
  lastUs = micros();
}

void loop() {
  updateHeading();
  Serial.println(heading, 2);   // rotate the rover and watch this track angle
  delay(20);
}
