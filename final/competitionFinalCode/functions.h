//functions header file for robot code 

/*  to do
- Brent - get bluetooth serial working for testing, interrupt for bumpers 
- Adam - intersection thing 
- Juliana Matrix thing, keep working on failsafe for IR 
- Connor look into Line following code 
	- write a function so if we put it diagonally it will straighten and follow line 
	
ILC 317 at 8:30 Sunday
ILC 317 at 3:30 on Monday 
ILC 317 8:30 on Monday 
ILC 317 at 5:30 to 11:30

fail safe functions to implement 
- if IR Beacon doesn't work 
- collision detection (front facing IR). Maybe with interrupts idk 
-
- 
*/

#include "Arduino.h"
#include <Servo.h>
#include "QSerial.h"
#include <math.h>

//Digital
#define BTRecieve		0
#define BTTransmit		1
#define leftEncoder   	2
#define rightEncoder  	3
#define leftDirection   4
#define leftSpeed     	5
#define rightSpeed    	6
#define rightDirection  7
#define receivePin    	8
#define servoPanPin   	9	
#define servoTiltPin  	10
#define servoGripPin  	11
#define LED       		12
#define transmitPin   	15

//Analog
#define leftIR            A0
#define centreIR          A1
#define rightIR           A2
#define gripSensor        A3
#define bumper            A4
#define frontIR           A5

QSerial IRReceiver;
Servo servoGrip, servoTilt, servoPan;

//----------IR Line Sensing -----------------
int L = 0;
int M = 0;
int R = 0;
int IRValues[3] = {0, 0, 0};
const int IRThresh = 980; //TESTED
const int intersectionDriftTime = 100;
//---------------------------------------------

//---------Encoder Starting Values------------- should we use these?
int LCount = 0;
int RCount = 0;
//---------------------------------------------

//Bumper values
const int bothBumpersHit = 0; //encoder circuit on analog pin
 
//---------TUNING VALUES------------------------
const int intersectionDelay = 700;
const int wallThreshold = 200;
int collisionThreshold = 580;
int gripThres = 700; //Test
int maxAngle = 150; 

int forwardSpeedLeft = 120; //retune on competition day
int forwardSpeedRight = 120; //retune on competition day

int rotateSpeedLeft = 120;
int rotateSpeedRight = 120;

int rotateStartDelay = 600;

int leftBumpThres = 200; //if under 200 its left bumper
int rightBumpThres = 400; //if greater than 400 its right 
int bothBumpers = 0; //if both bumpers are pressed 
//-------------------------------------------------

//Matrix Stuff
typedef struct {
	int x;
	int y;
	int direction;
} position;

position currentPosition = {0, 0, 0};
position balls[15] = {
	{ 4, 0, 3},{ 3, 0, 3},{ 2, 0, 3},{ 1, 0, 3},{ 0, 0, 3},
	{ 0, 0, 0},{ 0, 1, 0},{ 0, 2, 0},{ 0, 3, 0},{ 0, 4, 0},
	{ 4, 4, 1},{ 3, 4, 1},{ 2, 4, 1},{ 1, 4, 1},{ 0, 4, 1}
};

position routeTest[2] = {};

//All Function protocols
//Starting
void blink();
void Init();
void waitButton();
int getStartingPosition();

//Driving
int intersection(); //I think this works *test it*
void forward(int number); // need to update intersection 
bool collisionDetected();
void centerOnInt();
void backward(); //maybe
bool lostLine();
void findLine();
void move(int direction, long speed); //combine forward and backward into this
void followLine();	
bool notAtInt();
void rotate(int direction);
void stop(); 
//Servo
void pickUp();
void drop();
bool grab(int angleToRotate);
//Matrix
void movePosition(int final[]);
void completePass(int route[][2], int routeLength);
void completeRoute(int startPoint);
//-------------------------------------------------

//Starting Functions
void blink(){
  //blink LED 3 times, will be useful when watching how code functions
  for(int i = 0; i < 3; i++){
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
  }
}

void Init(){
  //Digital Pins
  //attachInterrupt(bumperInterupt, stop, FALLING);
//    pinMode(rightEncoder, INPUT);
    pinMode(leftDirection, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
  pinMode(receivePin, INPUT);
  //Servo Motor Controls
  servoPan.attach(servoPanPin);
  servoTilt.attach(servoTiltPin);
  servoGrip.attach(servoGripPin);
  servoPan.write(90);
  servoTilt.write(90);
  servoGrip.write(90);
    pinMode(LED, OUTPUT);

  //Analog Pins
    pinMode(leftIR, INPUT);
    pinMode(centreIR, INPUT);
    pinMode(rightIR, INPUT);
  pinMode(gripSensor, INPUT);
  pinMode(bumper, INPUT);
 pinMode(frontIR, INPUT);
  IRReceiver.attach(receivePin, transmitPin); //transmit is random 
  blink(); //will blink when setup is complete
  Serial.begin(9600);
}

void waitButton(){ //for testing, remove wrapper for competition
	while(true){
		if(analogRead(bumper) < 550){
		Serial.print("Pushbutton pressed");
		blink();
		delay(200);
		return;
        }
		
		Serial.println("Waiting for press");	
    }
}

int getStartingPosition(){
  //Function that recieves the signal from the IR Beacon. 
  //Once it has received the same char 10 times it will allow the program to initialize
  
  int counter = 0;
  int prevTemp = 0;
  int temp = IRReceiver.receive(200); //receive the IR ASCII 
  char start; //placeholder for starting position
  
  do 
  {   
    if(temp > 0 && temp < 60){ //valid character coming in
      if(prevTemp == temp){
        counter++;
      }
      
      //Last iteration!
      if(counter == 10){
        start = (int)temp; //We have our starting position
        Serial.println("Starting position is: " + start);
      }
    }
    prevTemp = temp;
  } while (counter <= 10);
  
  delay(500);
  return int (start);
}
//-------------------------------------------------

//Driving Functions
void forward(int num){ //function drives from one intersection to the next
  Serial.println("Driving forward...");
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  for(int i = 0; i < num; i++){
  int timer = millis();
  bool flag1 = true;
  do {
      analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight);
      followLine();
	    flag1 = notAtInt();
//      if(millis() < timer + 1000){
//        flag1 = true;
//      }
      if(lostLine()){
        Serial.println("Lost line!");
      }

 	      if(collisionDetected()){
          Serial.println("Collision detected");
          stop();
          delay(2000);
        }     
	} while(flag1);

  Serial.println("At Next Intersection!");
  centerOnInt();
  stop();
  delay(1000); 
  }
  //some code to check if were at the point we want to be at or if it was just detecting a collision?
}

void centerOnInt(){
  int timer = millis();
  while(millis() < (timer + intersectionDelay)){
    Serial.println("Forcing forward..");
    analogWrite(leftSpeed, forwardSpeedLeft);
    analogWrite(rightSpeed, forwardSpeedRight); 
  }
}

bool lostLine(){
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);
  if (L < IRThresh && C < IRThresh && R < IRThresh)
  {
    return true;
  }
  else
  {
    return false;
  } 
}

void findLine(){
  //backtrack and find the line code
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);
  bool noLine = true;                                       //check if it detects a line on any of the 3 sensors
  digitalWrite(leftDirection, LOW);                         // set direction backwards
  digitalWrite(rightDirection, LOW);
  while(noLine){
    if (L < IRThresh && C < IRThresh && R < IRThresh)       //once it finds the line, exit the loop and execute follow line code
    {
      analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight); 
    }
    else {
      Serial.println("Found line");
      stop();
      noLine=false;
    }
  }
  //followLine();
}

bool collisionDetected(){
  if (digitalRead(frontIR) == 1){
    return true;
  }
  return false;
}

void followLine(){ //still needs to debug
 Serial.println("Line tracking...");
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);

  if (L > IRThresh)
  {
    Serial.println("left drift");

    int rSpeed = 75;
    int lSpeed = 200;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);

    while(analogRead(centreIR) < IRThresh){
      //
    }
  }
  else if(R > IRThresh)
  {
    Serial.println("right drift");
    int lSpeed = 75;
    int rSpeed = 200;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);

    while(analogRead(centreIR) < IRThresh ){
      //
    }
  }
  else 
  {
  }
}

bool notAtInt(){
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);
  if (L > IRThresh && C > IRThresh && R > IRThresh)
  {
    return false;
  }
  else
  {
    return true;
  } 
}

void rotate(int angle){
	//Rotate function
  switch(angle){
    case 1:
      Serial.println("Turning Right");
      delay(100);
      digitalWrite(leftDirection, LOW);
      digitalWrite(rightDirection, HIGH);
  
        analogWrite(leftSpeed, forwardSpeedLeft);
        analogWrite(rightSpeed, forwardSpeedRight);
  
      delay(1000);
      while(analogRead(centreIR) < IRThresh)
      {
        //rotating 
      }
      Serial.println("Turned Right");
      stop();
      break;

    case 0:
      //This is for rotating 180 degrees 
      Serial.println("Turning around");
      delay(100);
      digitalWrite(leftDirection, LOW);
      digitalWrite(rightDirection, HIGH);
  
        analogWrite(leftSpeed, forwardSpeedLeft);
        analogWrite(rightSpeed, forwardSpeedRight);
  
      delay(1000);
      while(analogRead(centreIR) < IRThresh)
      {
        //rotating 
      }
      //Fix to ensure always turns 180 with a timer!
      Serial.println("Turned Right");
      stop();
      break;
      
    case -1:
      digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, LOW);
  
      analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight);
  
      delay(500);
      while(analogRead(centreIR) < IRThresh)
      {
        //rotating 
      }
      stop();
      break; 
    }
  }

void stop(){
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

void backTrack(){
	digitalWrite(leftDirection, LOW);
      digitalWrite(rightDirection, LOW);
	  analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight);
	  
	  int time = millis();
	  while(millis() < (time + wallThreshold)){
	  }
  
}
	
void drive(){
	digitalWrite(leftDirection, HIGH);
      digitalWrite(rightDirection, HIGH);
	  analogWrite(leftSpeed, forwardSpeedLeft);
      analogWrite(rightSpeed, forwardSpeedRight);
}
	
//-------------------------------------------------

//SERVO Functions
void pickUp(){
  servoTilt.write(180);
	while(analogRead(bumper) != 0){
		drive();
    followLine();
	}
	stop();
	backTrack();
	stop();
  servoGrip.write(0);
  servoTilt.write(70);
	int grip = 0;
  int gripThres = 1023;
  int currentAngle = 0;
  while(grip < gripThres)
  {
    grip = analogRead(gripSensor);
    Serial.println(grip);
    currentAngle++; //increase angle for servo grip (make it close)
    servoGrip.write(currentAngle);
    delay(40);  
  }
  servoTilt.write(160);
  delay(5000);
	rotate(1);
  rotate(1);
}

void drop(){
	servoTilt.write(180);
  while(analogRead(bumper) != 0){
    drive();
  }
  stop();
	delay(200);
	servoTilt.write(180);
	delay(500);
	servoTilt.write(0);
	delay(100);
  servoGrip.write(0);
  rotate(1);	
}
//-------------------------------------------------

//MATRIX Functions
void path1_ball1(){
  //straight through
  forward(5);
  pickUp();
  forward(5);
  drop();
}

void completeRoute1(){
  path1_ball1();
//  path1_ball2();
//  path1_ball3();
//  path1_ball4();
}
