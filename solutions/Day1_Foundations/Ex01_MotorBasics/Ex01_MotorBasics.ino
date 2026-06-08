/*
 *  EXERCISE 1 -- Motor Control Basics  (SOLUTION)
 */
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  Serial.begin(9600);
}

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void loop() {
  setMotors(true, 150, true, 150);    delay(1000);  // forward
  stopMotors();                       delay(500);
  setMotors(false, 150, false, 150);  delay(1000);  // backward
  stopMotors();                       delay(500);
  setMotors(true, 150, false, 150);   delay(700);   // spin right
  setMotors(false, 150, true, 150);   delay(700);   // spin left
  stopMotors();                       delay(2000);
}
