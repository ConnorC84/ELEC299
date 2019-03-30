//Testing file for different functions

#define LED 			12
#define transmitPin 	15

//Analog
#define leftIR			A0
#define centreIR 		A1
#define rightIR 		A2
#define gripSensor		A3
#define bumper 			A4
#define frontIR			A5

QSerial IR;
Servo servoPan, servoTilt, servoGrip
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
	pinMode(recievePin, INPUT);
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

void loop(){
	forward(1);
}
