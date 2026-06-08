# Third-party code

Some exercises (the gyro labs in Day 4 and the Trémaux maze solver in Day 5)
include IMU driver files so the starter sketches compile self-contained:

- `MPU6050.cpp/.h`, `I2Cdev.cpp/.h` — from **I2Cdevlib** by Jeff Rowberg
  (<https://github.com/jrowberg/i2cdevlib>), MIT License.
- `MPU6050_getdata.cpp/.h` — ELEGOO's thin yaw-integration wrapper, as shipped
  in the Smart Robot Car V4.0 example package.
- `IRremote.cpp/.h`, `IRremoteInt.h` (in the Day 0 teleop exercise) — the
  **IRremote 2.x** library by Ken Shirriff et al.
  (<https://github.com/Arduino-IRremote/Arduino-IRremote>), LGPL-2.1. This is the
  exact version ELEGOO ships with the car firmware (byte-identical), so the
  decoded remote codes match the car. `IRremote.LICENSE.txt` is included
  alongside it. Bundling it means the teleop builds without installing anything.

These are redistributed here only to keep the teaching exercises buildable on an
Arduino UNO. All original copyright and license notices are retained in the
files. The course material itself (exercise sketches, README, teaching guide) is
the only original content.
