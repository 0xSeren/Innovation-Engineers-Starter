/*
 * ======================================================================
 *  EXERCISE 17 -- CAPSTONE: Mars Exploration Course   (Day 5: Navigation)
 * ======================================================================
 *  Mars Rover Camp, Day 2 Session III + IV. Run the full mission and present
 *  it. The field uses tape, cones and pool noodles for boulders / craters /
 *  canyon walls.
 *
 *  MISSION OBJECTIVES (from the camp brief):
 *    1. Leave Base Camp        -> drive out under control
 *    2. Cross Terrain          -> follow a taped path / hold a heading
 *    3. Avoid Obstacles        -> ultrasonic sense + steer around boulders
 *    4. Reach Sample Zone      -> detect the goal marker and stop
 *
 *  This is INTEGRATION, not new theory: assemble the best pieces you built all
 *  week. Each mission phase below points to the exercise it comes from.
 *
 *  ENGINEERING DESIGN PROCESS (the camp's through-line): Imagine -> Build ->
 *  Test -> Improve. Run the course, find your weakest phase, improve ONE thing,
 *  retest, and record the before/after for your showcase.
 *
 *  WHAT YOU DO: drop in your solutions at each TODO and tune the thresholds.
 * ======================================================================
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
#define STOP_DISTANCE 25       // obstacle threshold (Ex7) -- TUNE
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
      // ----------------------------------------------------------------
      // TODO 1: Drive straight out of base camp for a moment, then advance
      //   to CROSS_TERRAIN. (Simplest: setMotors forward, delay, then
      //   phase = CROSS_TERRAIN.)
      // ----------------------------------------------------------------
      phase = CROSS_TERRAIN;
      break;

    case CROSS_TERRAIN: {
      // ----------------------------------------------------------------
      // TODO 2: Cross the terrain. Two valid designs -- pick one:
      //   (a) LINE FOLLOW a taped path using your Ex4 PID follower, OR
      //   (b) hold a heading with your Ex13 gyro and drive forward.
      //   While crossing, watch the ultrasonic; if something is within
      //   STOP_DISTANCE ahead, switch to phase = AVOID.
      // ----------------------------------------------------------------
      if (lookAt(90) < STOP_DISTANCE) phase = AVOID;
      break;
    }

    case AVOID: {
      // ----------------------------------------------------------------
      // TODO 3: Avoid the boulder using your Ex7 routine (stop, look left &
      //   right, turn toward the open side, nudge past it). Then return to
      //   phase = CROSS_TERRAIN to keep heading for the zone.
      // ----------------------------------------------------------------
      phase = CROSS_TERRAIN;
      break;
    }

    case REACH_ZONE:
      // ----------------------------------------------------------------
      // TODO 4 (stretch): Detect the Sample Collection Zone -- e.g. a thick
      //   tape marker that makes all 3 line sensors read the line at once,
      //   or a wall a fixed distance ahead. When detected, stopMotors() and
      //   set phase = DONE. (You can trigger this check from CROSS_TERRAIN.)
      // ----------------------------------------------------------------
      stopMotors();
      phase = DONE;
      break;

    case DONE:
      stopMotors();
      Serial.println(F("Sample zone reached. Mission complete."));
      while (true) {}
  }
}
