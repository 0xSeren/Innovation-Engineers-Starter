/*
 * ======================================================================
 *  EXERCISE 9 -- Filtering a Noisy Sensor          (Day 3: Calibration)
 * ======================================================================
 *  In Exercise 5 you saw the ultrasonic reading jump around and occasionally
 *  spit out a wild value (a "spike" / outlier). Acting on a single bad reading
 *  makes a rover swerve at ghosts. FILTERS smooth the signal.
 *
 *  You will implement and COMPARE three classic filters:
 *    1. Moving average  -- smooth, but lags and is dragged by spikes.
 *    2. Median-of-N     -- kills isolated spikes outright (great for sonar).
 *    3. Exponential (EMA / low-pass): out += alpha*(raw-out). One line, no
 *       buffer; alpha trades smoothness vs responsiveness.
 *
 *  LEARNING GOALS
 *    - Why raw sensor data is rarely used directly.
 *    - The smoothness/latency trade-off, and why MEDIAN beats MEAN for spikes.
 *    - The exponential filter -- the same math the complementary filter in
 *      Exercise 12 is built on.
 *
 *  WHAT YOU DO: implement the three TODO filters. The loop prints raw vs all
 *  three so you can watch them on the Serial Plotter (Tools -> Serial Plotter).
 * ======================================================================
 */
#define PIN_TRIG 13
#define PIN_ECHO 12
#define N 5                 // window size for average & median

float readRawCm() {
  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  return us == 0 ? 400.0 : us / 58.0;
}

float movingAverage() {
  // -------------------------------------------------------------------
  // TODO 1: Take N readRawCm() readings and return their MEAN.
  //   (Sum them in a loop, divide by N. A tiny delay between reads is fine.)
  // -------------------------------------------------------------------
  return 0;
}

float medianOfN() {
  // -------------------------------------------------------------------
  // TODO 2: Take N readings into a small float array, SORT it (a simple
  //   bubble/insertion sort is fine for N=5), and return the MIDDLE element
  //   buf[N/2]. The median ignores a single huge spike entirely.
  // -------------------------------------------------------------------
  return 0;
}

float emaState = 0;         // persists between calls
float exponential(float alpha) {
  // -------------------------------------------------------------------
  // TODO 3: One exponential-moving-average step.
  //   float raw = readRawCm();
  //   emaState = emaState + alpha * (raw - emaState);   // 0<alpha<=1
  //   return emaState;
  //   Small alpha (0.1) = very smooth but laggy; large alpha (0.6) = snappy.
  // -------------------------------------------------------------------
  return 0;
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  Serial.begin(9600);
  emaState = readRawCm();   // seed the EMA so it doesn't start at 0
}

void loop() {
  // Print as columns for the Serial Plotter: raw, average, median, ema
  Serial.print(readRawCm());     Serial.print('\t');
  Serial.print(movingAverage()); Serial.print('\t');
  Serial.print(medianOfN());     Serial.print('\t');
  Serial.println(exponential(0.3));
  delay(60);
}
