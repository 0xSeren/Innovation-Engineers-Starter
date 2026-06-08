/*
 * ======================================================================
 *  EXERCISE 1 -- Motor Control Basics            (Day 1: Rover Foundations)
 * ======================================================================
 *  Mission tie-in (Mars Rover Camp, Day 1 Session IV "Programming
 *  Foundations"): "Every rover moves." Before a rover can explore Mars it
 *  must drive forward, back, turn, and stop on command.
 *
 *  LEARNING GOALS
 *    - How an H-bridge motor driver (TB6612) is controlled: a DIRECTION pin
 *      and a PWM SPEED pin per motor, plus a STANDBY enable.
 *    - PWM (analogWrite, 0..255) = average voltage = speed.
 *
 *  HARDWARE (ELEGOO Smart Robot Car V4.0, TB6612 board)
 *    Motor A = RIGHT, Motor B = LEFT.
 *
 *  WHAT YOU DO: fill in the three TODO blocks. When done, the rover should
 *  run the demo sequence in loop(): forward, back, spin right, spin left, stop.
 * ======================================================================
 */
#define PIN_PWMA 5   // right speed (PWM)
#define PIN_PWMB 6   // left  speed (PWM)
#define PIN_AIN1 7   // right direction
#define PIN_BIN1 8   // left  direction
#define PIN_STBY 3   // TB6612 standby: HIGH = motors enabled

void setup() {
  // -------------------------------------------------------------------
  // TODO 1: Configure the motor pins.
  //   Make PIN_PWMA, PIN_PWMB, PIN_AIN1, PIN_BIN1, PIN_STBY all OUTPUTs.
  //   (Use pinMode(pin, OUTPUT) for each.)
  // -------------------------------------------------------------------

  Serial.begin(9600);
}

/*
 * setMotors(): the one function every later exercise reuses.
 *   rFwd / lFwd : true = that wheel spins forward, false = backward
 *   rSpd / lSpd : 0..255 speed for right / left wheel
 */
void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  // -------------------------------------------------------------------
  // TODO 2: Drive the TB6612.
  //   1. Enable the driver:           digitalWrite(PIN_STBY, HIGH);
  //   2. Set RIGHT direction from rFwd: digitalWrite(PIN_AIN1, rFwd?HIGH:LOW);
  //      Set RIGHT speed:               analogWrite(PIN_PWMA, rSpd);
  //   3. Do the same for the LEFT motor using PIN_BIN1 / PIN_PWMB and lFwd/lSpd.
  //   Tip: clamp speeds with constrain(x, 0, 255) so bad input can't wrap.
  // -------------------------------------------------------------------
}

void stopMotors() {
  // -------------------------------------------------------------------
  // TODO 3: Stop both motors (set both PWMs to 0) and pull PIN_STBY LOW.
  // -------------------------------------------------------------------
}

void loop() {
  setMotors(true, 150, true, 150);    delay(1000);  // forward
  stopMotors();                       delay(500);
  setMotors(false, 150, false, 150);  delay(1000);  // backward
  stopMotors();                       delay(500);
  setMotors(true, 150, false, 150);   delay(700);   // spin right (R fwd, L back)
  setMotors(false, 150, true, 150);   delay(700);   // spin left
  stopMotors();                       delay(2000);
}
