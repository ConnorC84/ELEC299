/* Main Code for the ELEC299 Robot Competition

Team 16:	Brent Champion 
			Jafer Jaradat
			Jingwen Zhang
			
Team 17: 	Adam Coholan
			Connor Chappell
			Nick Jones
						
Team 18:	Juliana Brown
			Jiaqi Li
			Rueben Rasaselvan

---------------Wiring Schematic----------------
DIGITAL PINS: 
0. Bluetooth things
1. Bluetooth things
2. Left Encoder
3. Right Encoder
4. Left Speed
5. Left Direction
6. Right Direction
7. Right Speed
8. IR Recieve Pin
9. Servo Motor 1
10. Servo Motor 2
11. Servo Motor 3
12. LED (Indicate when program has begun!)

ANALOG PINS: 
0. IR sensor Left 
1. IR sensor center
2. IR sensor right 
3. Pressure Sensor on Grip
4. Bumpers (Encoder Circuit)
5. Front Facing IR Sensor 
*/ 
//---------------------------------------------

//-----------Libraries-------------------------
#include "QSerial.h"
#include <Servo.h>
#include <EEPROM.h>
//---------------------------------------------

//-----------Pin Definitions-------------------

//Digital
#define leftEncoder 	2
#define rightEncoder 	3
#define leftDirection 	4 //black
#define leftSpeed 		5 //red
#define rightSpeed		6 //red
#define rightDirection 	7 //black
#define receivePin 		8
#define servoPanPin		9
#define servoTiltPin	10
#define servoGripPin	11
#define LED 			12
#define transmitPin 	15 //random, only need it for the definition

//Analog
#define leftIR			A0
#define centreIR 		A1
#define rightIR 		A2
#define gripSensor		A3
#define bumper 			A4
#define frontIR			A5

QSerial IR;
Servo servoPan, servoTilt, servoGrip;
byte leftSpeedPWM, rightSpeedPWM; //will use soon
//---------------------------------------------

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
int line[] = {0, 0, 0};
//---------------------------------------------

//--------Matrix Definition--------------------
int routeToFollow[8][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; //max route is 8 instructions long, always 2 columns

int ballsToGet = 5;
int length[3][5] = {{5, 4, 8, 3, 2}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}; // Need to complete 
/*	
	
	This is a little complicated and is still under development
	The goal is that in the main loop it will call "complete route", 
	which takes in the starting position based on IR and then follows 
	a hardcoded route for each case. Each route will consist of 5 balls 
	to pick up on the course. For this, the program will break down the 
	routine of retrieving each ball into a "Pass". The "Complete Pass" 
	function will call the "path" function with the starting point and 
	the iteration, as well as the length of the path. Within the "path" 
	function will be hard-coded routes to follow for each starting point.
	At this point, only the leftmost route has been developed however the 
	rest will be developed over the time remaining to the competition. 
	Furthermore, each path should be designed such that the chance of 
	collision is minimized as much as possible. The team will implement
	functions to deal with collisions but will minimize the likelyhood. 
	
*/

//--------Tuned Delays-------------------------
int rotateDelay = 100;
int backwardsDelay = 100;
// will need to find more delays throughout testing

//---------------------------------------------

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

int startingPoint = -1; //Global variable that will be changed when IR beacon is successfully read

void setup(){
    Init();
    WaitButton();
    Serial.begin(9600);  //Serial Monitor will be used throughout testing to see whats happening  
}

void Init(){
	//Digital Pins
	pinMode(leftEncoder, INPUT);
    pinMode(rightEncoder, INPUT);
    pinMode(leftDirection, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
	pinMode(receivePin, INPUT);
	//Servo Motor Controls
	servoPan.attach(servoPanPin);
	servoTilt.attach(servoTiltPin);
	servoGrip.attach(servoGripPin);

    pinMode(LED, OUTPUT);

	//Analog Pins
    pinMode(leftIR, INPUT);
    pinMode(centreIR, INPUT);
    pinMode(rightIR, INPUT);
	pinMode(gripSensor, INPUT);
	pinMode(bumper, INPUT);
	
	IR.attach(receivePin, transmitPin); //transmit is random 
	
	blink(); //will blink when setup is complete
}

void blink(){
	//blink LED 3 times, will be useful when watching how code functions
	for(int i = 0; i < 3; i++){
		digitalWrite(LED, HIGH);
		delay(100);
		digitalWrite(LED, LOW);
		delay(100);
	}
}

void WaitButton(){
    while(true){
		if(analogRead(bumper) < 550){
			return;
        }
      delay(100);
    }
	blink();
    //Serial.println("Program initiated!");
}

int getStartingPos(){
	//Function that recieves the signal from the IR Beacon. 
	//Once it has received the same char 10 times it will allow the program to initialize
	
	int counter = 0;
	int prevTemp = 0;
	int temp = IR.receive(200); //receive the IR ASCII 
	char start;
	
	do 
	{		
		if(temp > 0 && temp < 60){ //valid character coming in
			if(prevTemp == temp){
				counter++;
			}
			
			//Last iteration!
			if(counter == 10){
				start = (int)temp; //We have our starting position
			}
		}
		prevTemp = temp;
	} while (counter <= 10);

	blink();
	delay(1000);
	return int (start);
}

void loop() {
  completeRoute(getStartingPos());
}

// DRIVING FUNCTIONS 
void completeRoute(int startPoint){
	switch(startPoint){
//		case 1:
//			current[0] = startPoint + 1;
//		  for(int counter = 0; counter < ballsToGet; counter++){
//        
//        completePass(routeToFollow, length[startPoint-1][counter]);
//      } 
//		break;
			
		case 2:
			current[0] = startPoint + 1;
      for(int counter = 0; counter < ballsToGet; counter++){
        getPath(startPoint, counter);
        completePass(routeToFollow, length[startPoint-1][counter]);
      } 
			
			break;
		
		case 3:
//			current[0] = startPoint + 1;
//      for(int counter = 0; counter < ballsToGet; counter++){
//        int routeToFollow[8][2] = getPath(startPoint, counter);
//        completePass(routeToFollow, length[startPoint-1][counter]);
//      } 
			
			break;	

		default:
			cantReadPosition(); //safety net if IR beacon isn't recieved properly
	}
}

void completePass(int route[][2], int length){
	//This function takes in an array limited to 2 columns, and the length of the path that it will follow
//This is the function that will be called in the loop 
	for(int distance = 0; distance < length; distance++){
		movePosition(route[distance]);
	} //When this loop is done, the robot should be in the correct position to pick up the object
	
	pickUp();
	
	for(int distance = length; distance > 0; distance--){
		movePosition(route[distance]);
	} //When this loop is done, the robot will be back at its starting position
	
	//if statements COME BACK TO THIS
	
	drop();
}
	
void movePosition(int final[]){
	int difference[2] = {0, 0}; //initialize variable to hold distance between points at 0
	
	for(int i = 0; i < 2; i++){
		difference[i] = final[i] - current[i];
	} //Now the diference array has the distance to travel in x and y component
	
	if(difference[0] == 0 && difference[1] == 0)
		//Serial.println("Already in position");
		return;
		
	int nextOreintation = getOreintation(difference[1]); //Find which way the bot needs to turn 
	//Serial.println("You would like to face: " + goalOreintation);
	
	rotate(nextOreintation - current[2]); //rotate the difference from the current to the next oreintation so bot is in place to move
	
	forward(0); //move forward one block
	blink(); //for testing
	
	updatePosition(final); //update the bots position
}

int getOreintation(int rotationRequired){
  //fix this
	return 1;
}
	
void forward(int flag){
	int lineCounter = 0;
	digitalWrite(leftDirection, HIGH);
	digitalWrite(rightDirection, HIGH);
	
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
	
	delay(300);
	blink();
	
	while(true){
		digitalWrite(leftDirection, HIGH);
		digitalWrite(rightDirection, HIGH);
		checkLine();
	}
}

void backward(){
	digitalWrite(leftDirection, LOW);
	digitalWrite(rightDirection, LOW);
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
}

void rotate(int direction){
	if(direction == -90){ //turn left
		turnLeft();
	}
	if(direction == 90){ //turn right
		turnRight();
	}

 if(direction == 180){
    turnRight();
    turnRight();
 }
	
	analogWrite(rightSpeed, rotateSpeedRight);
	analogWrite(leftSpeed, rotateSpeedLeft);
	delay(rotateDelay);
	
	checkLine();
	while(line[direction+1] == 0){
		checkLine();
	}
	delay(200);
	resetMotors();
	
	current[2] += direction/90;
	current[2] %= 4;
}

void resetMotors(){
	analogWrite(leftSpeed, 0);
	analogWrite(rightSpeed, 0);
	digitalWrite(leftDirection, HIGH);
	digitalWrite(rightDirection, HIGH);
}

void turnLeft(){
	digitalWrite(leftDirection, LOW);
	digitalWrite(rightDirection, HIGH);
}

void turnRight(){
	digitalWrite(rightDirection, LOW);
	digitalWrite(leftDirection, HIGH);
}

void stop(){
	analogWrite(leftSpeed, 0);
	analogWrite(rightSpeed, 0);
}

// SERVO FUNCTIONS 
void pickUp(){
	int temp[3];
	for(int i = 0; i < 3; i++){
		temp[i] = current[i];
	}
	//moveForward until bumpers hit
	forward(0);
	delay(200);
	//slightly reverse
	backward();
	delay(backwardsDelay);
	resetMotors(); //reset the motors to be forward now
	
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
	
	delay(200);
	stop();
	
	//test different rotations to grip the ball
	if(grab(90)){
		if(grab(105)){
			if(grab(75)){}
		}
	} //by here it will have had to pick up a ball!
	
	servoPan.write(90);
	
	backward();
	rotate(180); 
	forward(0);
	current[2] = (temp[2] + 2) % 4; //reset oreintation in global position array
}

void drop(){
	int temp = current[2];
	forward(1);
	delay(200);
	servoTilt.write(180);
	delay(500);
	servoTilt.write(100);
	servoTilt.write(0);
	delay(500);
	
	backward();
	
	delay(300);
	rotate(180);
	delay(200);
	current[2] = (temp + 2) % 4;
	servoTilt.write(180);
	forward(0);
}

bool grab(int angleToRotate){
	int angle = 0;
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

void updatePosition(int updated[]){
	for(int i = 0; i < 2; i++){
		current[i] = updated[i];
	}
}

void getPath(int start, int iteration){
	switch(start){
		case 1: //leftmost route
			switch(iteration){
				case 1: //retrieve first ball
					routeToFollow = {{2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 5} , {4, 5}, {5, 5}};
					break;
					
				case 2:
					routeToFollow = {{2, 1}, {2, 2}, {2, 3}, {1, 3}};
					break;
					
				case 3:
					routeToFollow = {{2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}};
					break;
				
				case 4: 
					routeToFollow = {{2, 1}, {2, 2}, {1, 2}};
					break;
					
				case 5:
					route* = {{2, 1}, {1, 1}};
					break;
			}
		case 2: //middle route
			//still need to write
			break;
		
		case 3: //rightmost route
			//still need to write
			break;
	}
	return returnArray;
}

//SAFETY NETS
void adjustSpeed(){
	//Function that will adjust speeds based on the IR line readings
	//If robot is not driving straight, update EEPROM and recheck
}

void checkLine(){
	//Function that will check to ensure following on a line 
}

void cantReadPosition(){
	//Write a function that will provide a default safety net if any robot can't figure out its position!
}
