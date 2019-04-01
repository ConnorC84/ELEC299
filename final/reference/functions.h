#include "Arduino.h"
#include <Servo.h>
#include <QSerial.h>
#include <math.h>

//      0
//3    5X5     1
//      2

// Functions
void drive(int dir, float speedfactor); //dir=1 F, dir=0 B, dir=-1 S
void adjustS(float lsfactor, float rsfactor);
void pivot(int dir);
void interactBall(int act);//1 = Grab, 0 = Drop
void seekLoc(int R, int C);

// Variables
typedef struct {
  int R;
  int C;
  int dir;
} loc;

loc currLoc = {0, 0, 0};
loc targets[15] = { // 0-14 balls to grab
{ 4, 0, 3},{ 3, 0, 3},{ 2, 0, 3},{ 1, 0, 3},{ 0, 0, 3},
{ 0, 0, 0},{ 0, 1, 0},{ 0, 2, 0},{ 0, 3, 0},{ 0, 4, 0},
{ 4, 4, 1},{ 3, 4, 1},{ 2, 4, 1},{ 1, 4, 1},{ 0, 4, 1}
};

int right_speed = 115;
int left_speed = 105;

int lbump = 3; //10k pull up
int rbump = 2;

int Lwe = 12;
int Rwe = 13;

int E1 = 6;//right motor
int M1 = 7;
int E2 = 5;//left motor
int M2 = 4;

Servo Pan, Tilt, Grab;//G:0(OPEN)-180(CLOSED), T:~100(GRAB)-160(CARRY), P:90
QSerial IRReceiver;

int IRRange = A0;
int LineL = A1;
int LineC = A2;
int LineR = A3;
int GripFSR = A4;
int LineThresh = 850;

// Functions

int ReceiveIR(){ ///////////////////////////////////////////
  int rec = IRReceiver.receive(200);
  while (rec == 0 || rec == -1 || rec == -2){
    rec = IRReceiver.receive(200);
  }
  Serial.println(rec);
  return rec;
}

void drive(int dir, float speedfactor){ //dir=1 F, dir=0 B, dir=-1 S  ///////////////////////////////////////////
  if (dir == -1){
    analogWrite(E1, 0);
    analogWrite(E2, 0);
  }else{
    digitalWrite(M1, dir);
    digitalWrite(M2, dir);
    analogWrite(E1, right_speed*speedfactor);
    analogWrite(E2, left_speed*speedfactor);
  }
}

void adjustS(float lsfactor, float rsfactor){ ///////////////////////////////////////////
  analogWrite(E1, right_speed*rsfactor);
  analogWrite(E2, left_speed*lsfactor);
}

void pivot(int dir){ ///////////////////////////////////////////
  int ldir, rdir;
  int angle = 60;
  if(dir == currLoc.dir){ return; }
  if ((currLoc.dir-1 == dir) || (currLoc.dir == 0 && dir == 3)){ 
    ldir = 0;
    rdir = 1;
  }else{
    ldir = 1;
    rdir = 0;
    if(abs(dir - currLoc.dir) == 2){
      angle = 160;
    }
  }
  adjustS(1, 1);
  digitalWrite(M1, rdir);
  digitalWrite(M2, ldir);
  int count = 0;
  while(1){
    if (digitalRead(Rwe) == HIGH){
      count++;
      delay(10);
    }
    if (count >= angle*0.8){
      if (analogRead(LineC) > LineThresh){
        break;
      }
    }
  }
  drive(-1,0);
  currLoc.dir = dir;
}

void interactBall(int act){//1 = Grab, 0 = Drop ///////////////////////////////////////////
  Tilt.write(110);
  Pan.write(94);
  drive(1, 1.1);
  Serial.println("driving");
  while(1){
    if(analogRead(LineR)>LineThresh){
      adjustS(1,0);
    }else if(analogRead(LineL)>LineThresh){
      adjustS(0,1);
    }else{
      adjustS(1.1,1.1);
    }
    if(digitalRead(lbump) == LOW || digitalRead(rbump) == LOW){
      delay(200);
      if(digitalRead(lbump) == LOW && digitalRead(rbump) == LOW){
        break;
      }
    }
  }
  delay(200);
  
  if(act == 1){
    drive(0,1);
    int counter = 0;
    int prev = digitalRead(Rwe);
    while(counter < 2){ 
      if (prev != digitalRead(Rwe)){
        prev = digitalRead(Rwe);
        counter++;
      }
    }
    drive(-1,0);
    delay(10);
    Grab.write(75);
    delay(100);
    Tilt.write(80);
    delay(200);
    Tilt.write(45);
    delay(100);
    
    for(int i=75; i < 180; i++){
      Grab.write(i);
      if (analogRead(GripFSR) >= 450){ break; }
      delay(20);
    }
  }else{
    Tilt.write(60);
    delay(150);
    Grab.write(75);
    delay(500);
  }
  Tilt.write(110);
  delay(150);
  
  adjustS(1, 1);
  digitalWrite(M1, 1);
  digitalWrite(M2, 0);
  int count = 0;
  while(1){
    if (count >= 160*0.8){
      if (analogRead(LineC) > LineThresh){
        break;
      }
    }
    if (digitalRead(Rwe) == HIGH){
      count++;
      delay(10);
    }
  }
  drive(-1,0);

  if (currLoc.dir == 2){
   currLoc.R += 1;
   currLoc.dir = 0;
   seekLoc(currLoc.R-1,currLoc.C);
  }else if (currLoc.dir == 0){
   currLoc.R -= 1;
   currLoc.dir = 2;
   seekLoc(currLoc.R+1,currLoc.C);
  }else if (currLoc.dir == 3){
    currLoc.C -= 1;
    currLoc.dir = 1;
    seekLoc(currLoc.R,currLoc.C+1);
  }else if (currLoc.dir == 1){
   currLoc.C += 1;
   currLoc.dir = 3;
   seekLoc(currLoc.R,currLoc.C-1);
  } 
}

void seekLoc(int R, int C){ //////////////////////////////////////////////////////////////////////////////////////
  int dir, dist;
  if (R == currLoc.R){
    if (C < currLoc.C){
      dir = 3;
      dist = currLoc.C - C;
    }else{
      dir = 1;
      dist = C - currLoc.C;
    }
  }else if (C == currLoc.C){
    if (R < currLoc.R){
      dir = 0;
      dist = currLoc.R - R;
    }else{
      dir = 2;
      dist = R - currLoc.R;
    }
  }else{
    Serial.println("Loc too complex for seekLoc");
    return;
  }
  if (currLoc.dir != dir){
    pivot(dir);
  }
  //drive forward
  int x = 0;
  float lsf = 1;
  float rsf = 0;
  int LL, LR, LC, LRi, LLi, range;
  drive(1, 1.1);
  while( x < dist){
    LL = analogRead(LineL);
    LR = analogRead(LineR);
    LC = analogRead(LineC);
    LRi = analogRead(LineR);
    LLi = analogRead(LineL);
    if (dir == 1){
      LRi = LLi;
    }else if(dir == 3){
      LLi = LRi;
    }
    if ((LLi>LineThresh)&&(LRi>LineThresh)&&(LC>LineThresh)){ //intersection
      x++;
      if (dir == 2){ currLoc.R += 1;}
      else if (dir == 0){ currLoc.R -= 1;}
      else if (dir == 3){ currLoc.C -= 1;}
      else if (dir == 1){ currLoc.C += 1;} 
      adjustS(1,1);
      int counter = 0;
      int prev = digitalRead(Rwe);
      while(counter < 7){ 
        if (prev != digitalRead(Rwe)){
          prev = digitalRead(Rwe);
          counter++;
        }
      }
    }else if (LL>LineThresh){
      lsf = 0;
      rsf = 1;
    }else if (LR>LineThresh){
      lsf = 1;
      rsf = 0;
    }else if (LC>LineThresh){
      lsf = 1.1;
      rsf = 1.1;
    }
    
    range = analogRead(IRRange);
    if (range >= 500){
      adjustS(lsf*0.6, rsf*0.6);
    }else{
      adjustS(lsf,rsf);
    }
  }
  drive(-1, 0);
  delay(100);
}



