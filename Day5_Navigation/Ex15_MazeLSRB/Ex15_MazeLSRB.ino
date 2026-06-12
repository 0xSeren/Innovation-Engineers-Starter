/*
 * ======================================================================
 *  EXERCISE 15 -- Maze Solving: Left-Hand Rule (LSRB)  (Day 5: Navigation)
 * ======================================================================
 *  Solve a MAZE whose WALLS are strips of black electrical tape on a light
 *  floor. The rover drives the open lanes between the tape walls.
 *
 *  THE MAZE (build it with tape):
 *    - A grid of square CELLS, CELL_CM on a side (~30 cm to start).
 *    - A wall = a strip of black tape laid across a cell boundary.
 *      No tape on a boundary = an opening the rover can drive through.
 *    - Start: rover centered in a cell, facing into the maze.
 *    - GOAL: fill the goal cell with a solid tape PAD -- when all three
 *      sensors sit on tape at once, that's the finish (same "all dark = goal"
 *      idea as the line exercises).
 *
 *  WHY IT LOOKS AROUND: the 3 sensors all point straight DOWN at the front of
 *  the rover, so it cannot see a wall to its left or right while driving. So at
 *  every cell it physically LOOKS: it pivots 90 deg (using the closed-loop gyro
 *  turn from Ex13) to face left, ahead, then right, and each time creeps forward
 *  to feel for a tape wall. That fills in gLeft / gStraight / gRight.
 *
 *  ALGORITHM (this is the part you write): at every cell take the LEFT-most
 *  open direction -- priority Left > Straight > Right > Back. Record the turns;
 *  whenever a dead-end U-turn 'B' appears, COLLAPSE the detour into one
 *  equivalent turn. After the run the simplified list is the shortest path.
 *  Works on any maze with no loops.
 *
 *  The looking-around, gyro turns and cell driving are PROVIDED. You implement
 *  the two pieces that ARE the algorithm: the decision rule and the simplifier.
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
// --- geometry / motion (CALIBRATE these on your maze) ---
#define CELL_CM       30.0 // distance between cell centers = your grid pitch
#define WALL_PROBE_CM 12.0 // how far to creep to feel for a wall on a boundary
#define CM_PER_MS     0.020// how far the rover rolls per ms at CREEP (Ex2-style)
#define CREEP   90         // speed while probing for a wall
#define DRIVE   110        // speed while advancing a whole cell
#define TURN_SPEED 100     // wheel speed while pivoting
#define MAX_PATH 60

MPU6050 imu;
float biasDps = 0;

// ---- low-level I/O -------------------------------------------------------
void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}

// "Is this sensor sitting on tape (a wall)?"  (black tape -> dark reading)
bool L(){bool h=analogRead(PIN_L)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool M(){bool h=analogRead(PIN_M)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}
bool R(){bool h=analogRead(PIN_R)>TAPE_THRESHOLD;return TAPE_READS_HIGH?h:!h;}

// ---- gyro turn (provided -- your Ex11+Ex13 work, condensed) --------------
float readRateDps(){return imu.getRotationZ()/131.0 - biasDps;}
void calibrateBias(){long s=0;const int N=500;for(int i=0;i<N;i++){s+=imu.getRotationZ();delay(2);}
  biasDps=(s/(float)N)/131.0;}
// Pivot in place by |deg|. deg<0 = turn LEFT, deg>0 = turn RIGHT. Closed-loop:
// we drive the wheels one way and integrate the gyro until we've turned enough.
void turnDeg(float deg){
  bool right = deg>0; float target=fabs(deg), turned=0; unsigned long last=micros();
  while(turned<target){
    if(right) setMotors(false,TURN_SPEED,true,TURN_SPEED);   // CW: right wheel back
    else      setMotors(true,TURN_SPEED,false,TURN_SPEED);   // CCW: left wheel back
    unsigned long now=micros();
    turned += fabs(readRateDps())*(now-last)/1000000.0;      // we know the direction
    last=now;
  }
  stopMotors();
}

// ---- driving (provided) --------------------------------------------------
void driveCm(float cm,bool fwd){unsigned long ms=(unsigned long)(cm/CM_PER_MS),t0=millis();
  while(millis()-t0<ms)setMotors(fwd,DRIVE,fwd,DRIVE);stopMotors();}

// Creep forward up to WALL_PROBE_CM feeling for a tape wall, then back up the
// same distance so we end where we started. Returns true if a wall was there.
bool wallAhead(){
  unsigned long limit=(unsigned long)(WALL_PROBE_CM/CM_PER_MS),t0=millis(); bool seen=false;
  while(millis()-t0<limit){setMotors(true,CREEP,true,CREEP); if(M()){seen=true;break;}}
  unsigned long fwd=millis()-t0; stopMotors();
  unsigned long t1=millis(); while(millis()-t1<fwd)setMotors(false,CREEP,false,CREEP);
  stopMotors(); return seen;
}

// ---- look around this cell (provided): fill the three exit flags ----------
bool gLeft,gRight,gStraight,gGoal;
void senseCell(){
  gGoal = (L()&&M()&&R());            // sitting on the solid goal pad?
  turnDeg(-90); gLeft     = !wallAhead(); turnDeg(90);   // look left,  face back
  gStraight = !wallAhead();                              // look ahead
  turnDeg(90);  gRight    = !wallAhead(); turnDeg(-90);  // look right, face back
}

// ======================  YOUR CODE  =======================================
char pickLSRB() {
  // -------------------------------------------------------------------
  // TODO 1: Return the LSRB choice as a char, from gLeft/gStraight/gRight:
  //   priority: if (gLeft) return 'L';
  //             else if (gStraight) return 'S';
  //             else if (gRight) return 'R';
  //             else return 'B';     // walls all around = dead end, U-turn
  // -------------------------------------------------------------------
  return 'S';
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

// Pivot to the chosen direction, then drive one cell into the next center.
void executeMove(char t){
  switch(t){case 'L':turnDeg(-90);break;case 'R':turnDeg(90);break;case 'B':turnDeg(180);break;}
  driveCm(CELL_CM,true);
}

bool solved=false;
void setup(){
  pinMode(PIN_PWMA,OUTPUT);pinMode(PIN_PWMB,OUTPUT);pinMode(PIN_AIN1,OUTPUT);
  pinMode(PIN_BIN1,OUTPUT);pinMode(PIN_STBY,OUTPUT);stopMotors();
  Wire.begin(); Serial.begin(9600); imu.initialize();
  Serial.println(F("Place me in the START cell and HOLD STILL -- calibrating gyro..."));
  calibrateBias(); delay(500);
  Serial.println(F("Go!"));
}

void loop(){
  if(solved){stopMotors();return;}
  senseCell();
  if(gGoal){stopMotors();path[pathLen]='\0';
    Serial.print(F("GOAL! path=")); Serial.println(path); solved=true; return;}
  char t=pickLSRB();
  if(pathLen<MAX_PATH) path[pathLen++]=t;
  simplifyPath();
  executeMove(t);
}
