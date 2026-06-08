/*
 * ======================================================================
 *  EXERCISE 12 -- Sensor Fusion: the Complementary Filter (Day 4)
 * ======================================================================
 *  The gyro is smooth and fast but DRIFTS (Exercise 11). The accelerometer can
 *  measure a TILT angle absolutely (it feels gravity) but is NOISY and jumps
 *  when the rover accelerates. Each sensor is good exactly where the other is
 *  bad. SENSOR FUSION combines them so the result is both stable AND drift-free.
 *
 *  The COMPLEMENTARY FILTER -- the simplest, most-used fusion on small robots:
 *
 *      angle = A * (angle + gyroRate*dt)  +  (1 - A) * accelAngle
 *              \___ trust gyro short-term ___/   \__ trust accel long-term __/
 *
 *  with A near 0.98. The gyro carries the fast motion; the accel slowly pulls
 *  the estimate back to gravity so it can't drift away.
 *
 *  We fuse PITCH here (tilt up/down) because tilt has a gravity reference. NOTE:
 *  yaw/heading has NO gravity reference, which is exactly why the heading in
 *  Exercise 11 can only be helped by a magnetometer or by closed-loop correction
 *  (Exercise 13) -- a key lesson about what fusion can and cannot fix.
 *
 *  LEARNING GOALS
 *    - Complementary-filter fusion and the meaning of the blend constant A.
 *    - Computing a tilt angle from accelerometer components with atan2.
 *    - Each sensor covering the other's weakness.
 *
 *  WHAT YOU DO: implement accelPitch() and the fusion step in updatePitch().
 *  Tilt the rover slowly, then quickly, and compare gyro-only vs fused.
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 imu;
float pitch = 0;            // fused pitch angle (deg)
float gyroOnly = 0;         // gyro-integrated only, for comparison (drifts)
unsigned long lastUs = 0;
const float A = 0.98;       // complementary blend constant

float accelPitch(int16_t ax, int16_t az) {
  // -------------------------------------------------------------------
  // TODO 1: Pitch angle from gravity, in degrees.
  //   return atan2((float)ax, (float)az) * 180.0 / PI;
  //   (ax/az are raw accel counts; atan2 of two axes gives the tilt; the raw
  //    scale cancels inside atan2, so no need to convert to g.)
  // -------------------------------------------------------------------
  return 0;
}

void updatePitch() {
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float gyroRate = gx / 131.0;                 // deg/sec about the pitch axis
  unsigned long now = micros();
  float dt = (now - lastUs) / 1000000.0;
  lastUs = now;

  gyroOnly += gyroRate * dt;                   // reference: gyro alone (drifts)

  // -------------------------------------------------------------------
  // TODO 2: Complementary fusion.
  //   float aPitch = accelPitch(ax, az);
  //   pitch = A * (pitch + gyroRate * dt) + (1.0 - A) * aPitch;
  // -------------------------------------------------------------------
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
  // Serial Plotter columns: gyro-only (watch it drift) vs fused (stays true)
  Serial.print(gyroOnly, 2); Serial.print('\t');
  Serial.println(pitch, 2);
  delay(20);
}
