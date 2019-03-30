//functions header file for robot code 

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
#define gripSensor    A1
#define bumper      A2
#define frontIR     A0

//----------IR Starting Values-----------------
int L = 0;
int M = 0;
int R = 0;
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

int driftTime = 700;
int rotateStartDelay = 600;

int leftBumpThres = 200; //if under 200 its left bumper
int rightBumpThres = 400; //if greater than 400 its right 
int bothBumpers = 0; //if both bumpers are pressed 
//-------------------------------------------------

//Driving Functions
void forward(int flag){
	int lineCounter = 0;
	digitalWrite(leftDirection, HIGH);
	digitalWrite(rightDirection, HIGH);
	
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
	
	delay(300);
//	blink();
	
	while(true){
		digitalWrite(leftDirection, HIGH);
		digitalWrite(rightDirection, HIGH);
	//	checkLine();
	}
}
