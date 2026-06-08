/*
 *  EXERCISE 10 -- Reading the Gyro (raw)  (SOLUTION)
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 imu;

float readRateDps() {
  int16_t raw = imu.getRotationZ();
  return raw / 131.0;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  imu.initialize();
  Serial.println(imu.testConnection() ? F("MPU6050 OK") : F("MPU6050 NOT found"));
}

void loop() {
  Serial.println(readRateDps(), 3);
  delay(50);
}
