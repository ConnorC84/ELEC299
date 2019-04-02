//Testing file for different functions
#include "functions.h"
#include <Servo.h>


//Digital

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
#define leftIR            A0
#define centreIR          A1
#define rightIR           A2
#define gripSensor        A3
#define bumper            A4
#define frontIR           A5

int startingPosition = -1;

void setup(){
    Init();
    startingPosition = 1;
    //startingPosition = getStartingPosition();
    waitButton();  
    Serial.begin(9600);  //Serial Monitor will be used throughout testing to see whats happening  
}

void loop(){
//  switch(startingPosition){
//    case 1:
//      completeRoute1();
//      break;
//     default:
//      //
//      break;
//  }
//	forward(1);
//  rotate(1);
	//keep loop empty so that we can test things
    
  
//  Serial.print(analogRead(leftSpeed));
//  Serial.print("\t");
//  Serial.print(analogRead(rightSpeed));
//  Serial.print("\t");
  forward(1);
  Serial.print(analogRead(leftIR));
  Serial.print("\t");
  Serial.print(analogRead(centreIR));
  Serial.print("\t");
  Serial.print(analogRead(rightIR));
  Serial.print("\t");
  Serial.print(digitalRead(frontIR));
  Serial.print("\n");
  delay(100);

}
