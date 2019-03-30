//robot class, will have functionality for reoreinting 

#include "Arduino.h"
#include "robot.h"
#include "matrix.h"

//-----------Pin Definitions-------------------

//Digital
#define leftEncoder 	2
#define rightEncoder 	3
#define leftDirection 	4
#define leftSpeed 		5
#define rightSpeed		6
#define rightDirection 	7
#define receivePin 		8
#define servoPanPin		9
#define servoTiltPin	10
#define servoGripPin	11
#define LED 			12
#define transmitPin 	15

//Analog
#define leftIR			A0
#define centreIR 		A1
#define rightIR 		A2
#define gripSensor		A3
#define bumper 			A4
#define frontIR			A5

robot::begin(){
  Serial.begin(9600);
  Serial.println("Hello world\n");
}
//contsructor , function that handles the creation and setup of instances
robot::robot(){
	
}
//
//robot::robot(/*other input for constructor?*/ ){
//
//}

void robot::SETUP(){
	//Write Function code in here
	
}

void robot::print(){
	Serial.print("Hello there   ");
}

bool robot::grab(int angleToRotate){
	angle = 0;
	servoPan.write(angleToRotate); //rotate the arm
	servoGrip.write(angle); //open up the grip to start
	delay(500);
	servoTilt.write(70); //move arm down into position
	delay(200);
	int grip = 0;
	while(grip < gripThres){
		grip = analogRead(gripSensor);
		angle++; //increase angle for servo grip (make it close)
		servoGrip.write(angle);
		delay(40);
		if (angle > maxAngle){ //test case if the object is not picked up
			servoTilt.write(180); //move back up
			delay(200);
			return false;
		}
	} // at this point the ball is gripped and the arm is raised
}
