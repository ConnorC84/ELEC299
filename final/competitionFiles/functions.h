//functions header file for robot code 


/* Shit to do
- Brent - get bluetooth serial working for testing, interrupt for bumpers 
- Adam - intersection thing 
- Juliana Matrix thing, keep working on failsafe for IR 
- Connor look into Line following code 
	- write a function so if we put it diagonally it will straighten and follow line 
	
ILC 317 at 3:30 on Monday 
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
#define leftEncoder   2
#define rightEncoder  3
#define leftDirection   4
#define leftSpeed     5
#define rightSpeed    6
#define rightDirection  7
#define receivePin    8
#define servoPanPin   9
#define servoTiltPin  10
#define servoGripPin  11
#define LED       12
#define transmitPin   15

//Analog
#define leftIR      A3
#define centreIR    A4
#define rightIR     A5
#define gripSensor  A1
#define bumper      A2
#define frontIR     A0

//----------IR Starting Values-----------------
int L = 0;
int M = 0;
int R = 0;
int IRValues[3] = {0, 0, 0};
const int IRThreshold = 900; //TEST THIS 
const int driftTime = 100;
//---------------------------------------------

//---------Encoder Starting Values-------------
int LCount = 0;
int RCount = 0;
//---------------------------------------------

//--------Position Starting Variables----------
int current[] = {2, 0, 0};

//---------TUNED VALUES------------------------
int gripThres = 100; //TEST THIS!
int maxAngle = 150; 

int forwardSpeedLeft = 105; //retune on competition day
int forwardSpeedRight = 105; //retune on competition day

int rotateSpeedLeft = 105;
int rotateSpeedRight = 105;

int rotateStartDelay = 600;

int leftBumpThres = 200; //if under 200 its left bumper
int rightBumpThres = 400; //if greater than 400 its right 
int bothBumpers = 0; //if both bumpers are pressed 
//-------------------------------------------------

//All Functions
//Driving
void checkIRSensor(); //This works
void intersection(); //I think this works *test it*
void forward(); // need to update intersection 
void backward(); //maybe
void move(); //combine forward and backward into this 
void rotate(int direction);
void stop(); //sets direction to forward and speed to 0
//Servo stuff is good 
void pickUp();
void drop();
bool grab(int angleToRotate);

//Driving Functions

void checkIRSensors(){
  for(int i = 0; i < 5; i++){
    L = analogRead(leftIR);
    M = analogRead(centreIR);
    R = analogRead(rightIR);
    
    if(L > IRThreshold){
      IRValues[0] = 1;
    }
    
    if(M > IRThreshold){
      IRValues[1] = 1;
    }
    
    if(R > IRThreshold){
      IRValues[2] = 1;
    }
    delay(15);
  }
}
int intersection(){
  if(IRValues[0] == 1 && IRValues[1] == 1 && IRValues[2] == 1){
    return 0;
  }
  
  return 1;
}
void forward(){
	int lineCounter = 0;
	digitalWrite(leftDirection, HIGH);
	digitalWrite(rightDirection, HIGH);
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
	
	int intersectionBool = 1;
	
	while(intersectionBool){
		for(int i = 0; i < 5; i++){
			L = analogRead(leftIR);
			M = analogRead(centreIR);
			R = analogRead(rightIR);
		
			if(L > IRThreshold){
				IRValues[0] = 1;
			}
		
			if(M > IRThreshold){
				IRValues[1] = 1;
			}
		
			if(R > IRThreshold){
				IRValues[2] = 1;
			}
		}
		
		intersectionBool = intersection();
		
	}
	analogWrite(leftSpeed, 0);
	analogWrite(rightSpeed, 0);

	}
