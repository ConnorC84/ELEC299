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


//contsructor , function that handles the creation and setup of instances
robot::robot(){
	
}

robot::robot(/*other input for constructor?*/ ){

}

void robot::SETUP(){
	//Write Function code in here
	pinMode(	
}

void robot::print(){
	Serial.print("Hello there");
}