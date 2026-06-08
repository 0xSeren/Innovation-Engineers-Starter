/*
 * ======================================================================
 *  EXERCISE 0 -- TELEOP: Drive the Rover Yourself      (Day 0: Warm-up)
 * ======================================================================
 *  Before the rover thinks for itself, YOU drive it -- with the kit's IR
 *  REMOTE. In real robotics competitions (FTC / FRC -- the Bull City Robotics
 *  connection) every match has a driver-controlled "TeleOp" period and a
 *  self-driving "Autonomous" period. This is your TeleOp; the rest of the
 *  course builds the Autonomous.
 *
 *  HOW IT WORKS: point the remote at the receiver (the little dome on the
 *  board, wired to pin 9) and press:
 *        ^ (up) = forward   v (down) = back
 *        < (left) = spin left   > (right) = spin right   OK = stop
 *  HOLD a button to keep moving; let go and the rover stops on its own.
 *
 *  LEARNING GOALS
 *    - The control loop: READ an input -> DECIDE -> ACT on the motors.
 *    - Receiving and decoding an IR signal (each button = a number code).
 *    - Driving the rover before automating it.
 *
 *  WHAT YOU DO: implement the button->motion mapping in handle() (the TODO).
 *  Everything else (motors, IR decode, hold-to-drive) is provided.
 *
 *  REMOTE VERSIONS: the kit ships one of two remote types, so BOTH code sets
 *  are listed below and isBtn() accepts either. If your buttons do nothing,
 *  uncomment the Serial.println in loop() to print YOUR remote's actual codes,
 *  then drop them into the #defines.
 * ======================================================================
 */
#include "IRremote.h"          // bundled in this folder (ELEGOO's IRremote 2.x)

#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define PIN_IR   9             // IR receiver

// Button codes -- remote version A (NEC) and version B:
#define A_UP   16736925UL
#define A_DOWN 16754775UL
#define A_LEFT 16720605UL
#define A_RIGHT 16761405UL
#define A_OK   16712445UL
#define B_UP   5316027UL
#define B_DOWN 2747854299UL
#define B_LEFT 1386468383UL
#define B_RIGHT 553536955UL
#define B_OK   3622325019UL
#define IR_REPEAT 0xFFFFFFFFUL // sent over and over while a button is HELD

IRrecv irrecv(PIN_IR);
decode_results results;
int speed = 150;
unsigned long lastButton = 0, lastSeen = 0;

// Provided for you (you build this yourself in Exercise 1).
void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

// true if `code` is this button on EITHER remote version
bool isBtn(unsigned long code, unsigned long a, unsigned long b) { return code == a || code == b; }

void handle(unsigned long code) {
  // -------------------------------------------------------------------
  // TODO: Turn the button code into a motor command, using isBtn():
  //   if      (isBtn(code, A_UP,    B_UP))    setMotors(true,  speed, true,  speed); // forward
  //   else if (isBtn(code, A_DOWN,  B_DOWN))  setMotors(false, speed, false, speed); // back
  //   else if (isBtn(code, A_LEFT,  B_LEFT))  setMotors(true,  speed, false, speed); // spin left
  //   else if (isBtn(code, A_RIGHT, B_RIGHT)) setMotors(false, speed, true,  speed); // spin right
  //   else if (isBtn(code, A_OK,    B_OK))    stopMotors();
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  irrecv.enableIRIn();          // start the IR receiver
  Serial.println(F("TELEOP ready -- press the remote arrows, OK = stop."));
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long code = results.value;
    irrecv.resume();            // ready for the next signal
    // Serial.println(code, HEX);   // <-- uncomment to discover YOUR remote's codes
    if (code != IR_REPEAT) lastButton = code;   // a held button repeats; reuse the last real press
    lastSeen = millis();
    handle(lastButton);
  }
  if (millis() - lastSeen > 160) stopMotors();  // button released -> stop (hold-to-drive)
}
