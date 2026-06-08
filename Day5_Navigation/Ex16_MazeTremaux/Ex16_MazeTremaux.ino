/*
 * ======================================================================
 *  EXERCISE 16 -- Maze Solving: Tremaux's Algorithm   (Day 5: Navigation)
 * ======================================================================
 *  The left-hand rule fails on mazes with LOOPS -- it can circle forever.
 *  Tremaux's algorithm solves ANY maze by MARKING passages: never take a
 *  passage that already has two marks, and use marks to back out of loops.
 *
 *  To remember marks per junction the rover must know WHICH junction it is at,
 *  so this one needs the gyro: it dead-reckons (heading + distance), snaps to a
 *  grid, and labels each junction with integer (x,y) coords. Everything except
 *  the decision rule is PROVIDED (and uses your Day-4 gyro work).
 *
 *  TREMAUX RULES at a junction, after marking the passage you arrived by:
 *    - brand-new junction          -> take any UNMARKED passage (else go back)
 *    - known junction, reached via  -> go BACK the way you came (a loop closed)
 *      a freshly-marked passage
 *    - otherwise (backtracking)     -> prefer UNMARKED, else the 1-mark passage
 *    - NEVER enter a passage with 2 marks
 *
 *  >> Reference solution: ~/projects/code/MazeSolverTremaux/ <<  (peek if stuck)
 *
 *  WHAT YOU DO: implement the decision block (TODO) inside loop().
 * ======================================================================
 */
#include <Arduino.h>
#include <Wire.h>
#include "MPU6050_getdata.h"

#define PIN_L A2
#define PIN_M A1
#define PIN_R A0
#define PIN_PWMA 5
#define PIN_PWMB 6
#define PIN_AIN1 7
#define PIN_BIN1 8
#define PIN_STBY 3
#define LINE_THRESHOLD 500
#define LINE_IS_HIGH 1
#define GYRO_SIGN (+1)
#define BASE 105
#define CREEP 90
#define PIVOT 120
#define CM_PER_MS 0.020
#define NUDGE_CM 4.0
#define CELL_CM 25.0
#define MAX_NODES 30

bool L(){bool h=analogRead(PIN_L)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
bool M(){bool h=analogRead(PIN_M)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
bool R(){bool h=analogRead(PIN_R)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}

float gX=0,gY=0,gHeadingRad=0;
void updateHeading(){float y=0;MPU6050Getdata.MPU6050_dveGetEulerAngles(&y);gHeadingRad=(float)GYRO_SIGN*y*(PI/180.0);}
int compassDir(){float d=gHeadingRad*180.0/PI;int q=((int)roundf(d/90.0))%4;return (q+4)%4;}
void creepCm(float cm){unsigned long ms=(unsigned long)(cm/CM_PER_MS),t0=millis(),tl=t0;
  while(millis()-t0<ms){updateHeading();setMotors(true,CREEP,true,CREEP);
    unsigned long n=millis();float dd=CM_PER_MS*(n-tl);tl=n;gX+=dd*sin(gHeadingRad);gY+=dd*cos(gHeadingRad);}stopMotors();}

bool gLeft,gRight,gStraight,gGoal;
void followToJunction(){gLeft=gRight=gStraight=gGoal=false;unsigned long tl=millis();
  while(true){updateHeading();bool l=L(),m=M(),r=R();
    unsigned long n=millis();float dd=CM_PER_MS*(float)BASE/(float)CREEP*(n-tl);tl=n;
    if(m){gX+=dd*sin(gHeadingRad);gY+=dd*cos(gHeadingRad);}
    if(l&&r){stopMotors();gLeft=gRight=true;break;}
    if(!m&&!l&&!r){stopMotors();break;}
    if(!m&&(l||r)){stopMotors();gLeft=l;gRight=r;break;}
    if(m&&!l&&!r){setMotors(true,BASE,true,BASE);continue;}
    if(m&&l){gLeft=true;break;}else if(m&&r){gRight=true;break;}}
  creepCm(NUDGE_CM);bool l=L(),m=M(),r=R();if(l&&m&&r)gGoal=true;gStraight=m;}
void reacquireSpin(bool left){unsigned long t0=millis();
  while(millis()-t0<180){updateHeading();setMotors(!left,PIVOT,left,PIVOT);}
  while(!M()){updateHeading();setMotors(!left,PIVOT,left,PIVOT);}stopMotors();}
void executeRel(int rel){switch(rel){case 1:reacquireSpin(false);break;case 3:reacquireSpin(true);break;
  case 2:reacquireSpin(true);break;default:break;}}

struct Node{int8_t x,y;uint8_t marks[4];};
Node nodes[MAX_NODES]; int nodeCount=0;
int findNode(int8_t x,int8_t y){for(int i=0;i<nodeCount;i++)if(nodes[i].x==x&&nodes[i].y==y)return i;return -1;}
int getNode(int8_t x,int8_t y){int i=findNode(x,y);if(i>=0)return i;if(nodeCount>=MAX_NODES)return nodeCount-1;
  nodes[nodeCount]={x,y,{0,0,0,0}};return nodeCount++;}
int pickExit(bool ex[4],uint8_t mk[4],int want,int cur){int order[4]={cur,(cur+3)%4,(cur+1)%4,(cur+2)%4};
  for(int k=0;k<4;k++){int d=order[k];if(ex[d]&&mk[d]==want)return d;}return -1;}

void setup(){pinMode(PIN_PWMA,OUTPUT);pinMode(PIN_PWMB,OUTPUT);pinMode(PIN_AIN1,OUTPUT);
  pinMode(PIN_BIN1,OUTPUT);pinMode(PIN_STBY,OUTPUT);stopMotors();Wire.begin();Serial.begin(9600);
  MPU6050Getdata.MPU6050_dveInit();MPU6050Getdata.MPU6050_calibration();updateHeading();
  Serial.println(F("Tremaux -- hold still during calibration, then it runs."));delay(500);}

void loop(){
  static bool done=false; if(done){stopMotors();return;}
  followToJunction();
  if(gGoal){stopMotors();Serial.println(F("Goal!"));done=true;return;}

  int curDir=compassDir(), came=(curDir+2)%4;
  int8_t nx=(int8_t)roundf(gX/CELL_CM), ny=(int8_t)roundf(gY/CELL_CM);
  bool isNew=(findNode(nx,ny)<0);
  Node &n=nodes[getNode(nx,ny)];
  bool exists[4]={false,false,false,false}; exists[came]=true;
  if(gStraight)exists[curDir]=true;
  if(gLeft)exists[(curDir+3)%4]=true;
  if(gRight)exists[(curDir+1)%4]=true;

  bool cameWasUnmarked=(n.marks[came]==0);
  if(n.marks[came]<2) n.marks[came]++;       // mark passage we arrived by

  int chosen;
  // -------------------------------------------------------------------
  // TODO: Apply Tremaux's rules to set `chosen` (an absolute direction 0..3).
  //   if (isNew) {
  //       chosen = pickExit(exists, n.marks, 0, curDir);   // any unmarked
  //       if (chosen < 0) chosen = came;                   // dead end -> back
  //   } else if (cameWasUnmarked) {
  //       chosen = came;                                   // loop closed -> back
  //   } else {
  //       chosen = pickExit(exists, n.marks, 0, curDir);   // prefer unmarked
  //       if (chosen < 0) chosen = pickExit(exists, n.marks, 1, curDir);  // else 1-mark
  //       if (chosen < 0) chosen = came;
  //   }
  // -------------------------------------------------------------------
  chosen = came;   // placeholder (just backtracks) -- replace via the TODO

  if(n.marks[chosen]<2) n.marks[chosen]++;   // mark passage we leave by
  executeRel((chosen-curDir+4)%4);
  gX=nx*CELL_CM; gY=ny*CELL_CM;              // re-center odometry on this node
}
