/*
 *  EXERCISE 0 -- TELEOP  (SOLUTION)
 *  Drive the rover with the keyboard over Serial. See the starter for the
 *  full explanation; this is a worked answer.
 */
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

int speed = 150;

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
  if (!Serial.available()) return;
  char c = Serial.read();
  switch (c) {
    case 'w': setMotors(true,  speed, true,  speed); break;   // forward
    case 's': setMotors(false, speed, false, speed); break;   // back
    case 'a': setMotors(true,  speed, false, speed); break;   // spin left
    case 'd': setMotors(false, speed, true,  speed); break;   // spin right
    case 'x': case ' ': stopMotors(); break;
    case '+': speed = constrain(speed + 20, 0, 255); Serial.print(F("speed=")); Serial.println(speed); break;
    case '-': speed = constrain(speed - 20, 0, 255); Serial.print(F("speed=")); Serial.println(speed); break;
  }
}
