/*
 *  EXERCISE 14 -- Junction Detection  (SOLUTION)
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
#define CM_PER_MS 0.020
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
  while (true) {                                  // TODO 1: follow + capture exits
    bool l = L(), m = M(), r = R();
    if (l && r)         { stopMotors(); gLeft = gRight = true; break; }   // cross / T
    if (!m && !l && !r) { stopMotors(); break; }                         // dead end
    if (!m && (l || r)) { stopMotors(); gLeft = l; gRight = r; break; }  // a turn
    if (m && !l && !r)  { setMotors(true, BASE, true, BASE); continue; } // centered
    if (m && l)         { gLeft = true; break; }                         // straight+left
    if (m && r)         { gRight = true; break; }                        // straight+right
  }
  creepCm(NUDGE_CM);                              // TODO 2: nudge, classify straight/goal
  bool l = L(), m = M(), r = R();
  if (l && m && r) gGoal = true;
  gStraight = m;
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
  delay(1500);
}
