/*
 * ======================================================================
 *  EXERCISE 10 -- Reading the Gyro (raw)           (Day 4: Gyro & Fusion)
 * ======================================================================
 *  The MPU6050 is an IMU: a 3-axis GYROSCOPE (angular RATE, deg/sec) plus a
 *  3-axis ACCELEROMETER. For steering we care about the Z gyro axis = how fast
 *  the rover is rotating (yaw rate).
 *
 *  Run this and DON'T MOVE the rover. Two things to notice:
 *    - The reading is not exactly 0 even when still -> that constant error is
 *      the BIAS (a.k.a. zero-rate offset). Exercise 11 removes it.
 *    - The reading jitters a little every sample -> that is NOISE.
 *
 *  LEARNING GOALS
 *    - Talking to an I2C sensor (Wire + the MPU6050 library).
 *    - Raw counts vs physical units: divide raw Z by 131 to get deg/sec
 *      (the gyro's default +/-250 deg/s range = 131 counts per deg/s).
 *
 *  WHAT YOU DO: implement readRateDps().
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 imu;

float readRateDps() {
  // -------------------------------------------------------------------
  // TODO: Return the yaw RATE in degrees/second.
  //   int16_t raw = imu.getRotationZ();   // raw gyro Z counts
  //   return raw / 131.0;                 // counts -> deg/sec
  // -------------------------------------------------------------------
  return 0;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  imu.initialize();
  Serial.println(imu.testConnection() ? F("MPU6050 OK") : F("MPU6050 NOT found"));
  Serial.println(F("Hold the rover STILL and watch the bias + noise:"));
}

void loop() {
  Serial.println(readRateDps(), 3);   // deg/sec; should hover near (but not at) 0
  delay(50);
}
