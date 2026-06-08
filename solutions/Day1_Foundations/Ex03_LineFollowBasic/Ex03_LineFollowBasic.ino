/*
 *  EXERCISE 3 -- Line Following (basic, bang-bang)  (SOLUTION)
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
#define BASE 120
#define TURN 150
#define SLOW 40

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

bool onLine(int pin) { return analogRead(pin) > LINE_THRESHOLD; }

void followLine() {
  bool L = onLine(PIN_L), M = onLine(PIN_M), R = onLine(PIN_R);
  if (M && !L && !R)        setMotors(true, BASE, true, BASE);   // straight
  else if (L && !R)         setMotors(true, TURN, true, SLOW);   // steer left
  else if (R && !L)         setMotors(true, SLOW, true, TURN);   // steer right
  else if (!L && !M && !R)  stopMotors();                        // lost the line
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  delay(1000);
}

void loop() { followLine(); }
