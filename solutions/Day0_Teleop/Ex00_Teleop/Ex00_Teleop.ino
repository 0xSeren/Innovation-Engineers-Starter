/*
 *  EXERCISE 0 -- TELEOP (IR remote)  (SOLUTION)
 *  Drive with the kit's IR remote; hold a button to move, release to stop.
 *  Accepts both shipped remote code sets. See the starter for the full writeup.
 */
#include <IRremote.h>          // ELEGOO's bundled IRremote 2.x

#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define PIN_IR   9

#define A_UP   16736925UL
#define A_DOWN 16754775UL
#define A_LEFT 16720605UL
#define A_RIGHT 16761405UL
#define A_OK   16712445UL
#define B_UP   5316027UL
#define B_DOWN 2747854299UL
#define B_LEFT 1386468383UL
#define B_RIGHT 553536955UL
#define B_OK   3622325019UL
#define IR_REPEAT 0xFFFFFFFFUL

IRrecv irrecv(PIN_IR);
decode_results results;
int speed = 150;
unsigned long lastButton = 0, lastSeen = 0;

void setMotors(bool rFwd, int rSpd, bool lFwd, int lSpd) {
  rSpd = constrain(rSpd, 0, 255); lSpd = constrain(lSpd, 0, 255);
  digitalWrite(PIN_STBY, HIGH);
  digitalWrite(PIN_AIN1, rFwd ? HIGH : LOW); analogWrite(PIN_PWMA, rSpd);
  digitalWrite(PIN_BIN1, lFwd ? HIGH : LOW); analogWrite(PIN_PWMB, lSpd);
}
void stopMotors() { analogWrite(PIN_PWMA, 0); analogWrite(PIN_PWMB, 0); digitalWrite(PIN_STBY, LOW); }
bool isBtn(unsigned long code, unsigned long a, unsigned long b) { return code == a || code == b; }

void handle(unsigned long code) {
  if      (isBtn(code, A_UP,    B_UP))    setMotors(true,  speed, true,  speed);  // forward
  else if (isBtn(code, A_DOWN,  B_DOWN))  setMotors(false, speed, false, speed);  // back
  else if (isBtn(code, A_LEFT,  B_LEFT))  setMotors(true,  speed, false, speed);  // spin left
  else if (isBtn(code, A_RIGHT, B_RIGHT)) setMotors(false, speed, true,  speed);  // spin right
  else if (isBtn(code, A_OK,    B_OK))    stopMotors();
}

void setup() {
  pinMode(PIN_PWMA, OUTPUT); pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT); pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  stopMotors();
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println(F("TELEOP ready -- press the remote arrows, OK = stop."));
}

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long code = results.value;
    irrecv.resume();
    if (code != IR_REPEAT) lastButton = code;
    lastSeen = millis();
    handle(lastButton);
  }
  if (millis() - lastSeen > 160) stopMotors();
}
