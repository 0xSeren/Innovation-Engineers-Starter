/*
 *  EXERCISE 11 -- Gyro Bias Calibration & Integration  (SOLUTION)
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 imu;
float biasDps = 0;
float heading = 0;
unsigned long lastUs = 0;

void calibrateBias() {
  const int N = 500;
  long sum = 0;
  for (int i = 0; i < N; i++) { sum += imu.getRotationZ(); delay(2); }
  biasDps = (sum / (float)N) / 131.0;
}

void updateHeading() {
  float rate = imu.getRotationZ() / 131.0 - biasDps;
  unsigned long now = micros();
  float dt = (now - lastUs) / 1000000.0;
  lastUs = now;
  if (fabs(rate) < 0.5) rate = 0;     // deadband kills tiny standing drift
  heading += rate * dt;
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
  Serial.println(heading, 2);
  delay(20);
}
