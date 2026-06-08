/*
 * ======================================================================
 *  EXERCISE 3 -- Line Following (basic)          (Day 1: Rover Foundations)
 * ======================================================================
 *  The rover follows a dark line on the floor using its three downward
 *  ITR20001 reflectance sensors. This is the "bang-bang" version: each sensor
 *  is just ON the line or OFF it, and we react. It will wobble -- that wobble
 *  is exactly what the PID controller in Exercise 4 smooths out.
 *
 *  THE LOGIC (dark line reads HIGH on these sensors):
 *      middle on, sides off   -> go straight
 *      left sees the line     -> steer left
 *      right sees the line    -> steer right
 *      nothing sees the line  -> you've run off; stop (or search)
 *
 *  LEARNING GOALS
 *    - Reading the line sensors and thresholding them to on/off.
 *    - A first closed-loop behavior: the sensor decides the motors.
 *    - Feeling WHY bang-bang wobbles -> motivates PID next.
 *
 *  WHAT YOU DO: implement onLine() and the steering logic in followLine().
 *  Sensors: A2 = Left, A1 = Middle, A0 = Right.
 * ======================================================================
 */
#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

#define LINE_THRESHOLD 500   // above this = sensor sees the dark line (tune in Ex8)
#define BASE  120            // straight speed
#define TURN  150            // outer-wheel speed in a correction
#define SLOW  40             // inner-wheel speed in a correction

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

bool onLine(int pin) {
  // -------------------------------------------------------------------
  // TODO 1: Return true if this sensor is over the dark line.
  //   return analogRead(pin) > LINE_THRESHOLD;
  //   (If YOUR line is light on a dark floor, flip the comparison.)
  // -------------------------------------------------------------------
  return false;
}

void followLine() {
  bool L = onLine(PIN_L), M = onLine(PIN_M), R = onLine(PIN_R);
  // -------------------------------------------------------------------
  // TODO 2: Steer based on which sensors see the line.
  //   if (M && !L && !R)  setMotors(true, BASE, true, BASE);   // straight
  //   else if (L && !R)   setMotors(true, TURN, true, SLOW);   // steer left
  //   else if (R && !L)   setMotors(true, SLOW, true, TURN);   // steer right
  //   else if (!L && !M && !R) stopMotors();                   // lost the line
  //   (Watch how much it wobbles on straights -- remember it for Exercise 4.)
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  followLine();
}
