/*
 *  EXERCISE 5 -- Ultrasonic Sensor Science Lab  (SOLUTION)
 */
#define PIN_TRIG 13
#define PIN_ECHO 12

float readDistanceCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  if (us == 0) return 400.0;
  return us / 58.0;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(9600);
  Serial.println(F("Ultrasonic Science Lab -- distance (cm):"));
}

void loop() {
  // average + spread, for the data table / noise discussion
  const int N = 10;
  float sum = 0, lo = 1e9, hi = -1e9;
  for (int i = 0; i < N; i++) {
    float d = readDistanceCm();
    sum += d; if (d < lo) lo = d; if (d > hi) hi = d;
    delay(20);
  }
  Serial.print(F("avg=")); Serial.print(sum / N);
  Serial.print(F("  spread=")); Serial.println(hi - lo);
  delay(300);
}
