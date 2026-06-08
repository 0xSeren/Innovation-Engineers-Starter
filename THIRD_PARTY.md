# Third-party code

Some exercises (the gyro labs in Day 4 and the Trémaux maze solver in Day 5)
include IMU driver files so the starter sketches compile self-contained:

- `MPU6050.cpp/.h`, `I2Cdev.cpp/.h` — from **I2Cdevlib** by Jeff Rowberg
  (<https://github.com/jrowberg/i2cdevlib>), MIT License.
- `MPU6050_getdata.cpp/.h` — ELEGOO's thin yaw-integration wrapper, as shipped
  in the Smart Robot Car V4.0 example package.

These are redistributed here only to keep the teaching exercises buildable on an
Arduino UNO. All original copyright and license notices are retained in the
files. The course material itself (exercise sketches, README, teaching guide) is
the only original content.
