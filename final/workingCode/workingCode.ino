//----------Gripper Servo pins------
#include <Servo.h>
Servo servoArm, servoGrip, servoRotate;

const int bumpers = A2;
int servoArmPin = 9;
int servoGripPin = 10;
int servoRotatePin = 11;
int gripperPad = A2;


//----------Gripper variables-------
int grip = 0;
int angle = 0;
int count = 0;
int curr = 0;
int prev = 0;
int gripThreshold = 40; //threshold at which we consider a dice grabed
int maxClosePoint = 100;//the angle that the griper will close to before relizing there is no dice

//----------Motor control pins-------
int RMotorSpeed = 6;
int RMotorDir = 7;

int LMotorSpeed = 5;
int LMotorDir = 4;
//----------Line Sensor pins-------
int LSensor = 3;
int MSensor = 5;
int RSensor = 4;
// --- analog values read from IR line sensor
int L = 0;
int M = 0;
int R = 0;
//---encoder pins --
//int LEncoder = 3;
//int REncoder = 2;

int LCount = 0;
int RCount = 0;

//-----Movement Variables ----
int forwardSpeedL = 105; //value given for motor forward movement speed    recomend 100 for charged batries and 110 for less charged
int forwardSpeedR = 110;

int rotateSpeedL = 105; //value given for motor rotation speed
int rotateSpeedR = 105; //value given for motor rotation speed

int driftTime = 700;    //time to drift when croses an intersection to center over it
int rotateStartDelay = 600; //delay when performing a turn to give time to clear current line first

//----------Ir line sensor checking variable-----
long long sensorReadings[2] = {0, 0};
int IRIO[] = {0, 0, 0};     //values of IRLine sensor [L,M,R]
int IRIOTrials[] = {0, 0, 0}; //value used by checkSensors to store results
int IRtrialCount = 4;         //how many readings (at 5msec apart) to take
int IRtrialThreshold = 3;     //how many of those readings need to show a line for IRIO to show line
int IRanalogThreshold = 945;  //what reading of the line sensor is needed to show a line. higher is less sensitve
//Position Variables
int botCoords[] = {2, 0, 0}; // x and y postion, 0,1,2,3 Direction (N,E,S,W)
int nextTargetCoords[] = {0, 0};

//----------Bluethooth vars----
int checkAvailable;
int checkRead;

//int testpath[8][2] = {{0, 1}, {1, 1}, {1, 2}, {2, 2}, {2, 1}, {1, 1}, {1, 0}, {0, 0}};
//int testpath[8][2] = {{0, 1}, {0, 0}, {1, 0}, {0, 0}, {0, -1}, {0, 0}, {-1, 0}, {0, 0}};
//int testpath[14][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {1, 2}, {1, 1}, {1, 0}, {2, 0}, {2, -1}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, {0, 0}};
int testpath[4][2] = {{2, 0}, {2, 1}, {2, 2}, {1, 2}};

int testpath1_1[5][2] = {{2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}};
int testpath1_2[4][2] = {{2, 1}, {2, 2}, {2, 3}, {1, 3}};
int testpath1_3[8][2] = {{2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 5} , {4, 5}, {5, 5}};
int testpath1_4[3][2] = {{2, 1}, {2, 2}, {1, 2}};
int testpath1_5[2][2] = {{2, 1}, {1, 1}};

int testpath2_1[5][2] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}};
int testpath2_2[6][2] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {2, 4}, {1, 4}};
int testpath2_3[6][2] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 4}, {5, 4}};
int testpath2_4[7][2] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {2, 4}, {1, 4}, {1, 5}};
int testpath2_5[7][2] = {{3, 1}, {3, 2}, {3, 3}, {3, 4}, {4, 4}, {5, 4}, {5, 5}};

int testpath3_1[5][2] = {{4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}};
int testpath3_2[8][2] = {{4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {3, 5}, {2 , 5}, {1, 5}};
int testpath3_3[4][2] = {{4, 1}, {4, 2}, {4, 3}, {5, 3}};
int testpath3_4[3][2] = {{4, 1}, {4, 2}, {5, 2}};
int testpath3_5[2][2] = {{4, 1}, {5, 1}};

int pathLength [3][5] = {{5, 4, 8, 3, 2}, {5, 6, 6, 7, 7}, {5, 8, 4, 3, 2}};

int startSpot = 1;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(RMotorDir, OUTPUT);
  pinMode(LMotorDir, OUTPUT);

  pinMode(RMotorSpeed, OUTPUT);
  pinMode(LMotorSpeed, OUTPUT);

  pinMode(LSensor, INPUT);
  pinMode(MSensor, INPUT);
  pinMode(RSensor, INPUT);

  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);

  servoArm.attach(servoArmPin);
  servoGrip.attach(servoGripPin);
  servoRotate.attach(servoRotatePin);


  servoArm.write(160);
  servoGrip.write(0);
  servoRotate.write(90);//center arm
  pinMode(bumpers, INPUT); //left bumper

  // pinMode(LEncoder, INPUT);
  // pinMode(REncoder, INPUT);

  // attachInterrupt(digitalPinToInterrupt(LEncoder), countL, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(REncoder), countR, CHANGE);

  Serial.begin(9600);
}

void loop() {
  //sweepForLostLine();

  calibrateSensors();
  //completePath(testpath3_5, pathLength[3 - 1][4]);

  if (startSpot == 1) {
    botCoords[0] = 2;
    completePath(testpath1_1, pathLength[1 - 1][0]);
    completePath(testpath1_2, pathLength[1 - 1][1]);
    completePath(testpath1_3, pathLength[1 - 1][2]);
    completePath(testpath1_4, pathLength[1 - 1][3]);
    completePath(testpath1_5, pathLength[1 - 1][4]);
  }

  if (startSpot == 2) {
    botCoords[0] = 3;
    completePath(testpath2_1, pathLength[2 - 1][0]);
    completePath(testpath2_2, pathLength[2 - 1][1]);
    completePath(testpath2_3, pathLength[2 - 1][2]);
    completePath(testpath2_4, pathLength[2 - 1][3]);
    completePath(testpath2_5, pathLength[2 - 1][4]);
  }

  if (startSpot == 3) {
    botCoords[0] = 4;
    completePath(testpath3_1, pathLength[3 - 1][0]);
    completePath(testpath3_2, pathLength[3 - 1][1]);
    completePath(testpath3_3, pathLength[3 - 1][2]);
    completePath(testpath3_4, pathLength[3 - 1][3]);
    completePath(testpath3_5, pathLength[3 - 1][4]);
  }
  delay(1000);

}
void completePath(int path[][2], int pathLength) {
  //int pathLenght = (sizeof(path)/sizeof(path[0]));
  Serial.println(pathLength);

  for (int i = 0; i < pathLength; i++) {
    moveToCordinate(path[i]);
  }
  pickUpDice();
  for (int i = pathLength - 1; i >= 0; i--) {
    moveToCordinate(path[i]);
  }
  if(botCoords[2]==1){
    RotateBot(1);
  }
  if(botCoords[2]==3){
    RotateBot(-1);
  }
  GoalDrop();

}

void moveToCordinate(int cord[]) {
  //find the difrence betweenCordinates
  int cordDifrence[2] = {0, 0};
  cordDifrence[0] = cord[0] - botCoords[0];
  cordDifrence[1] = cord[1] - botCoords[1];
  //first check that the cordinates are adjecent and we can make it there
  if (abs(cordDifrence[0]) > 1 || abs(cordDifrence[1]) > 1) {
    Serial.println("this isnt a valid next cordinate, fatal error");
  }
  if (abs(cordDifrence[0]) > 0 && abs(cordDifrence[1]) > 0) {
    Serial.println("this isnt a valid next cordinate,fatal error");
  } if (cordDifrence[0] == 0 && cordDifrence[1] == 0) {
    Serial.println("at postion, no need to move");
    return;
  }
  int targetOrientation;
  if (cordDifrence[0] == 1) {
    targetOrientation = 1;
  }
  if (cordDifrence[0] == -1) {
    targetOrientation = 3;
  }
  if (cordDifrence[1] == 1) {
    targetOrientation = 0;
  }
  if (cordDifrence[1] == -1) {
    targetOrientation = 2;
  }
  Serial.println("target Oreintation is: " + String(targetOrientation));
  int neededRotation;
  neededRotation = targetOrientation - botCoords[2];
  Serial.println("Needed rotation is: " + String(neededRotation));
  if (neededRotation == -1 || neededRotation == 3) {
    Serial.println("Need to turn left");
    RotateBot(-1);
  }
  if (neededRotation == 1 || neededRotation == -3) {
    Serial.println("Need to turn right");
    RotateBot(1);
  }
  if (neededRotation == 2 || neededRotation == -2) {
    Serial.println("Need to turn 180");
    RotateBot(1);
    RotateBot(1);
  }
  if (neededRotation == 0) {
    Serial.println("Allready Facing the right way");
  }
  MoveForwardAlongLine(0);
  botCoords[0] = cord[0];
  botCoords[1] = cord[1];

}



int lastSeenLineDirection = 0;
//moves forward along line till the next intersection/cordinate
void MoveForwardAlongLine(int bumperFlag) {
  int noLineSeenCount = 0;
  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);
  //intial forward movement
  analogWrite(RMotorSpeed, forwardSpeedR);
  analogWrite(LMotorSpeed, forwardSpeedL);
  delay(200);
  while (1) {
    digitalWrite(RMotorDir, HIGH);
    digitalWrite(LMotorDir, HIGH);
    checkSensors();//update sensors
    if ( (bumperFlag == 0 && (IRIO[0] == 1 && IRIO[1] == 1 && IRIO[2] == 1)) || (bumperFlag == 1 && digitalRead(bumpers) == 0)) { //if see all back dont move
      delay(driftTime); //delay to drift to center of intersection
      analogWrite(RMotorSpeed, 0);
      analogWrite(LMotorSpeed, 0);
      delay(200);
      return;
    } else {
      if (IRIO[0] == 0 && IRIO[1] == 1 && IRIO[2] == 0) {
        analogWrite(RMotorSpeed, forwardSpeedR);
        analogWrite(LMotorSpeed, forwardSpeedL);
        noLineSeenCount = 0;
        //lastSeenLineDirection = 0;
      }
      else if (IRIO[0] == 1 && IRIO[1] == 0 && IRIO[2] == 0) {
        analogWrite(RMotorSpeed, forwardSpeedR);
        analogWrite(LMotorSpeed, forwardSpeedL / 2);
        noLineSeenCount = 0;
        lastSeenLineDirection = -1;
      }
      else if (IRIO[0] == 0 && IRIO[1] == 0 && IRIO[2] == 1) {
        analogWrite(RMotorSpeed, forwardSpeedR / 2);
        analogWrite(LMotorSpeed, forwardSpeedL);
        noLineSeenCount = 0;
        lastSeenLineDirection = 1;
      } else { //if we see no line count it, and if we dont see a line for a bit start sweeping
        noLineSeenCount++;
        if (noLineSeenCount > 60) {
          /*
            if (lastSeenLineDirection == -1) {
            analogWrite(RMotorSpeed, forwardSpeedR);
            analogWrite(LMotorSpeed, forwardSpeedL/2);
            }
            if (lastSeenLineDirection == 1) {
            analogWrite(RMotorSpeed, forwardSpeedR/2);
            analogWrite(LMotorSpeed, forwardSpeedL);
            }
          */
          noLineSeenCount = 0;
          findLostLine();

          //sweepForLostLine();
        }
      }
    }
  }
}
void findLostLine() {
  if (sweepForLostLine() == 0) {
    digitalWrite(RMotorDir, LOW);
    digitalWrite(LMotorDir, LOW);
    analogWrite(RMotorSpeed, forwardSpeedR);
    analogWrite(LMotorSpeed, forwardSpeedL);
    while (IRIO[0] == 0 && IRIO[1] == 0 && IRIO[2] == 0) {
      checkSensors();
    }
  }
}


//rotate bot 90 deg until it sees a line again
//dirFlag -1 for left, 1 for right
void RotateBot(int dirFlag) {
  if (dirFlag == -1) {
    //set wheel directions
    digitalWrite(RMotorDir, HIGH);
    digitalWrite(LMotorDir, LOW);
  }
  if (dirFlag == 1) {
    //set wheel directions
    digitalWrite(RMotorDir, LOW);
    digitalWrite(LMotorDir, HIGH);
  }
  //set speeds
  analogWrite(RMotorSpeed, rotateSpeedR);
  analogWrite(LMotorSpeed, rotateSpeedL);
  delay(rotateStartDelay);//delay to clear line first
  checkSensors();
  while (IRIO[1 + dirFlag] == 0) {
    checkSensors();
  }
  delay(100);
  //stop motors and reset directions
  analogWrite(RMotorSpeed, 0);
  analogWrite(LMotorSpeed, 0);
  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);
  //update rotation in memeory
  botCoords[2] = (botCoords[2] + dirFlag) % 4;
}

int sweepForLostLine() {
  analogWrite(RMotorSpeed, rotateSpeedR);
  analogWrite(LMotorSpeed, rotateSpeedL);
  int sweepDir = -1; //-1 is left, 1 is right
  int sweepCount = 1;
  while (sweepCount < 4) {
    if (sweepDir == -1) {
      digitalWrite(RMotorDir, HIGH);
      digitalWrite(LMotorDir, LOW);
    } if (sweepDir == 1) {
      digitalWrite(RMotorDir, LOW);
      digitalWrite(LMotorDir, HIGH);
    }
    for (int i = 0; i < sweepCount * 10; i++) {
      checkSensors();
      if (IRIO[0] == 1 || IRIO[1] == 1 || IRIO[2] == 1) {
        return 1;//succes,
      }
    }
    sweepDir = -sweepDir;
    sweepCount++;
  }
  return 0;//no line
}
//takes IRtrialCount readings from the sensors 5msec
//from each other and sets IRIO aproprielty if more then
//IRtrialThreshold readings showed a line
void checkSensors() {
  IRIOTrials[0] = 0;
  IRIOTrials[1] = 0;
  IRIOTrials[2] = 0;

  IRIO[0] = 0;
  IRIO[1] = 0;
  IRIO[2] = 0;

  for (int i = 0; i < IRtrialCount; i++) {
    L = analogRead(LSensor);
    M = analogRead(MSensor);
    R = analogRead(RSensor);
    if (L > IRanalogThreshold) {
      IRIOTrials[0] += 1;
    }
    if (M > IRanalogThreshold) {
      IRIOTrials[1] += 1;
    }
    if (R > IRanalogThreshold) {
      IRIOTrials[2] += 1;
    }
    delay(5);
  }
  //set values acording to threshold
  if ( IRIOTrials[0] >= IRtrialThreshold) {
    IRIO[0] = 1;
  }
  if ( IRIOTrials[1] >= IRtrialThreshold) {
    IRIO[1] = 1;
  }
  if ( IRIOTrials[2] >= IRtrialThreshold) {
    IRIO[2] = 1;
  }




  Serial.println(String(IRIO[0]) + " " + String(IRIO[1]) + " " + String(IRIO[2]));
}

int testDelay = 600;
int forwardDelay = 400;
int backwardDelay = 600;
int endPickupForwardDelay = 150;
void pickUpDice() {
  //save the tempoary postion a direction of the robot
  int tempCoords[3];
  tempCoords[0] = botCoords[0];
  tempCoords[1] = botCoords[1];
  tempCoords[2] = botCoords[2];
  //move forward till we hit the wall
  MoveForwardAlongLine(1);
  delay(testDelay);
  //reverse a bit
  digitalWrite(RMotorDir, LOW);
  digitalWrite(LMotorDir, LOW);
  analogWrite(LMotorSpeed, forwardSpeedL);
  analogWrite(RMotorSpeed, forwardSpeedR);
  delay(backwardDelay);
  analogWrite(LMotorSpeed, 0);
  analogWrite(RMotorSpeed, 0);

  //move forward just a bit and stop
  digitalWrite(RMotorDir, HIGH);
  digitalWrite(LMotorDir, HIGH);
  analogWrite(LMotorSpeed, forwardSpeedL);
  analogWrite(RMotorSpeed, forwardSpeedR);
  delay(forwardDelay);
  analogWrite(LMotorSpeed, 0);
  analogWrite(RMotorSpeed, 0);
  centerDice(); //try this new fancy monouver
  //try 3 diffrent grips
  if (Grip(90) != 1) {
    if (Grip(105) != 1) {
      if (Grip(75) != 1) {

      }
    }
  }
  servoRotate.write(90);

  //move back a bit
  analogWrite(LMotorSpeed, forwardSpeedL);
  analogWrite(RMotorSpeed, forwardSpeedR);
  delay(endPickupForwardDelay);

  RotateBot(1); //rate 180
  MoveForwardAlongLine(0);
  botCoords[2] = (tempCoords[2] + 2) % 4;  //reset direction cause we rotated 180 not 90



}

void GoalDrop() {
  int tempOrientation = botCoords[2];
  MoveForwardAlongLine(1);
  delay(200);
  servoArm.write(180);
  delay(500);
  servoArm.write(100);
  servoGrip.write(0);
  delay(500);// wait a sec
  //reverse a bit
  digitalWrite(RMotorDir, LOW);
  digitalWrite(LMotorDir, LOW);
  analogWrite(LMotorSpeed, forwardSpeedL);
  analogWrite(RMotorSpeed, forwardSpeedR);
  delay(300);
  RotateBot(1); //rotate 180
  delay(150);
  botCoords[2] = (tempOrientation + 2) % 4;
  servoArm.write(180);//move arm up
  MoveForwardAlongLine(0);
}
int Grip(int armRotateAngle) {
  angle = 0;
  servoRotate.write(armRotateAngle);//rotate the arm
  servoGrip.write(angle); //open the jaws
  delay(400);//wait for it to hapen
  servoArm.write(70); //move arm down
  delay(200);//wait till that hapends
  grip = 0; //reset gripvalue
  while (grip < gripThreshold) {
    grip = analogRead (gripperPad);
    servoGrip.write(++angle);
    Serial.println("gripforce " + String(grip) + " grip angle" + String(angle));//debug
    delay(50);//close delay, adjust for closing speed
    if (angle > maxClosePoint) { //if we get past this point assume we missed a dice
      servoArm.write(180); //move arm back up
      delay(100); //wait a moment so it moves
      return 0;
    }
  }
  //if we get here we have griped the ball

  delay(100);
  servoGrip.write(angle - 0);//let go a bit
  delay(100);
  servoArm.write(180); //move arm back up
  delay(300);
  return 1;
}
//pushes the dice to the center of where we want to pick it up
int centerDiceMovementDelay = 400;
void centerDice() {
  int i = 50;
  servoGrip.write(120);//close jaws
  delay(centerDiceMovementDelay);
  servoRotate.write(i);//rotate the arm to outside left
  delay(centerDiceMovementDelay);
  servoArm.write(75); //move arm down
  delay(centerDiceMovementDelay);
  while (i < 75) {
    servoRotate.write(++i);//rotate the arm to inside
    delay(40);
  }
  servoRotate.write(60);//move back to no knock dice before moving up
  delay(centerDiceMovementDelay);
  servoArm.write(130); //move arm up
  delay(centerDiceMovementDelay);
  i = 140;
  servoRotate.write(i);//rotate the arm to outside rigth
  delay(centerDiceMovementDelay);
  servoArm.write(75); //move arm down
  delay(centerDiceMovementDelay);

  while (i > 105) {
    servoRotate.write(--i);//rotate the arm to inside
    delay(40);
  }
  servoRotate.write(120);//move back to no knock dice before moving up
  delay(centerDiceMovementDelay);
  servoArm.write(170); //move arm up
  servoRotate.write(90);
  delay(centerDiceMovementDelay);
}
int calibrateSensorsReadings = 40;
void calibrateSensors() {
  sensorReadings[0] = 0; //floor
  sensorReadings[1] = 0;  //line

  for (int i = 0; i < calibrateSensorsReadings; i++) {
    sensorReadings[0] += (analogRead(LSensor) + analogRead(RSensor)) / 2;
    sensorReadings[1] += analogRead(MSensor);
    //Serial.println(String(sensorReadings[0]) + "    " + String(sensorReadings[1]));

    delay(10);
  }

  IRanalogThreshold = ((sensorReadings[0] / calibrateSensorsReadings) + (sensorReadings[1] / calibrateSensorsReadings)) / 2 + 5;

  Serial.println(IRanalogThreshold);
}



