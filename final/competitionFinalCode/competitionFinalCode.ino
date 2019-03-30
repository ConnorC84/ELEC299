//Testing file for different functions
#include "functions.h"
#include <Servo.h>


//Digital
#define leftEncoder   2
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
    getStartingPosition();
    waitButton();
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
//	servoPan.attach(servoPanPin);
//	servoTilt.attach(servoTiltPin);
//	servoGrip.attach(servoGripPin);

    pinMode(LED, OUTPUT);

	//Analog Pins
    pinMode(leftIR, INPUT);
    pinMode(centreIR, INPUT);
    pinMode(rightIR, INPUT);
	pinMode(gripSensor, INPUT);
	pinMode(bumper, INPUT);
	
//	IR.attach(receivePin, transmitPin); //transmit is random 

  Serial.begin(9600);
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
    Serial.print("Pushbutton pressed");
			return;
        }
        Serial.print("Waiting for press");
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
	char start; //placeholder for starting position
	
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

void loop(){
	forward();
}
