/*
 *  EXERCISE 15 -- Maze Solving: Left-Hand Rule (LSRB)  (SOLUTION)
 *  Full standalone reference: ~/projects/code/MazeSolverLSRB/
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
void followToJunction(){
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

char pickLSRB() {                       // SOLUTION to TODO 1
  if (gLeft)     return 'L';
  if (gStraight) return 'S';
  if (gRight)    return 'R';
  return 'B';
}

void simplifyPath() {                   // SOLUTION to TODO 2
  if (pathLen < 3 || path[pathLen - 2] != 'B') return;
  int angle = 0;
  for (int i = pathLen - 3; i < pathLen; i++) {
    switch (path[i]) {
      case 'R': angle += 90;  break;
      case 'L': angle += 270; break;
      case 'B': angle += 180; break;
    }
  }
  angle %= 360;
  char nt = 'S';
  if (angle == 90) nt = 'R'; else if (angle == 180) nt = 'B'; else if (angle == 270) nt = 'L';
  pathLen -= 3;
  path[pathLen++] = nt;
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
