/*
 *  EXERCISE 17 -- CAPSTONE: Mars Exploration Course  (SOLUTION)
 *  One worked integration. Many valid designs exist; this one line-follows the
 *  terrain, avoids boulders with the ultrasonic, and stops on a thick goal
 *  marker (all three line sensors dark at once).
 */
#include <Servo.h>
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define PIN_SERVO 10
#define PIN_TRIG 13
#define PIN_ECHO 12
#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define CRUISE 120
#define TURN_SPEED 130
#define STOP_DISTANCE 25
#define LINE_THRESHOLD 500
Servo head;
enum Phase { LEAVE_BASE, CROSS_TERRAIN, AVOID, REACH_ZONE, DONE };
Phase phase = LEAVE_BASE;

void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}
float lookAt(int a){head.write(a);delay(150);
  digitalWrite(PIN_TRIG,LOW);delayMicroseconds(2);digitalWrite(PIN_TRIG,HIGH);delayMicroseconds(10);digitalWrite(PIN_TRIG,LOW);
  unsigned long us=pulseIn(PIN_ECHO,HIGH,25000UL);return us==0?400.0:us/58.0;}
bool onLine(int pin){return analogRead(pin)>LINE_THRESHOLD;}

void setup(){
  pinMode(PIN_PWMA,OUTPUT);pinMode(PIN_PWMB,OUTPUT);pinMode(PIN_AIN1,OUTPUT);
  pinMode(PIN_BIN1,OUTPUT);pinMode(PIN_STBY,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);pinMode(PIN_ECHO,INPUT);
  head.attach(PIN_SERVO);head.write(90);
  Serial.begin(9600);
  delay(1000);
  Serial.println(F("Mission start."));
}

void loop(){
  switch(phase){

    case LEAVE_BASE:
      setMotors(true, CRUISE, true, CRUISE);
      delay(600);
      stopMotors();
      phase = CROSS_TERRAIN;
      break;

    case CROSS_TERRAIN: {
      // goal marker = all three sensors dark
      if (onLine(PIN_L) && onLine(PIN_M) && onLine(PIN_R)) { phase = REACH_ZONE; break; }
      // boulder ahead?
      if (lookAt(90) < STOP_DISTANCE) { phase = AVOID; break; }
      // otherwise bang-bang line follow
      bool L = onLine(PIN_L), M = onLine(PIN_M), R = onLine(PIN_R);
      if (M && !L && !R)      setMotors(true, CRUISE, true, CRUISE);
      else if (L && !R)       setMotors(true, CRUISE, true, 40);
      else if (R && !L)       setMotors(true, 40, true, CRUISE);
      else                    setMotors(true, CRUISE, true, CRUISE);  // coast if unsure
      break;
    }

    case AVOID: {
      stopMotors();
      float left = lookAt(150), right = lookAt(30);
      head.write(90);
      if (left > right) setMotors(true,  TURN_SPEED, false, TURN_SPEED);  // spin left
      else              setMotors(false, TURN_SPEED, true,  TURN_SPEED);  // spin right
      delay(400);
      setMotors(true, CRUISE, true, CRUISE); delay(400);                  // clear the boulder
      stopMotors();
      phase = CROSS_TERRAIN;
      break;
    }

    case REACH_ZONE:
      stopMotors();
      phase = DONE;
      break;

    case DONE:
      stopMotors();
      Serial.println(F("Sample zone reached. Mission complete."));
      while (true) {}
  }
}
