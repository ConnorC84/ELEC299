//--successfully completes all runs for group 17s robot

#include "QSerial.h"
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
int rSpeed = 160;
int lSpeed = 160;
int IRThresh = 1000;
int turnSpeed = 140;
int turnDelay = 750;
int bumpThresh = 650;
QSerial IR;

Servo servoGrip, servoTilt, servoPan;

void setup()
{
  Init();
  waitButton();
  Serial.begin(9600);
}

//---------------------PINS SETUP---------------------

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

//----------------WHATS GOING TO RUN----------------

void loop() 
{
  servoTilt.write(160);

  //robotR();
  //robotL();
  robotC();


}


//----------------------DRIVING THINGS---------------


void nextInt()
{
  while (notAtInt() == true)
   {
      forward();
      lineTrack();
      delay(50);
    }
    delay(100);
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
  delay(300);
}

void forceForwardV2()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  delay(200);
}

void forceForwardV3()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  delay(300);
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
  delay(400);
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
    int rSpeed = 120;
    int lSpeed = 180;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);
  }
  else if(R > IRThresh)
  {
    Serial.println("right drift");
    int lSpeed = 120;
    int rSpeed = 180;
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
  
  analogWrite(leftSpeed, turnSpeed);
  analogWrite(rightSpeed, turnSpeed);
  
  delay(turnDelay);
  while(analogRead(centreIR) < IRThresh)
  {
  
  }
}

void turnLeft() //will use this for turn around bc trust me it'll work
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, LOW);
  
  analogWrite(leftSpeed, turnSpeed);
  analogWrite(rightSpeed, turnSpeed);
  
  delay(turnDelay);
  while(analogRead(centreIR) < IRThresh)
  {
  
  }
}

void wallStuff()
{
  while(analogRead(bumper) > bumpThresh)
  {
    forward();
    lineTrack();
    delay(50);
  }
  delay(250);
  stop();
  backward();
  delay(220);
  stop();
  pickUp();
  backward();
  delay(700);
  stop();
  turnRight();
  turnRight();
  stop();
}

void wallStuffV2()
{
  while(analogRead(bumper) > bumpThresh)
  {
    forward();
    lineTrack();
    delay(50);
  }
  delay(250);
  stop();
  backward();
  delay(175);
  stop();
  pickUp();
  backward();
  delay(800);
  stop();
  turnRight();
  turnRight();
  stop();
}

void dropAtHome()
{
   while(analogRead(bumper) > bumpThresh)
  {
    forward();
    lineTrack();
    delay(50);
  }
  stop();
  drop();
  backward();
  delay(500);
  stop();
  turnRight();
  stop();
}

void dropAtHomeV2()
{
   while(analogRead(bumper) > bumpThresh)
  {
    forward();
    lineTrack();
    delay(50);
  }
  stop();
  drop();
  backward();
  delay(500);
  stop();
  turnRight();
  stop();
  backward();
  delay(300);
  stop();
  delay(200);
  forceLittleRotateLeft();
}

void forceLittleRotateLeft()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, LOW);
  
  analogWrite(leftSpeed, turnSpeed);
  analogWrite(rightSpeed, turnSpeed);
  
  delay(200);
  stop();
}



//-------------------Servo Stuff--------------------

void pickUp()
{
  Serial.print("hello");
  servoTilt.write(65);
  delay(1000);
  int grip = 0;
  int gripThres = 1000;
  int currentGrip = 0;
  while(grip < gripThres)
  {
    grip = analogRead(gripSensor);
    Serial.println(grip);
    currentGrip++; //increase angle for servo grip (make it close)
    servoGrip.write(currentGrip);
    delay(5);  
  }
  servoTilt.write(160);
}

void drop()
{
  servoTilt.write(180);
  delay(100);
  servoTilt.write(0);
  delay(100);
  servoGrip.write(0);
  delay(100);
  servoTilt.write(180);
}


void waitButton(){ //for testing, remove wrapper for competition
  while(true){
    if(analogRead(bumper) < 550){
    Serial.print("Pushbutton pressed");
    delay(200);
    return;
        }
  }
}
//-------------------------TEST IR SENSORS----------------------

//int L = analogRead(leftIR);
//int C = analogRead(centreIR);
//int R = analogRead(rightIR);
//  Serial.print(L);
//  Serial.print("  ");
//  Serial.print(C);
//  Serial.print("  ");
//  Serial.print(R);
//  Serial.println("     ");
//  delay(500);


//-----------------------PATHS------------------------


//-------------LEFT ROBOT------------------------

void robotL()
{

  //dice 1

  
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  wallStuff();
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();
  

  //dice 2
  
  nextInt();
  forceForwardV3();
  stop();
  turnLeft();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  dropAtHome();
  forceLittleRotateLeft();
  


  //dice 3
  
  nextInt();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  nextInt();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();

  //dice 4
  
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();
  
  


  //dice 5


  nextInt();
  nextInt();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();
 
}








//------------------ CENTRE ROBOT------------------------
  void robotC()
  {
    //dice 1
    nextInt();
    nextInt();
    nextInt();
    nextInt();
    nextInt();
    wallStuff();
    nextInt();
    nextInt();
    nextInt();
    nextInt();
    nextInt();
    dropAtHome();
    forceLittleRotateLeft();
    


   // dice 2
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      wallStuffV2();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();
      forceLittleRotateLeft();

    //dice 3
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      wallStuff();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();
      forceLittleRotateLeft();

    //dice 4
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      forceForward();
      delay(3);
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      wallStuff();
      nextInt();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();     
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();
      forceLittleRotateLeft();

    //dice 5
    
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnLeft();
      stop();
      wallStuff();
      nextInt();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();     
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();
      forceLittleRotateLeft();
  }

  //---------------------RIGHT ROBOT----------------------------

//this is the code for the far right robot
void robotR(){
  //get first dice on the far wall - this works 

  nextInt();
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  wallStuff();
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();
  
 //get second dice - works! 

  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  dropAtHome();
  forceLittleRotateLeft();

//get dice 3 -- swaped 3 & five to facilitate the process of getting the thir

  nextInt();
  nextInt();
  nextInt();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  wallStuffV2();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();

//get dice 4

   nextInt();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  nextInt();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();


//get dice 5

  nextInt();
  nextInt();
  nextInt();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  nextInt();
  dropAtHome();
  forceLittleRotateLeft();
    
}
