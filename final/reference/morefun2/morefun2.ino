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
int lSpeed = 160;
int rSpeed = 160;
int IRThresh = 1000;
int turnSpeed = 120;
int turnDelay = 750;

//QSerial IR;

Servo servoGrip, servoTilt, servoPan;

void setup()
{
  Init();
  waitButton();
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
//  int L = analogRead(leftIR);
//  int C = analogRead(centreIR);
//  int R = analogRead(rightIR);
//  Serial.print(L);
//  Serial.print("  ");
//  Serial.print(C);
//  Serial.print("  ");
//  Serial.print(R);
//  Serial.println("     ");
//  delay(500);

  servoTilt.write(160);

  robotL();
//  nextInt();
//  nextInt();
//  nextInt();
//  stop();
//  forceForward();
//  stop();
//  turnRight();
//  stop();
//  nextInt();
//  wallStuff();
//  nextInt();
//  stop();
//  turnLeft();
//  stop();
//  nextInt();
//  nextInt();
//  dropAtHome();

  delay(100000);


  
 // turnRight();
//  while (notAtInt() == true)
//   {
//      forward();
//      lineTrack();
//      delay(50);
//    }
//    stop();
//    delay(1000);
//    positionForTurn();
//    stop();
//    delay(1000);
//    turnRight();
//    stop();
//    delay(1000);
//    forceForward();  
//    pickUp();
//    drop();
//    delay(5000);
}

void nextInt()
{
  while (notAtInt() == true)
   {
      forward();
      lineTrack();
      delay(50);
	  if(collisionDetected()){
		  delay(2000);
	  }
    }
    delay(100);
}

bool collisionDetected(){
  if (digitalRead(frontIR) == 1){
    return true;
  }
  return false;
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
  delay(200);
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
  delay(1000);
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
    int rSpeed = 100;
    int lSpeed = 160;
    analogWrite(leftSpeed, lSpeed);
    analogWrite(rightSpeed, rSpeed);
  }
  else if(R > IRThresh)
  {
    Serial.println("right drift");
    int lSpeed = 100;
    int rSpeed = 160;
    analogWrite(leftSpeed, 100);
    analogWrite(rightSpeed, 160);
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


void positionForTurn()
{
  digitalWrite(leftDirection, HIGH);
  digitalWrite(rightDirection, HIGH);
  
  analogWrite(leftSpeed, lSpeed);
  analogWrite(rightSpeed, rSpeed);
  delay(1000);
}

void wallStuff()
{
  while(analogRead(bumper) != 0)
  {
    forward();
    lineTrack();
    delay(50);
  }
  delay(250);
  stop();
  backward();
  delay(200);
  stop();
  pickUp();
  backward();
  delay(500);
  stop();
  turnRight();
  stop();
  turnRight();
  stop();
}

void dropAtHome()
{
   while(analogRead(bumper) != 0)
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



//--------Servo Stuff----------

void pickUp()
{
  Serial.print("hello");
  servoPan.write(90);
  delay(1000);
  servoGrip.write(10);
  delay(1000);
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
    delay(40);  
  }
  servoTilt.write(160);
  delay(000);
}

void drop()
{
  servoTilt.write(180);
  delay(500);
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

void robotL()
{
  //dice 1
  nextInt();
  nextInt();
  nextInt();
  nextInt();
  nextInt();

  //dice 2


  //dice 3


  //dice 4


  //dice 5
  nextInt();
  nextInt();
  nextInt();
  stop();
  forceForward();
  stop();
  turnRight();
  stop();
  nextInt();
  wallStuff();
  nextInt();
  stop();
  forceForward();
  stop();
  turnLeft();
  stop();
  nextInt();
  nextInt();
  dropAtHome();
  
}

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
    


   // dice 2
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      wallStuff();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();

    //dice 3
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      wallStuff();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();

    //dice 4
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight();
      stop();
      wallStuff();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      stop();
      nextInt();
      nextInt();
      stop();
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

    //dice 5
    
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      stop();
      wallStuff();
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight();
      stop();
      nextInt();
      nextInt();
      stop();
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
  }
  
//this is the code for the far right robot
void robotR(){
  //get first dice on the far wall - this works 
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      wallStuff(); //pick up dice hopefully
      //turnLeft();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      nextInt(); 
      dropAtHome();

 //get second dice - works! 
      nextInt();
      stop();
      forceForward();
      stop();
      turnRight(); 
      nextInt();
      wallStuff();
      forceForward();
      nextInt();
      stop();
      forceForward();
      turnLeft();
      dropAtHome();


//get dice 5 -- swaped 3 & five to facilitate the process of getting the thir
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      nextInt();
      nextInt();
      nextInt();
      wallStuff();
      nextInt();
      stop();
      forceForward();
      turnRight();
      nextInt();
      nextInt();
      dropAtHome();

//get dice 4
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      nextInt();
      wallStuff();
      nextInt();
      stop(); 
      forceForward();
      turnRight();
      nextInt();
      nextInt();
      dropAtHome();

//get dice 3
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      stop();
      forceForward();
      stop();
      turnLeft();
      nextInt();
      nextInt();
      wallStuff();
      nextInt();
      nextInt();
      stop();
      forceForward();
      turnRight();
      nextInt();
      nextInt();
      nextInt();
      nextInt();
      dropAtHome();
    
}
