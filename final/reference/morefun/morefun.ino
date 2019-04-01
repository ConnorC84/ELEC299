//#include "QSerial.h"
#include <Servo.h>

//-----------Pin Definitions-------------------

//Digital
#define leftEncoder       2
#define rightEncoder      3
#define leftDirection     4
#define leftSpeed         5
#define rightSpeed        6
#define rightDirection    7
#define receivePin        8
#define servoPanPin       9
#define servoTiltPin      10
#define servoGripPin      11
#define LED               12
#define transmitPin       15

//Analog
#define leftIR            A0
#define centreIR          A1
#define rightIR           A2
#define gripSensor        A3
#define bumper            A4
#define frontIR           A5
int lSpeed = 100;
int rSpeed = 100;
int IRThresh = 700;

//QSerial IR;

Servo servoGrip, servoTilt, servoPan;

void setup()
{
  Init();
  Serial.begin(9600);
}

void Init()
{
  //Digital Pins
    pinMode(leftDirection, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);

  //Analog Pins
    pinMode(leftIR, INPUT);
    pinMode(centreIR, INPUT);
    pinMode(rightIR, INPUT);
    pinMode(bumper, INPUT);

    //Servo
      servoTilt.attach(10);
      servoGrip.attach(11);
      servoPan.attach(9);
}

void loop() 
{
while (notAtInt() == true)
   {
      forward();
      lineTrack();
      delay(50);
    }
    stop();
    delay(1000);
    positionForTurn();
    stop();
    delay(1000);
    turnRight();
    stop();
    delay(1000);
    forceForward(); 
}

void forward()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
}

void forceForward()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  delay(500);
}

void backward()
{
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
}

void stop()
{
  analogWrite(leftSpeed, 0);
  analogWrite(rightSpeed, 0);
}

bool notAtInt()
{
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

void lineTrack()
{
  //Serial.println("1");
  int L = analogRead(leftIR);
  int C = analogRead(centreIR);
  int R = analogRead(rightIR);

  if (L > IRThresh)
  {
    Serial.println("left drift");
    int rSpeed = 75;
    int lSpeed = 100;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);
  }
  else if(R > IRThresh)
  {
    Serial.println("right drift");
    int lSpeed = 75;
    int rSpeed = 100;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);
  }
  else 
  {
  }
}

void turnRight()
{
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  
  delay(500);
  while(analogRead(centreIR) < IRThresh)
  {
  
  }
}

void turnLeft() //will use this for turn around bc trust me it'll work
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, LOW);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  
  delay(500);
  while(analogRead(centreIR) < IRThresh)
  {
  
  }
}


void positionForTurn()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  delay(1000);
}



//--------Servo Stuff----------

void pickUp()
{
  Serial.print("hello");
  servoTilt.write(160);
  delay(1000);
  servoPan.write(90);
  delay(1000);
  servoGrip.write(0);
  delay(1000);
  servoTilt.write(55);
  delay(1000);
  int grip = 0;
  int gripThres = 1000;
  int angleToRotate = 0;
  while(grip < gripThres)
  {
    grip = analogRead(gripSensor);
    Serial.println(grip);
    angleToRotate++; //increase angle for servo grip (make it close)
    servoGrip.write(angleToRotate);
    delay(40);  
  }
  servoTilt.write(160);
  delay(5000);
}

void drop()
{
  servoTilt.write(0);
  delay(50);
  servoGrip.write(0);
}

//  Serial.print(R);
//  Serial.print("  ");
//  Serial.print(C);
//  Serial.print("  ");
//  Serial.print(R);
//  Serial.println("     ");
