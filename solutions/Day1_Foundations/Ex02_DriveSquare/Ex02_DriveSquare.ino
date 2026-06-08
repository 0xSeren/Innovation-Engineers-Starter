/*
 *  EXERCISE 2 -- Drive a Square (open-loop)  (SOLUTION)
 */
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define SPEED    150
#define MS_PER_CM        20
#define MS_PER_90_TURN  450

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void forwardCm(float cm) {
  setMotors(true, SPEED, true, SPEED);
  delay((unsigned long)(cm * MS_PER_CM));
  stopMotors();
}
void turnRight90() {
  setMotors(false, SPEED, true, SPEED);   // right back, left forward = spin right
  delay(MS_PER_90_TURN);
  stopMotors();
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  delay(1500);
}

void loop() {
  for (int side = 0; side < 4; side++) {
    forwardCm(50);  delay(300);
    turnRight90();  delay(300);
  }
  stopMotors();
  while (true) {}
}
