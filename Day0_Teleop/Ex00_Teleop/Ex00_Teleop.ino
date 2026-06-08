/*
 * ======================================================================
 *  EXERCISE 0 -- TELEOP: Drive the Rover Yourself      (Day 0: Warm-up)
 * ======================================================================
 *  Before the rover thinks for itself, YOU drive it. In real robotics
 *  competitions (FTC / FRC -- the Bull City Robotics connection) every match
 *  has two periods: a driver-controlled "TeleOp" period and a self-driving
 *  "Autonomous" period. This is your TeleOp. The whole rest of the course
 *  builds the Autonomous.
 *
 *  HOW IT WORKS: open the Serial Monitor (9600 baud, set line ending to
 *  "No line ending"). Type a key and press Send -- the keyboard is your
 *  gamepad:
 *        w = forward   s = back   a = spin left   d = spin right
 *        x or space = stop        + / - = faster / slower
 *
 *  LEARNING GOALS
 *    - The control loop: READ an input -> DECIDE -> ACT on the motors.
 *    - Reading characters from the Serial port.
 *    - Getting comfortable driving the rover before automating it.
 *
 *  WHAT YOU DO: implement the command parser in the TODO inside loop().
 *  (setMotors is provided so you can drive on day one.)
 *
 *  HARDWARE NOTE: this teleop uses the USB cable. The kit's IR remote is an
 *  untethered alternative -- see the stretch goal at the bottom.
 * ======================================================================
 */
#define PIN_PWMA 5      // right motor speed
#define PIN_PWMB 6      // left  motor speed
#define PIN_AIN1 7      // right motor direction
#define PIN_BIN1 8      // left  motor direction
#define PIN_STBY 3      // TB6612 standby (HIGH = enabled)

int speed = 150;        // current drive speed, 0..255 (+/- changes it)

// Provided for you (you will build this yourself in Exercise 1).
void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  Serial.println(F("TELEOP ready. Keys: w a s d, x=stop, +/- speed."));
}

void loop() {
  if (!Serial.available()) return;     // nothing typed yet -> keep doing nothing
  char c = Serial.read();

  // -------------------------------------------------------------------
  // TODO: Turn the key `c` into a motor command. A switch statement is clean:
  //   switch (c) {
  //     case 'w': setMotors(true,  speed, true,  speed); break;  // forward
  //     case 's': setMotors(false, speed, false, speed); break;  // back
  //     case 'a': setMotors(true,  speed, false, speed); break;  // spin left
  //     case 'd': setMotors(false, speed, true,  speed); break;  // spin right
  //     case 'x': case ' ': stopMotors(); break;
  //     case '+': speed = constrain(speed + 20, 0, 255); break;
  //     case '-': speed = constrain(speed - 20, 0, 255); break;
  //   }
  //   Then print the speed so the driver can see it.
  // -------------------------------------------------------------------

  // SAFETY STRETCH: a real gamepad sends commands continuously; a single typed
  // key here latches until you send another. Add a "dead-man" timeout that
  // stops the motors if no key arrives for, say, 600 ms (track millis() of the
  // last command and stopMotors() once it expires).

  // IR-REMOTE STRETCH: the kit's IR remote (receiver on pin 9) is an untethered
  // teleop. Decode it with the IRremote library and map its arrow buttons to
  // the same four motions. (The reference firmware in
  // ~/Arduino/ElegooSmartCarV4 lists the button codes.)
}
