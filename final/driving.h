// Driving Functions for the car

// --- analog values read from IR line sensor
int L = 0;
int M = 0;
int R = 0;
//---encoder pins --
//int leftEncoder = 3;
//int rightEncoder = 2;

int LCount = 0;
int RCount = 0;

//-----Movement Variables ----
int forwardSpeed = 100; //value given for motor forward movement speed    recomend 100 for charged batries and 110 for less charged
int rotateSpeed = 100; //value given for motor rotation speed
int driftTime = 700;    //time to drift when croses an intersection to center over it
int rotateStartDelay = 600; //delay when performing a turn to give time to clear current line first

//----------Ir line sensor checking variable-----
int IRIO[] = {0, 0, 0};     //values of IRLine sensor [L,M,R]
int IRIOTrials[] = {0, 0, 0}; //value used by checkSensors to store results
int IRtrialCount = 4;         //how many readings (at 5msec apart) to take
int IRtrialThreshold = 2;     //how many of those readings need to show a line for IRIO to show line
int IRanalogThreshold = 945;  //what reading of the line sensor is needed to show a line. higher is less sensitve


//moves forward along line till the next intersection/cordinate
void MoveForwardAlongLine(int targetCord[]) {
  //intial forward movement, to ensure we get over the intersection already on
  analogWrite(leftSpeed, forwardSpeed);
  analogWrite(rightSpeed, forwardSpeed);
  delay(100);
  while (1) { //loop

    checkSensors();//update sensors
    if (IRIO[0] == 1 && IRIO[1] == 1 && IRIO[2] == 1) { //if see all black lines, then we crossed an intersection
      delay(driftTime); //delay to drift to center of intersection before killing motors
      analogWrite(rightSpeed, 0);
      analogWrite(leftSpeed, 0);
      delay(200);
      return; //return out of this function, we have arrived 
    } else { //if we havnet arrived to the intersection use this logic to follow lines
      if (IRIO[0] == 0 && IRIO[1] == 1 && IRIO[2] == 0) { //line is in midddle move ahead
        analogWrite(rightSpeed, forwardSpeed);
        analogWrite(leftSpeed, forwardSpeed);
      }
      else if (IRIO[0] == 1 && IRIO[1] == 0 && IRIO[2] == 0) { //line is on left, drift left a bit
        analogWrite(rightSpeed, forwardSpeed);
        analogWrite(leftSpeed, forwardSpeed / 2);
      }
      else if (IRIO[0] == 0 && IRIO[1] == 0 && IRIO[2] == 1) { //line is on right, drift right a bit
        analogWrite(rightSpeed, forwardSpeed / 2);
        analogWrite(leftSpeed, forwardSpeed);
      }
    }
  }
}

//rotate bot 90 deg until it sees a line again
//dirFlag -1 for left, 1 for right
void RotateBot(int dirFlag) {
  if (dirFlag == -1) {
    //set wheel directions
    digitalWrite(rightDirection, HIGH);
    digitalWrite(leftDirection, LOW);
  }
  if (dirFlag == 1) {
    //set wheel directions
    digitalWrite(rightDirection, LOW);
    digitalWrite(leftDirection, HIGH);
  }
  //set speeds
  analogWrite(rightSpeed, rotateSpeed);
  analogWrite(leftSpeed, rotateSpeed);
  delay(rotateStartDelay);//delay to clear line first
  checkSensors(); // check the sensors 
  while (IRIO[1 + dirFlag] == 0) { //keep checking sensors until the sensor on that side sees a line 
    checkSensors();
  }
  //stop motors and reset directions
  analogWrite(rightSpeed, 0);
  analogWrite(leftSpeed, 0);
  digitalWrite(rightDirection, HIGH);
  digitalWrite(leftDirection, HIGH);
  //update rotation in memeory
  botCoords[2] = (botCoords[2] + dirFlag) % 4;
}
//takes IRtrialCount number of readings from the sensors 5msec
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
