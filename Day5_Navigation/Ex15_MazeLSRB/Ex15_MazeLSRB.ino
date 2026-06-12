/*
 * ======================================================================
 *  EXERCISE 15 -- Maze Solving: Left-Hand Rule (LSRB)  (Day 5: Navigation)
 * ======================================================================
 *  Solve a MAZE whose WALLS are strips of black electrical tape laid ACROSS
 *  the corridors on a light floor. The rover does NOT follow a line. It just:
 *
 *      1. drives STRAIGHT until all three sensors hit tape  = a WALL AHEAD,
 *      2. then DECIDES which way to go.
 *
 *  Because the three sensors all look straight DOWN at the front, the rover
 *  can't see a side opening while driving -- so to decide, it pivots (a
 *  closed-loop gyro turn from Ex13) to glance LEFT, then RIGHT, and creeps a
 *  little to feel whether that way is open or walled.
 *
 *  Note one thing: the rover only ever stops because a wall blocked it going
 *  straight -- so "straight" is never an option at a stop. The choice is just
 *  Left > Right > Back. (Open straights it simply drives through.)
 *
 *  ALGORITHM (the part you write): take the LEFT-most open direction. Record
 *  the turns; whenever a dead-end U-turn 'B' appears, COLLAPSE the detour into
 *  one equivalent turn. The simplified list is the shortest path. No loops.
 *
 *  GOAL: fill the finish cell with a solid tape PAD -- when the rover drives
 *  onto it the tape stays under all three sensors (not a thin wall line).
 *
 *  The driving, gyro turns and look-around are PROVIDED. You implement the two
 *  pieces that ARE the algorithm: the decision rule and the path simplifier.
 *
 *  >> Reference solution: ~/projects/code/MazeSolverLSRB/ <<  (peek only if stuck)
 *
 *  WHAT YOU DO: implement pickLSRB() and simplifyPath().
 * ======================================================================
 */
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define PIN_L A2            // left  reflectance sensor (points down)
#define PIN_M A1            // middle
#define PIN_R A0            // right
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3

// --- tape sensing (black tape on a light floor) ---
#define TAPE_THRESHOLD 500 // analog cutoff; calibrate with Ex8
#define TAPE_READS_HIGH 1  // 1: a sensor over tape reads HIGH. Flip to 0 if your
                           //    floor/tape contrast is the other way around.
// --- motion (CALIBRATE CM_PER_MS on your rover) ---
#define CM_PER_MS  0.020   // how far the rover rolls per ms at CREEP (Ex2-style)
#define PROBE_CM   12.0    // how far to glance down a side passage to call it open
#define NUDGE_CM   3.0     // creep used to tell a solid GOAL pad from a thin wall
#define DRIVE   110        // straight-ahead cruising speed
#define CREEP   90         // speed while probing/glancing
#define TURN_SPEED 100     // wheel speed while pivoting
#define MAX_PATH 60

MPU6050 imu;
float biasDps = 0;

// ---- low-level I/O -------------------------------------------------------
void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}

// "Is this sensor sitting on tape?"  (black tape -> dark reading)
bool L(){bool h=analogRead(PIN_L)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool M(){bool h=analogRead(PIN_M)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool R(){bool h=analogRead(PIN_R)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool wall(){ return L() && M() && R(); }       // all three on tape = a wall here

// ---- gyro turn (provided -- your Ex11+Ex13 work, condensed) --------------
float readRateDps(){return imu.getRotationZ()/131.0 - biasDps;}
void calibrateBias(){long s=0;const int N=500;for(int i=0;i<N;i++){s+=imu.getRotationZ();delay(2);}
  biasDps=(s/(float)N)/131.0;}
// Pivot in place by |deg|. deg<0 = LEFT, deg>0 = RIGHT (closed-loop on the gyro).
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

// ---- driving (provided) --------------------------------------------------
// Drive straight ahead until we run into a wall. This is the whole point:
// the rover stops when the SENSORS see a wall, not after some set distance.
void driveToWall(){
  while(!wall()) setMotors(true,DRIVE,true,DRIVE);
  stopMotors();
}

// Creep forward up to `cm`, stopping early if we hit a wall, then reverse back
// to where we started. Returns true if it stayed CLEAR the whole way (= open).
bool creepClear(float cm){
  unsigned long limit=(unsigned long)(cm/CM_PER_MS),t0=millis(); bool blocked=false;
  while(millis()-t0<limit){ setMotors(true,CREEP,true,CREEP); if(wall()){blocked=true;break;} }
  unsigned long fwd=millis()-t0; stopMotors();
  unsigned long t1=millis(); while(millis()-t1<fwd) setMotors(false,CREEP,false,CREEP);
  stopMotors(); return !blocked;
}

// ---- look around this wall (provided): fill the exit flags ----------------
bool gLeft,gRight,gStraight,gGoal;
void senseWall(){
  gStraight=false;                          // we only stop when straight is blocked
  gGoal = !creepClear(NUDGE_CM);            // tape still there after a nudge = GOAL pad
  if(gGoal) return;
  turnDeg(-90); gLeft  = creepClear(PROBE_CM); turnDeg(90);   // glance left
  turnDeg(90);  gRight = creepClear(PROBE_CM); turnDeg(-90);  // glance right
}

// ======================  YOUR CODE  =======================================
char pickLSRB() {
  // -------------------------------------------------------------------
  // TODO 1: Return the LSRB choice as a char, from gLeft/gStraight/gRight:
  //   priority: if (gLeft) return 'L';
  //             else if (gStraight) return 'S';   // (always false here -- see note)
  //             else if (gRight) return 'R';
  //             else return 'B';     // walls all around = dead end, U-turn
  // -------------------------------------------------------------------
  return 'B';
}

char path[MAX_PATH]; int pathLen=0;

void simplifyPath() {
  // -------------------------------------------------------------------
  // TODO 2: If the 2nd-to-last recorded turn is 'B', collapse the last THREE
  //   turns into one, by total rotation:
  //     sum: R=+90, L=+270, B=+180, S=+0 ; then  sum %= 360
  //     0->'S', 90->'R', 180->'B', 270->'L'
  //   Replace path[len-3..len-1] with that single char (pathLen -= 2).
  //   Only run when pathLen >= 3 AND path[pathLen-2] == 'B'.
  //   (This is the canonical dead-end-elimination rule.)
  // -------------------------------------------------------------------
}
// ==========================================================================

// Pivot to face the chosen direction; the next driveToWall() drives down it.
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
  driveToWall();                       // straight until a wall stops us
  senseWall();                         // glance left/right, check for the goal pad
  if(gGoal){stopMotors();path[pathLen]='\0';
    Serial.print(F("GOAL! path=")); Serial.println(path); solved=true; return;}
  char t=pickLSRB();
  if(pathLen<MAX_PATH) path[pathLen++]=t;
  simplifyPath();
  executeTurn(t);
}
