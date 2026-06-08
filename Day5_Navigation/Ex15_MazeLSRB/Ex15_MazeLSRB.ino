/*
 * ======================================================================
 *  EXERCISE 15 -- Maze Solving: Left-Hand Rule (LSRB)  (Day 5: Navigation)
 * ======================================================================
 *  Put Exercises 4 (follow) + 14 (junctions) together into a maze SOLVER.
 *
 *  ALGORITHM: at every junction take the LEFT-most option (priority
 *  Left > Straight > Right > Back). Record the turns; whenever a dead-end
 *  U-turn 'B' appears, COLLAPSE the detour into one equivalent turn. After the
 *  first pass the simplified list is the shortest path. Works on any maze with
 *  no loops.
 *
 *  The line-following, junction detection and turning are PROVIDED (your work
 *  from Ex4 + Ex14, condensed). You implement the two pieces that ARE the
 *  algorithm: the decision rule and the path simplifier.
 *
 *  >> Reference solution: ~/projects/code/MazeSolverLSRB/ <<  (peek only if stuck)
 *
 *  WHAT YOU DO: implement pickLSRB() and simplifyPath().
 * ======================================================================
 */
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
#define BASE 105
#define CREEP 90
#define PIVOT 120
#define CM_PER_MS 0.020
#define NUDGE_CM 4.0
#define MAX_PATH 60

bool L(){bool h=analogRead(PIN_L)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
bool M(){bool h=analogRead(PIN_M)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
bool R(){bool h=analogRead(PIN_R)>LINE_THRESHOLD;return LINE_IS_HIGH?h:!h;}
void setMotors(bool rF,int rS,bool lF,int lS){rS=constrain(rS,0,255);lS=constrain(lS,0,255);
  digitalWrite(PIN_STBY,HIGH);digitalWrite(PIN_AIN1,rF?HIGH:LOW);analogWrite(PIN_PWMA,rS);
  digitalWrite(PIN_BIN1,lF?HIGH:LOW);analogWrite(PIN_PWMB,lS);}
void stopMotors(){analogWrite(PIN_PWMA,0);analogWrite(PIN_PWMB,0);digitalWrite(PIN_STBY,LOW);}
void creepCm(float cm){unsigned long ms=(unsigned long)(cm/CM_PER_MS),t0=millis();
  while(millis()-t0<ms)setMotors(true,CREEP,true,CREEP);stopMotors();}

bool gLeft,gRight,gStraight,gGoal;
void followToJunction(){            // provided (Ex14)
  gLeft=gRight=gStraight=gGoal=false;
  while(true){bool l=L(),m=M(),r=R();
    if(l&&r){stopMotors();gLeft=gRight=true;break;}
    if(!m&&!l&&!r){stopMotors();break;}
    if(!m&&(l||r)){stopMotors();gLeft=l;gRight=r;break;}
    if(m&&!l&&!r){setMotors(true,BASE,true,BASE);continue;}
    if(m&&l){gLeft=true;break;} else if(m&&r){gRight=true;break;}}
  creepCm(NUDGE_CM);
  bool l=L(),m=M(),r=R(); if(l&&m&&r)gGoal=true; gStraight=m;
}
void reacquireSpin(bool left){unsigned long t0=millis();
  while(millis()-t0<180)setMotors(!left,PIVOT,left,PIVOT);
  while(!M())setMotors(!left,PIVOT,left,PIVOT); stopMotors();}
void doTurn(char t){switch(t){case 'L':reacquireSpin(true);break;case 'R':reacquireSpin(false);break;
  case 'B':reacquireSpin(true);break;default:break;}}

char path[MAX_PATH]; int pathLen=0;

char pickLSRB() {
  // -------------------------------------------------------------------
  // TODO 1: Return the LSRB choice as a char.
  //   priority: if (gLeft) return 'L';
  //             else if (gStraight) return 'S';
  //             else if (gRight) return 'R';
  //             else return 'B';     // nothing ahead = dead end, U-turn
  // -------------------------------------------------------------------
  return 'S';
}

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

bool solved=false;
void setup(){pinMode(PIN_PWMA,OUTPUT);pinMode(PIN_PWMB,OUTPUT);pinMode(PIN_AIN1,OUTPUT);
  pinMode(PIN_BIN1,OUTPUT);pinMode(PIN_STBY,OUTPUT);stopMotors();Serial.begin(9600);delay(1500);}

void loop(){
  if(solved){stopMotors();return;}
  followToJunction();
  if(gGoal){stopMotors();path[pathLen]='\0';
    Serial.print(F("GOAL! path=")); Serial.println(path); solved=true; return;}
  char t=pickLSRB();
  if(pathLen<MAX_PATH) path[pathLen++]=t;
  simplifyPath();
  doTurn(t);
}
