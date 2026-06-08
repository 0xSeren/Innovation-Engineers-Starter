/*
 *  EXERCISE 13 -- Closed-Loop Turn-by-Angle  (SOLUTION)
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

void turnAngle(float deg) {
  float turned = 0;
  unsigned long lastUs = micros();
  while (fabs(turned) < fabs(deg)) {
    if (deg > 0) setMotors(false, TURN_SPEED, true,  TURN_SPEED);  // spin right (+)
    else         setMotors(true,  TURN_SPEED, false, TURN_SPEED);  // spin left  (-)
    float rate = readRateDps();
    unsigned long now = micros();
    turned += rate * (now - lastUs) / 1000000.0;
    lastUs = now;
  }
  stopMotors();
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
  for (int s = 0; s < 4; s++) {
    setMotors(true, 150, true, 150); delay(900);
    stopMotors(); delay(250);
    turnAngle(90);                          // sign matches readRateDps()/GYRO_SIGN
    delay(250);
  }
  stopMotors();
  while (true) {}
}
