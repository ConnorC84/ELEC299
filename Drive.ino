//IR Thresholds
#define IR1Threshold 975 
#define IR2Threshold 975
#define IR3Threshold 975 // Note IR3 is the middle one 

//Encoder counter full pass definition
#define pass 16
//IR Sensors
int IR1 = A0;
int IR2 = A1;
int IR3 = A2;

//Encoders
int encoder1 = 10; 
int encoder2 = 9; 

//Motor stuff
int rightDirection = 7;
int leftDirection = 4;
int leftSpeed = 5;
int rightSpeed = 6;

void setup() {
  // IR Sensors 
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);

  // Motor Controllers 
  pinMode(rightDirection, OUTPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightSpeed, OUTPUT);
  pinMode(leftSpeed, OUTPUT);

  // Encoders are inputs - Reads 0 when it passes a point (16 times per full rotation)
  pinMode(encoder1, INPUT);
  pinMode(encoder2, INPUT);
  
  // Start communication with serial monitor at baud rate 9600
  Serial.begin(9600);
}

void loop() {
  countEncoder(true, 3);
  countEncoder(false, 2);
  rotateLeft();
}


//Function that counts the encoder passes 
//Distance parameter is how many times it runs 1 full wheel rotation
void countEncoder(bool direction, int distance){

  int counter = 0;

  while(counter < pass*distance){

    if (direction == true){
      driveForward();
    }

    else{
      driveBackwards();
    }

    if(analogRead(encoder1) == 0){
     counter++;
    }

  }
}// countEcoder()

void driveForward(){

    digitalWrite(leftDirection, HIGH);
    digitalWrite(rightDirection, HIGH);
    analogWrite(leftSpeed, 200);
    analogWrite(rightSpeed, 200); 

}// driveForward()

void driveBackwards(){
  
  digitalWrite(leftDirection, LOW);
  digitalWrite(rightDirection, LOW);
  analogWrite(leftSpeed, 200);
  analogWrite(rightSpeed, 200); 
  
}// driveBackwards

//Function rotates left based on a certain amount of degrees 
void rotateLeft(int degrees){
  int turn = degrees/360;

  int rotate = turn/16;
  int rotated = 0;

  while(rotated < rotate){

    if(analogRead(encoder1) == 0){
     rotated++;
    }

    digitalWrite(leftDirection, LOW);
    digitalWrite(rightDirection, HIGH);
    analogWrite(leftSpeed, 200);
    analogWrite(rightSpeed, 200); 

  }
}// rotateLeft()
