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
#define leftIR      A3
#define centreIR    A4
#define rightIR     A5
#define gripSensor  A1
#define bumper      A2
#define frontIR     A0


void setup(){
    Init();
    //getStartingPosition();
    waitButton();
    Serial.begin(9600);  //Serial Monitor will be used throughout testing to see whats happening  
}

void loop(){
//	//keep loop empty so that we can test things
//  forward();
//  Serial.print(analogRead(leftSpeed));
//  Serial.print("\t");
//  Serial.print(analogRead(rightSpeed));
//  Serial.print("\t");
//  Serial.print(analogRead(leftIR));
//  Serial.print("\t");
//  Serial.print(analogRead(centreIR));
//  Serial.print("\t");
//  Serial.print(analogRead(rightIR));
//  Serial.print("\t");
//  Serial.print(analogRead(frontIR));
//  Serial.print("\n");
//  delay(100);

  pickUp();
  delay(5000);
}
