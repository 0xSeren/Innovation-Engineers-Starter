/*
 *  EXERCISE 7 -- Autonomous Obstacle Avoidance  (SOLUTION)
 */
#include <Servo.h>
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define PIN_SERVO 10
#define PIN_TRIG 13
#define PIN_ECHO 12
#define CRUISE_SPEED  130
#define TURN_SPEED    130
#define STOP_DISTANCE 25
Servo head;

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }
float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  return us == 0 ? 400.0 : us / 58.0;
}
float lookAt(int angle) { head.write(angle); delay(160); return readDistanceCm(); }

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT); pinMode(PIN_ECHO, INPUT);
  head.attach(PIN_SERVO); head.write(90);
  Serial.begin(9600);
  delay(800);
}

void loop() {
  float ahead = lookAt(90);
  if (ahead > STOP_DISTANCE) {
    setMotors(true, CRUISE_SPEED, true, CRUISE_SPEED);
    return;
  }
  // obstacle: stop, look both ways, turn toward the open side
  stopMotors();
  float left  = lookAt(150);
  float right = lookAt(30);
  head.write(90);
  if (left < STOP_DISTANCE && right < STOP_DISTANCE) {
    setMotors(false, CRUISE_SPEED, false, CRUISE_SPEED); delay(400);  // both blocked: back up
  }
  if (left > right) setMotors(true,  TURN_SPEED, false, TURN_SPEED);   // spin left
  else              setMotors(false, TURN_SPEED, true,  TURN_SPEED);   // spin right
  delay(400);
  stopMotors();
}
