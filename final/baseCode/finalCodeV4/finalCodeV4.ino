/* Main Code for the ELEC299 Robot Competition

Team 16:	Brent Champion 
			    Jafer 
			    JingWen
			
Team 17: 	Adam Coholan
			    Connor Chappell
			    Nick Jones
			
Team 18:	

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
//#include "driving.h"
//#include "matrix.h"
//#include "gripper.h"
//---------------------------------------------

//-----------Pin Definitions-------------------

//Digital
#define leftEncoder 	 2
#define rightEncoder 	 3
#define leftDirection  4
#define leftSpeed 		 5 
#define rightSpeed		 6
#define rightDirection 7
#define receivePin 		 8
#define servoPanPin		 9
#define servoTiltPin	 10
#define servoGripPin	 11
#define LED 		    	 12
#define transmitPin  	 15   //to keep the compiler happy as a pin must be defined

//Analog
#define leftIR		  	 A0
#define centreIR 	  	 A1
#define rightIR 		   A2
#define gripSensor	   A3
#define bumper 			   A4
#define frontIR			   A5

QSerial IR;
Servo servoPan, servoTilt, servoGrip;
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

//---------TUNED VALUES------------------------
int gripThres = 100; //TEST THIS!
int maxAngle = 150; 

int forwardSpeedLeft = 105; //retune on competition day
int forwardSpeedRight = 105; //retune on competition day

int rotateSpeedLeft = 105;
int rotateSpeedRight = 105;

int driftTime = 700;
int rotateDelay = 1000;

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
		if(digitalRead(bumper) < 550){
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
	char start = ' '; //placeholder for starting position -- get rid or leave 0?
	
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
	return int (start);
}

void loop() {
  completeRoute(getStartingPos());
}

// DRIVING FUNCTIONS FROM LINE 190 to 300!
void completeRoute(int startPoint)
{
	switch(startPoint)
		case 1:
			
			
		case 2:
		
		
		case 3:	
    break;	
}

void completePass(int route[][2], int length){
	//This function takes in an array limited to 2 columns, and the length of the path that it will follow
//This is the function that will be called in the loop 
int i;
	for(int distance; distance < length; distance++){
		movePosition(route[i]);
	} //When this loop is done, the robot should be in the correct position to pick up the object
	
	pickUp();
	
	for(int distance = length; distance > 0; distance--){
		movePosition(route[i]);
	} //When this loop is done, the robot will be back at its starting position
	
	//if statements COME BACK TO THIS
	
	drop();
}

int getOrientation(int currentPosition[])
{
  return currentPosition[2];
}
int updatePosition(int final[])
{
  return current[2] = final; 
}



void movePosition(int final[]){
	int difference[2] = {0, 0}; //initialize variable to hold distance between points at 0
	
	for(int i = 0; i < 2; i++){
		difference[i] = final[i] - current[i];
	} //Now the diference array has the distance to travel in x and y component
	
	if(difference[0] == 0 && difference[1] == 0)
		//Serial.println("Already in position");
		return;
		
	int nextOrientation = getOrientation(difference); //Find which way the bot needs to turn 
	//Serial.println("You would like to face: " + goalOreintation);
	
	rotate(nextOrientation - current[2]); //rotate the difference from the current to the next oreintation so bot is in place to move
	
	forward(0); //move forward one block
	blink(); //for testing
	
	updatePosition(final); //update the bots position
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
	//	checkLine();
	}
}

void backward(){
	digitalWrite(leftDirection, LOW);
	digitalWrite(rightDirection, LOW);
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
}

void rotate(int direction){
	if(direction == -1){ //turn left
		turnLeft();
	}
	if(direction == 1){ //turn right
		turnRight();
	}
	
	analogWrite(rightSpeed, rotateSpeedRight);
	analogWrite(leftSpeed, rotateSpeedLeft);
	delay(rotateDelay);
	
	//checkLine();
  
	while(line[flag+1] == 0){
	//	checkLine();
	}
	delay(200);
	resetMotors();
	
	current[2] += flag;
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

// SERVO FUNCTIONS FROM LINE 300 to 400!
void pickUp(){
	int temp[3];
	for(int i = 0; i < 3; i++){
		temp[i] = current[i];
	}
	//moveForward until bumpers hit
	forward(0);
	delay();
	//slightly reverse
	backward();
	delay(backwardsDelay);
	resetMotors(); //reset the motors to be forward now
	
	analogWrite(leftSpeed, forwardSpeedLeft);
	analogWrite(rightSpeed, forwardSpeedRight);
	
	delay();
	stop();
	
	//test different rotations to grip the ball
	if(grip(90)){
		if(grip(105)){
			if(grip(75))
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
	forward();
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
