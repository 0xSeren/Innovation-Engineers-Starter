/*
 *  EXERCISE 8 -- Line Sensor Calibration & Offsets  (SOLUTION)
 */
#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

int sensorPin[3] = { PIN_L, PIN_M, PIN_R };
int minVal[3], maxVal[3];

void spin(int spd) {
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, HIGH); analogWrite(PIN_PWMA, spd);
  digitalWrite(PIN_BIN1, LOW);  analogWrite(PIN_PWMB, spd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void calibrateSensors() {
  for (int i = 0; i < 3; i++) { minVal[i] = 1023; maxVal[i] = 0; }
  unsigned long t0 = millis();
  while (millis() - t0 < 3000) {        // sweep over line + floor for 3 s
    spin(110);
    for (int i = 0; i < 3; i++) {
      int v = analogRead(sensorPin[i]);
      if (v < minVal[i]) minVal[i] = v;
      if (v > maxVal[i]) maxVal[i] = v;
    }
  }
  stopMotors();
}

int normalize(int i) {
  int raw = analogRead(sensorPin[i]);
  int n = map(raw, minVal[i], maxVal[i], 0, 1000);
  return constrain(n, 0, 1000);
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  Serial.println(F("Calibrating..."));
  delay(1000);
  calibrateSensors();
  Serial.print(F("min: ")); Serial.print(minVal[0]); Serial.print(' '); Serial.print(minVal[1]); Serial.print(' '); Serial.println(minVal[2]);
  Serial.print(F("max: ")); Serial.print(maxVal[0]); Serial.print(' '); Serial.print(maxVal[1]); Serial.print(' '); Serial.println(maxVal[2]);
}

void loop() {
  Serial.print(F("norm L/M/R: "));
  Serial.print(normalize(0)); Serial.print('\t');
  Serial.print(normalize(1)); Serial.print('\t');
  Serial.println(normalize(2));
  delay(200);
}
