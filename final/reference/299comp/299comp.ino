#include "./functions.h"

typedef struct {
  char I;// 'G'rab    'M'ove
  int A; //  action     R
  int B; //  target     C
} instruction;

#define pl1 28
#define pl2 24
#define pl3 28
instruction path1[pl1] = {{ 'M', 0, 1}, { 'G', 1, 6}, { 'M', 4, 1}, { 'G', 0, 0}, { 'M', 0, 1}, { 'M', 0, 0}, { 'G', 1, 5}, { 'M', 0, 1}, { 'M', 4, 1}, { 'G', 0, 0}, { 'M', 1, 1}, { 'M', 1, 0}, { 'G', 1, 3}, { 'M', 1, 1}, { 'M', 4, 1}, { 'G', 0, 0}, { 'M', 2, 1}, { 'M', 2, 4}, { 'G', 1, 12}, { 'M', 2, 3}, { 'M', 4, 3}, { 'G', 0, 0}, { 'M', 3, 3}, { 'M', 3, 4}, { 'G', 1, 13}, { 'M', 3, 3}, { 'M', 4, 3}, { 'G', 0, 0}};
instruction path2[pl2] = {{ 'M', 0, 2}, { 'G', 1, 7}, { 'M', 4, 2}, { 'G', 0, 0}, { 'M', 4, 0}, { 'G', 1, 0}, { 'M', 4, 2}, { 'G', 0, 0}, { 'M', 4, 4}, { 'G', 1, 14}, { 'M', 4, 2}, { 'G', 0, 0}, { 'M', 0, 2}, { 'M', 0, 0}, { 'G', 1, 4}, { 'M', 0, 2}, { 'M', 4, 2}, { 'G', 0, 0}, { 'M', 0, 2}, { 'M', 0, 4}, { 'G', 1, 10}, { 'M', 0, 2}, { 'M', 4, 2}, { 'G', 0, 0}};
instruction path3[pl3] = {{ 'M', 0, 3}, { 'G', 1, 8}, { 'M', 4, 3}, { 'G', 0, 0}, { 'M', 0, 3}, { 'M', 0, 4}, { 'G', 1, 9}, { 'M', 0, 3}, { 'M', 4, 3}, { 'G', 0, 0}, { 'M', 1, 3}, { 'M', 1, 4}, { 'G', 1, 11}, { 'M', 1, 3}, { 'M', 4, 3}, { 'G', 0, 0}, { 'M', 3, 3}, { 'M', 3, 0}, { 'G', 1, 1}, { 'M', 3, 1}, { 'M', 4, 1}, { 'G', 0, 0}, { 'M', 2, 1}, { 'M', 2, 0}, { 'G', 1, 2}, { 'M', 2, 1}, { 'M', 4, 1}, { 'G', 0, 0}};
int pathnum;

void setup() {  
  Serial.begin(9600);

  // PinModes
  pinMode(bumper, INPUT);
//  pinMode(lbump, INPUT);
//  pinMode(rbump, INPUT);
  pinMode(Lwe, INPUT);
  pinMode(Rwe, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(E1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(E2, OUTPUT);
  Pan.attach(8);
  Tilt.attach(9);
  Grab.attach(10);
 // IRReceiver.attach(11, -1);
}

void loop() {
//  start();
    runPath();
    
}

void waitButton(){ //for testing, remove wrapper for competition
  while(true){
    if(analogRead(bumper) < 550){
    Serial.print("Pushbutton pressed");
    //blink();
    delay(200);
    return;
        }
    
    Serial.println("Waiting for press");  
    }
}

void runPath(){
  instruction *path;
  pathnum = 1;
  int plen;
  if (pathnum == 1){
    path = path1;
    plen = pl1;
  }else if(pathnum == 2){
    path = path2;
    plen = pl2;
  }else{
    path = path3;
    plen = pl3;
  }
  for(int i=0; i < plen; i++){
    if(path[i].I == 'G'){
      int dir;
      if(path[i].A == 0){
        dir = 2;
      }else{
        dir = targets[path[i].B].dir;
      }
      if(currLoc.dir != dir){pivot(dir);}
      interactBall(path[i].A);
    }else if(path[i].I == 'M'){
      seekLoc(path[i].A, path[i].B);
    }else{
      Serial.print("Invalid path command \"");
      Serial.print(path[i].I);
      Serial.println("\"");
    }
  }
  //end run
  drive(-1,0);
  while(1){Serial.println("PATH FINISHED!");}
}
/*
void start(){
  Serial.println("Press Bumper to Start");
  while(digitalRead(lbump) == HIGH && digitalRead(rbump) == HIGH){delay(1);}
  delay(100);
  int data = 0;
  while(data != '0' && data != '1' && data != '2'){
    data = ReceiveIR();
  }
  if (char(data) == '0'){
    pathnum = 1;
    currLoc = {5,1,0};
  }else if (char(data) == '1'){
    pathnum = 2;
    currLoc = {5,2,0};
  }else{
    pathnum = 3;
    currLoc = {5,3,0};
  }
  
  seekLoc(currLoc.R-1, currLoc.C);
}
*/
