/*
 * ======================================================================
 *  EXERCISE 7 -- Autonomous Obstacle Avoidance    (Day 2: Sensor Science)
 * ======================================================================
 *  Mars Rover Camp, Day 2 Session II "Autonomous Programming" + Session III
 *  "Mars Exploration Course": the rover drives itself, avoiding boulders
 *  (cones / pool noodles) without a driver.
 *
 *  This is your first true SENSE -> THINK -> ACT loop:
 *    SENSE  read the distance ahead
 *    THINK  far  -> keep going;  near -> stop, look left & right, pick the
 *           more open side
 *    ACT    drive forward, or turn toward the open side
 *
 *  LEARNING GOALS
 *    - Sensor THRESHOLDS (how near is "too near?") and how tuning one constant
 *      changes the whole behavior. (The camp literally says: "Modify speed,
 *      turning behavior, and sensor thresholds.")
 *
 *  WHAT YOU DO: implement the avoidance decision in loop()'s TODO block.
 *  setMotors / readDistanceCm / lookAt are provided from earlier exercises.
 * ======================================================================
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
#define STOP_DISTANCE 25     // cm: closer than this = obstacle  <-- TUNE ME

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

  // -------------------------------------------------------------------
  // TODO: Obstacle-avoidance decision.
  //   if ahead > STOP_DISTANCE:
  //        drive forward at CRUISE_SPEED and return.
  //   else (obstacle!):
  //     1. stopMotors()
  //     2. float left  = lookAt(150);   // look left
  //        float right = lookAt(30);    // look right
  //     3. head.write(90);              // recenter for driving
  //     4. if (left > right)  spin LEFT  for ~400 ms (right wheel fwd, left back)
  //        else                spin RIGHT for ~400 ms
  //     5. stopMotors()
  //   STRETCH: if BOTH sides are blocked, back up before turning.
  // -------------------------------------------------------------------
}
