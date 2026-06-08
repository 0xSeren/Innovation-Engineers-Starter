/*
 *  EXERCISE 9 -- Filtering a Noisy Sensor  (SOLUTION)
 */
#define PIN_TRIG 13
#define PIN_ECHO 12
#define N 5

float readRawCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  return us == 0 ? 400.0 : us / 58.0;
}

float movingAverage() {
  float sum = 0;
  for (int i = 0; i < N; i++) { sum += readRawCm(); delay(2); }
  return sum / N;
}

float medianOfN() {
  float buf[N];
  for (int i = 0; i < N; i++) { buf[i] = readRawCm(); delay(2); }
  for (int i = 0; i < N - 1; i++)             // simple bubble sort
    for (int j = 0; j < N - 1 - i; j++)
      if (buf[j] > buf[j + 1]) { float t = buf[j]; buf[j] = buf[j + 1]; buf[j + 1] = t; }
  return buf[N / 2];
}

float emaState = 0;
float exponential(float alpha) {
  float raw = readRawCm();
  emaState = emaState + alpha * (raw - emaState);
  return emaState;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(9600);
  emaState = readRawCm();
}

void loop() {
  Serial.print(readRawCm());     Serial.print('\t');
  Serial.print(movingAverage()); Serial.print('\t');
  Serial.print(medianOfN());     Serial.print('\t');
  Serial.println(exponential(0.3));
  delay(60);
}
