/*
 *  EXERCISE 12 -- Sensor Fusion: the Complementary Filter  (SOLUTION)
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 imu;
float pitch = 0;
float gyroOnly = 0;
unsigned long lastUs = 0;
const float A = 0.98;

float accelPitch(int16_t ax, int16_t az) {
  return atan2((float)ax, (float)az) * 180.0 / PI;
}

void updatePitch() {
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float gyroRate = gx / 131.0;
  unsigned long now = micros();
  float dt = (now - lastUs) / 1000000.0;
  lastUs = now;
  gyroOnly += gyroRate * dt;
  float aPitch = accelPitch(ax, az);
  pitch = A * (pitch + gyroRate * dt) + (1.0 - A) * aPitch;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  imu.initialize();
  lastUs = micros();
  delay(50);
}

void loop() {
  updatePitch();
  Serial.print(gyroOnly, 2); Serial.print('\t');
  Serial.println(pitch, 2);
  delay(20);
}
