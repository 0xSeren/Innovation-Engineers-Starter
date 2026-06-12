/*
 * ======================================================================
 *  EXERCISE 15 -- Maze Solving: Left-Hand Rule (LSRB)   *** SOLUTION ***
 * ======================================================================
 *  Tape-WALL maze: walls are tape strips ACROSS the corridors on a light
 *  floor. The rover drives STRAIGHT until all three sensors hit tape (a wall
 *  ahead), then pivots (gyro turn) to glance left/right and turns. It only
 *  stops when straight is blocked, so the choice is Left > Right > Back.
 *  Goal = a solid tape pad.
 *
 *  Both TODOs are filled in below: pickLSRB() and simplifyPath(). The full
 *  two-pass reference (explore + replay, button start) is in
 *  ~/projects/code/MazeSolverLSRB/.
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

#define TAPE_THRESHOLD 500
#define TAPE_READS_HIGH 1
#define CM_PER_MS  0.020
#define PROBE_CM   12.0
#define NUDGE_CM   3.0
#define DRIVE   110
#define CREEP   90
#define TURN_SPEED 100
#define MAX_PATH 60

MPU6050 imu;
float biasDps = 0;

void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}

bool L(){bool h=analogRead(PIN_L)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool M(){bool h=analogRead(PIN_M)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool R(){bool h=analogRead(PIN_R)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool wall(){ return L() && M() && R(); }

float readRateDps(){return imu.getRotationZ()/131.0 - biasDps;}
void calibrateBias(){long s=0;const int N=500;for(int i=0;i<N;i++){s+=imu.getRotationZ();delay(2);}
  biasDps=(s/(float)N)/131.0;}
void turnDeg(float deg){
  bool right = deg>0; float target=fabs(deg), turned=0; unsigned long last=micros();
  while(turned<target){
    if(right) setMotors(false,TURN_SPEED,true,TURN_SPEED);
    else      setMotors(true,TURN_SPEED,false,TURN_SPEED);
    unsigned long now=micros();
    turned += fabs(readRateDps())*(now-last)/1000000.0;
    last=now;
  }
  stopMotors();
}

void driveToWall(){ while(!wall()) setMotors(true,DRIVE,true,DRIVE); stopMotors(); }

bool creepClear(float cm){
  unsigned long limit=(unsigned long)(cm/CM_PER_MS),t0=millis(); bool blocked=false;
  while(millis()-t0<limit){ setMotors(true,CREEP,true,CREEP); if(wall()){blocked=true;break;} }
  unsigned long fwd=millis()-t0; stopMotors();
  unsigned long t1=millis(); while(millis()-t1<fwd) setMotors(false,CREEP,false,CREEP);
  stopMotors(); return !blocked;
}

bool gLeft,gRight,gStraight,gGoal;
void senseWall(){
  gStraight=false;
  gGoal = !creepClear(NUDGE_CM);
  if(gGoal) return;
  turnDeg(-90); gLeft  = creepClear(PROBE_CM); turnDeg(90);
  turnDeg(90);  gRight = creepClear(PROBE_CM); turnDeg(-90);
}

// ======================  SOLUTION  ========================================
char pickLSRB() {
  if (gLeft)     return 'L';        // left-hand rule: leftmost open wins
  if (gStraight) return 'S';        // never true here (we stop only when straight is walled)
  if (gRight)    return 'R';
  return 'B';                       // walls all around = dead end, U-turn
}

char path[MAX_PATH]; int pathLen=0;

void simplifyPath() {
  if (pathLen < 3 || path[pathLen - 2] != 'B') return;   // only collapse "X B Y"
  int angle = 0;
  for (int i = pathLen - 3; i < pathLen; i++) {
    switch (path[i]) {
      case 'R': angle += 90;  break;
      case 'L': angle += 270; break;
      case 'B': angle += 180; break;
    }                                                     // 'S' adds 0
  }
  angle %= 360;
  char nt = 'S';
  if (angle == 90) nt = 'R'; else if (angle == 180) nt = 'B'; else if (angle == 270) nt = 'L';
  pathLen -= 3;
  path[pathLen++] = nt;
}
// ==========================================================================

void executeTurn(char t){
  switch(t){case 'L':turnDeg(-90);break;case 'R':turnDeg(90);break;case 'B':turnDeg(180);break;}
}

bool solved=false;
void setup(){
  pinMode(PIN_PWMA,OUTPUT);pinMode(PIN_PWMB,OUTPUT);pinMode(PIN_AIN1,OUTPUT);
  pinMode(PIN_BIN1,OUTPUT);pinMode(PIN_STBY,OUTPUT);stopMotors();
  Wire.begin(); Serial.begin(9600); imu.initialize();
  Serial.println(F("Aim me down the first corridor and HOLD STILL -- calibrating gyro..."));
  calibrateBias(); delay(500);
  Serial.println(F("Go!"));
}

void loop(){
  if(solved){stopMotors();return;}
  driveToWall();
  senseWall();
  if(gGoal){stopMotors();path[pathLen]='\0';
    Serial.print(F("GOAL! path=")); Serial.println(path); solved=true; return;}
  char t=pickLSRB();
  if(pathLen<MAX_PATH) path[pathLen++]=t;
  simplifyPath();
  executeTurn(t);
}
