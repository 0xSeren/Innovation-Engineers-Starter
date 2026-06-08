/*
 *  EXERCISE 4 -- Line Following with PID  (SOLUTION)
 *  Kp/Kd values are a reasonable starting point; re-tune for your robot.
 */
#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define BASE 110
float Kp = 0.08;
float Kd = 0.50;
float lastError = 0;

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}

float readError() {
  // line to the right -> positive, to the left -> negative
  return (float)(analogRead(PIN_R) - analogRead(PIN_L));
}

void followLine() {
  float error = readError();
  float correction = Kp * error + Kd * (error - lastError);
  lastError = error;
  int left  = (int)(BASE + correction);
  int right = (int)(BASE - correction);
  setMotors(true, right, true, left);
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() { followLine(); }
