/*
 * ======================================================================
 *  EXERCISE 8 -- Line Sensor Calibration & Offsets   (Day 3: Calibration)
 * ======================================================================
 *  The three ITR20001 reflectance sensors do NOT read the same number over the
 *  same surface: each has its own gain and offset, and ambient light and ride
 *  height shift them. A single hard-coded threshold (like we used early on) is
 *  fragile. Real robots CALIBRATE.
 *
 *  THE IDEA (per-sensor min/max normalization):
 *    1. Sweep all three sensors across line AND floor, recording each sensor's
 *       own min and max raw value.
 *    2. At run time, map each raw reading to a normalized 0..1000 using ITS OWN
 *       min/max. Now all three are on the same scale regardless of their
 *       individual offset -- a threshold of 500 means the same thing for each.
 *
 *  LEARNING GOALS
 *    - "Offset" and "gain" differences between nominally identical sensors.
 *    - Calibration = learning each sensor's range, then normalizing it out.
 *    - map()/constrain() as the normalization tool.
 *
 *  WHAT YOU DO: implement calibrateSensors() and normalize().
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

int sensorPin[3] = { PIN_L, PIN_M, PIN_R };
int minVal[3], maxVal[3];     // learned per-sensor calibration

void spin(int spd) {  // gentle in-place spin so sensors sweep over line+floor
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, HIGH); analogWrite(PIN_PWMA, spd);   // right fwd
  digitalWrite(PIN_BIN1, LOW);  analogWrite(PIN_PWMB, spd);   // left back
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }

void calibrateSensors() {
  // -------------------------------------------------------------------
  // TODO 1: Learn each sensor's min and max.
  //   1. Seed: for each sensor i, minVal[i] = 1023; maxVal[i] = 0;
  //   2. For about 3 seconds, keep the robot spin()-ing so the three sensors
  //      pass over both the dark line and the light floor. In a tight loop:
  //        read each sensor (analogRead(sensorPin[i]))
  //        if reading < minVal[i] -> minVal[i] = reading
  //        if reading > maxVal[i] -> maxVal[i] = reading
  //   3. stopMotors() when the time is up.
  //   (Use millis() for the 3-second window; don't use delay() inside it.)
  // -------------------------------------------------------------------
}

int normalize(int i) {
  // -------------------------------------------------------------------
  // TODO 2: Return this sensor mapped to 0..1000 using ITS OWN min/max.
  //   int raw = analogRead(sensorPin[i]);
  //   int n   = map(raw, minVal[i], maxVal[i], 0, 1000);
  //   return constrain(n, 0, 1000);
  //   (After this, 0 = floor-like, 1000 = line-like, for ALL three sensors.)
  // -------------------------------------------------------------------
  return 0;   // placeholder
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  Serial.println(F("Calibrating -- place sensors over the line and let it spin..."));
  delay(1000);
  calibrateSensors();
  Serial.print(F("min: ")); Serial.print(minVal[0]); Serial.print(' '); Serial.print(minVal[1]); Serial.print(' '); Serial.println(minVal[2]);
  Serial.print(F("max: ")); Serial.print(maxVal[0]); Serial.print(' '); Serial.print(maxVal[1]); Serial.print(' '); Serial.println(maxVal[2]);
}

void loop() {
  // After calibration, raw values differ per sensor but NORMALIZED ones match.
  Serial.print(F("norm L/M/R: "));
  Serial.print(normalize(0)); Serial.print('\t');
  Serial.print(normalize(1)); Serial.print('\t');
  Serial.println(normalize(2));
  delay(200);
}
