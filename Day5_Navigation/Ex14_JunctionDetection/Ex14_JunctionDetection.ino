/*
 * ======================================================================
 *  EXERCISE 14 -- Junction Detection                (Day 5: Navigation)
 * ======================================================================
 *  A maze is just line-following PLUS deciding what to do at JUNCTIONS. With
 *  only 3 sensors you cannot see a whole intersection at once, so you use the
 *  "capture then nudge" trick:
 *    - while following, an OUTER sensor going dark means a branch is here:
 *      record left/right exits AT THAT MOMENT.
 *    - then NUDGE forward a few cm and re-read the MIDDLE sensor to learn
 *      whether the STRAIGHT path continues, and whether all three are dark
 *      (a big filled square = the GOAL).
 *
 *  This exercise just CLASSIFIES and prints the junction type. Getting this
 *  solid makes Exercises 15 and 16 (the actual solvers) easy.
 *
 *  LEARNING GOALS
 *    - Why 3 sensors force a two-step (capture + nudge) detection.
 *    - Distinguishing left/right/straight/dead-end/cross/goal.
 *
 *  WHAT YOU DO: implement the capture and nudge logic in detectJunction().
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

#define LINE_THRESHOLD 500
#define LINE_IS_HIGH   1
#define BASE 100
#define CREEP 90
#define CM_PER_MS 0.020      // calibrate (Exercise 2 style)
#define NUDGE_CM 4.0

bool L() { bool h = analogRead(PIN_L) > LINE_THRESHOLD; return LINE_IS_HIGH ? h : !h; }
bool M() { bool h = analogRead(PIN_M) > LINE_THRESHOLD; return LINE_IS_HIGH ? h : !h; }
bool R() { bool h = analogRead(PIN_R) > LINE_THRESHOLD; return LINE_IS_HIGH ? h : !h; }
void setMotors(bool rF, int rS, bool lF, int lS) {
  rS = constrain(rS, 0, 255); lS = constrain(lS, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rF ? HIGH : LOW); analogWrite(PIN_PWMA, rS);
  digitalWrite(PIN_BIN1, lF ? HIGH : LOW); analogWrite(PIN_PWMB, lS);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }
void creepCm(float cm) {
  unsigned long ms = (unsigned long)(cm / CM_PER_MS), t0 = millis();
  while (millis() - t0 < ms) setMotors(true, CREEP, true, CREEP);
  stopMotors();
}

bool gLeft, gRight, gStraight, gGoal;

void detectJunction() {
  gLeft = gRight = gStraight = gGoal = false;
  // -------------------------------------------------------------------
  // TODO 1: Follow the line until something happens, capturing exits.
  //   Loop reading l=L(), m=M(), r=R():
  //     - if (l && r)            : cross/T   -> gLeft=gRight=true; break
  //     - if (!m && !l && !r)    : dead end  -> break (gLeft=gRight stay false)
  //     - if (!m && (l||r))      : a turn    -> gLeft=l; gRight=r; break
  //     - if (m && !l && !r)     : centered  -> drive straight; continue
  //     - else (m and one outer) : treat as a branch on that side -> set the
  //                                 flag and break (advanced: confirm it isn't
  //                                 just drift by creeping ~2 cm first)
  // -------------------------------------------------------------------

  // -------------------------------------------------------------------
  // TODO 2: Nudge past the junction, then classify straight/goal.
  //   creepCm(NUDGE_CM);
  //   bool l=L(), m=M(), r=R();
  //   if (l && m && r) gGoal = true;     // big filled square
  //   gStraight = m;                     // straight path continues?
  // -------------------------------------------------------------------
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  detectJunction();
  stopMotors();
  Serial.print(F("Junction -> "));
  if (gGoal) Serial.println(F("GOAL"));
  else {
    if (gLeft)     Serial.print(F("L "));
    if (gStraight) Serial.print(F("S "));
    if (gRight)    Serial.print(F("R "));
    if (!gLeft && !gStraight && !gRight) Serial.print(F("dead-end"));
    Serial.println();
  }
  delay(1500);   // pause so you can read it, then reposition and try another
}
