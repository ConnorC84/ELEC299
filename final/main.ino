/* Main Code for the ELEC299 Robot Competition

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

*/ 

//-----------Libraries-------------------------
#include "QSerial.h"
#include <Servo.h>
#include "driving.h"
#include "matrix.h"
#include "gripper.h"

//-----------Pin Definitions-------------------

//Digital
#define leftEncoder 	2
#define rightEncoder 	3
#define leftDirection 	4
#define leftSpeed 		5
#define rightSpeed		6
#define rightDirection 	7
#define receivePin 		8
#define servoArmPin		9
#define servoGripPin	10
#define _____ 			11
#define LED 			12

//Analog
#define leftIR			A1
#define centreIR 		A2
#define rightIR 		A3
#define Bumper 			A4

QSerial IR;
Servo servoArm, servoGrip
bool LEFT = false;
bool RIGHT = true;

//----------IR Starting Values----------------
int L = 0;
int M = 0;
int R = 0;

//---------
int LCount = 0;
int RCount = 0;

int startingPoint = -1; //Global variable that will be changed when IR beacon is successfully read

void setup(){
	startingPoint = getStartingPos();
    Init();
    WaitButton();
    Serial.begin(9600);    
}

//Function that recieves the signal from the IR Beacon. Once it has received the same char 10 times it will allow the program to initialize
int getStartingPos(){
	int counter = 0;
	int prevTemp = 0;
	int temp = IR.receive(200); //receive the IR ASCII 
	char start = ''; //placeholder for starting position
	
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
	} while (counter <= 10)

	return int (start);
}

void Init(){
	//Digital Pins
    pinMode(leftBumper, INPUT);
    pinMode(rightBumper, INPUT);
    pinMode(leftDirection, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(rightDirection, OUTPUT);
    pinMode(leftEncoder, INPUT);
    pinMode(rightEncoder, INPUT);
    pinMode(button, INPUT);
    pinMode(LED, OUTPUT);

	//Analog Pins
    pinMode(leftIR, INPUT);
    pinMode(centreIR, INPUT);
    pinMode(rightIR, INPUT);
	
	IR.attach(receivePin, transmitPin);
}

void WaitButton(){
    int read = 1; //force the while loop
    while(read){
        read = digitalRead(Bumper); //will = 0 when button is pressed (button pulls down a LOW signal)
        Serial.println("Waiting for button...");
        delay(100);
    }

    digitalWrite(LED, HIGH);
    Serial.println("Program initiated!");
}


void loop(){
	
	for(int i = 0; i < ballsToGet; i++){
		moveToPosition[i];
	}
	pickUpBall();
	
	delay(1000);
}
















